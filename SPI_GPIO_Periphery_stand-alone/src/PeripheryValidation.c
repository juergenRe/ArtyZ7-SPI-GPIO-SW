/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * 
 *
 * This file is a generated sample test application.
 *
 * This application is intended to test and/or illustrate some 
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * SDK application project when you run the "Generate Libraries" menu item.
 *
 */

#include <stdio.h>
#include "xparameters.h"
#include "xil_cache.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "sleep.h"

#include "GPIOControl.h"
#include "SPIControl.h"
#include "IICControl.h"

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define INTCTRL_DEVICE_ID 			XPAR_SCUGIC_0_DEVICE_ID

#define SPI0_INTR_ID				XPS_SPI0_INT_ID					// interrupt ID of SPI 0 device
#define IIC0_INTR_ID				XPS_I2C0_INT_ID					// interrupt ID of IIC 0 device
#define BTN_INTR_ID					XPS_FPGA0_INT_ID				// interrupt ID of GPIO interrupt line
#define BTN_INTR_MASK				1U								// interrupts to handle

#define GPIO_0						XPAR_AXI_GPIO_0_DEVICE_ID			// Buttons 6 bit, LEDs 10bit
//#define GPIO_1						XPAR_AXI_GPIO_1_DEVICE_ID			// LEDs 4 bit
#define SPI_0						XPAR_XSPIPS_0_DEVICE_ID
#define IIC_0						XPAR_XIICPS_0_DEVICE_ID

#define MSK_LED 0x0F

#define SW_DEBOUNCE  2000

#define OLED_RD	0x1
#define OLED_WR 0x0
#define OLED_WR_CMD 0x80			//Co=1, D/C#=0
#define OLED_RD_STATUS 0x80			//Co=1, D/C#=0
#define OLED_CMD_NOP 0xE3
#define OLED_CMD_DISP_ON 0xAF
#define OLED_CMD_DISP_OFF 0xAE

// trigger state definitions
#define TRG_IDLE 	0
#define TRG_RISED	1
#define TRG_RUNNING 2
#define TRG_FALLS 	3
#define TRG_STARTED 4
#define TRG_AS_OFF  5

typedef struct {
	u32 actIn;
	u32 lastIn;
	u32 autoSample;
	u32 actDelay;
	u32 Cnt;
} ButtonState;

typedef struct {
	u16 ByteCount;
	u16 RemainingBytes;
	u32 Status;
	u32 Cnt;
	u32 Active;
} SPIState;

typedef struct {
	u32 ActCnt;
	u32 LastCnt;
	u16 ByteCount;
	u32 Status;
	u8 Active;
	u8 IsSend;
} IICState;

int ScuGicExample(u16 DeviceId);
int SetUpInterruptSystem(XScuGic *pGICInst, XScuGic_Config *pGicConfig, uint gicIntID);
int SetUpGPIOInterrupt(XScuGic *pGICInst, u16 DeviceID, void *pHandler, uint intID, u8 priority, u8 trigger);
int SetUpSPIInterrupt(XScuGic *pGICInst, u16 DeviceID, void *pHandler, uint intID, u8 priority, u8 trigger);
int SetUpIICInterrupt(XScuGic *pGICInst, u16 DeviceID, void *pHandler, uint intID, u8 priority, u8 trigger);
void GPIOInterruptHandler(void *CallbackRef);
void SPIStatusHandler(const void *CallBackRef, u32 StatusEvent, u32 ByteCount);
void IICStatusHandler(void *CallBackRef, u32 StatusEvent);
void OLEDComm(u32 maxIICInt, IICState* iics);

/************************** Variable Definitions *****************************/

XScuGic InterruptController; 	     /* Instance of the Interrupt Controller */
static XScuGic_Config *GicConfig;    /* The configuration parameters of the controller */

// Create a shared variable to be used by the main thread of processing and the interrupt processing
static volatile u32 intGPIOCount;	// count called interrupts
static volatile ButtonState sw;		// actual button status

static volatile u32 intSPICount;	// count called SPI interrupts
static volatile SPIState spis;		// actual spi status

//static volatile u32 intIICCount;	// count called IIC interrupts
static IICState iics;				// actual iic status
static IICState *piics;				// pointer to status word

#define SPI_SCREEN 0				// number of slave to be addressed for screen display
#define SPI_BUF_SIZE 128
#define IIC_BUF_SIZE 128
unsigned char SPIWriteBuffer[SPI_BUF_SIZE] = {'a', '1', 'z'};
unsigned char SPIReadBuffer[SPI_BUF_SIZE+1];

// commands to be send: each line describes nbBytes, type, startIndex
// command list ends with 0 as nbBytes
#define SZ_CMD 3
unsigned char OLEDCmdList[] = {
		2, OLED_WR, 0,
		2, OLED_WR, 2,
		2, OLED_WR, 4,
		0
};

unsigned char OLEDWriteBuffer[IIC_BUF_SIZE] = {
		OLED_WR_CMD, OLED_CMD_NOP,
		OLED_WR_CMD, OLED_CMD_DISP_ON,
		OLED_WR_CMD, OLED_CMD_DISP_OFF,
		OLED_RD_STATUS
};
unsigned char OLEDReadBuffer[4];

static void AssertPrint(const char8 *FilenamePtr, s32 LineNumber){
	xil_printf("ASSERT: File Name: %s ", FilenamePtr);
	xil_printf("Line Number: %d\r\n",LineNumber);
}


int main () 
{

	Xil_ICacheEnable();
	Xil_DCacheEnable();

    intGPIOCount = 0;
    intSPICount = 0;
    //intIICCount = 0;
    iics.ActCnt = 0;
    iics.LastCnt = 0;
    iics.Active = 0;
    piics = &iics;

	// Setup an assert call back to get some info if we assert.
	Xil_AssertSetCallback(AssertPrint);

    //init_platform();
    printf("\n\r--- Entering main ---\n\r");

    // Init GPIO
    int status = GpioInit(GPIO_0);
    printf("--- GPIO initialized. Status: %d ---\n\r", status);
    status = SpiInitMaster(SPI_0);
    printf("--- SPI initialized. Status: %d ---\n\r", status);
    status = IicInit(IIC_0);
    printf("--- IIC initialized. Status: %d ---\n\r", status);

    //set up interrupts
    SetUpInterruptSystem(&InterruptController, GicConfig, INTCTRL_DEVICE_ID);
    SetUpGPIOInterrupt(&InterruptController, GPIO_0, GPIOInterruptHandler, BTN_INTR_ID, 0x20, 0x03);
    SetUpSPIInterrupt(&InterruptController, SPI_0, XSpiPs_InterruptHandler, SPI0_INTR_ID, 0x20, 0x03);
    SetUpIICInterrupt(&InterruptController, IIC_0, XIicPs_MasterInterruptHandler, IIC0_INTR_ID, 0x20, 0x03);
    printf("---Interrupts ready---\n\r");

    u32 maxIICInt = sizeof(OLEDCmdList)/SZ_CMD;
    printf("   Size IIC commandlist: %d NbCmds: %ld", sizeof(OLEDCmdList), maxIICInt);

    print("---Entering main loop---\n\r");
    int running = 1;
	u32 lastGPIOCount = intGPIOCount;
	u32 lastSPICount = intSPICount;
	//u32 lastIICCount = intIICCount;
    while (running != 0){
    	if (intGPIOCount != lastGPIOCount){
    		lastGPIOCount = intGPIOCount;
    		printf("---Catches interrupt. Count: %ld switch: %ld\n\r", lastGPIOCount, sw.actIn);
    		if (sw.lastIn != sw.actIn){
    			sw.lastIn = sw.actIn;
    			GpioLEDOutput(sw.actIn);
    		}
    	}
    	if (intGPIOCount > 20){
    		running = 0;
    	}

    	// handle SPI transfer
    	if(intSPICount != lastSPICount){
    		if (spis.Status == XST_SPI_TRANSFER_DONE) {
    			SPIReadBuffer[spis.ByteCount] = 0x0;
    			printf("   SPI Input: %s ; rem. Bytes: %d; Cnt: %ld\n\r", SPIReadBuffer, spis.RemainingBytes, spis.Cnt);
    		}
    		printf(" Clearing int struct.\n\r");
    		for(int i = 0;i<spis.ByteCount;i++)
    			SPIReadBuffer[i] = 0x00;
    		spis.ByteCount = 0;
    		spis.RemainingBytes = 0;
    		spis.Status = 0;
    		spis.Active = 0;
    		lastSPICount = intSPICount;
    	}
    	if((spis.Active == 0) && (intSPICount < 3)){
    		printf("Start SPI Tx %ld\n\r", intSPICount);
    		SpiReadWrite(SPI_0, SPIWriteBuffer, SPIReadBuffer, 3);
    		spis.Active = 1;
    		spis.ByteCount = 3;
    	}
    	OLEDComm(maxIICInt, piics);
    }

   print("---Exiting main---\n\r");
   Xil_DCacheDisable();
   Xil_ICacheDisable();
   return 0;
}

// setting up global interrupt system
int SetUpInterruptSystem(XScuGic *pGICInst, XScuGic_Config *pGicConfig, uint gicIntID){
    // Initialize the interrupt controller driver so that it is ready to use.
	pGicConfig = XScuGic_LookupConfig(INTCTRL_DEVICE_ID);
	if (NULL == pGicConfig) {
		return XST_FAILURE;
	}

	int Status = XScuGic_CfgInitialize(pGICInst, pGicConfig, pGicConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//Perform a self-test to ensure that the hardware was built correctly
	Status = XScuGic_SelfTest(pGICInst);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	// Setup the Interrupt System
	// Connect the interrupt controller interrupt handler to the hardware
	// interrupt handling logic in the ARM processor.
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(	XIL_EXCEPTION_ID_INT,
									(Xil_ExceptionHandler) XScuGic_InterruptHandler,
									pGICInst);

	// Enable interrupts in the ARM
	Xil_ExceptionEnable();
	return XST_SUCCESS;
}


// setting up GPIO related interrupt system
int SetUpGPIOInterrupt(XScuGic *pGICInst, u16 DeviceID, void *pHandler, uint intID, u8 priority, u8 trigger){
	XGpio *InstancePointer = getGPIOInstance(DeviceID);
	if (NULL == InstancePointer) {
		return XST_FAILURE;
	}

	// Connect a device driver handler that will be called when an interrupt for the device occurs,
	// the device driver handler performs the specific interrupt processing for the device
	int Status = XScuGic_Connect(pGICInst, intID, (Xil_ExceptionHandler)pHandler, (void *)InstancePointer);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// reset any pending interrupts for this source
	XScuGic_CPUWriteReg(pGICInst, XSCUGIC_EOI_OFFSET, intID);

	// Enable the interrupt for the device and set priority and mode
	GpioSwitchSetInt(1);
	//XScuGic_GetPriorityTriggerType(pGICInst, intID, &prio, &trig);

	XScuGic_SetPriorityTriggerType(pGICInst, intID, priority, trigger);
	XScuGic_Enable(pGICInst, intID);
	return XST_SUCCESS;
}

/******************************************************************************/
/**
*
* This function is designed to look like an interrupt handler in a device
* driver. This is typically a 2nd level handler that is called from the
* interrupt controller interrupt handler.  This handler would typically
* perform device specific processing such as reading and writing the registers
* of the device to clear the interrupt condition and pass any data to an
* application using the device driver.  Many drivers already provide this
* handler and the user is not required to create it.
*
* @param	CallbackRef is passed back to the device driver's interrupt
*		handler by the XScuGic driver.  It was given to the XScuGic
*		driver in the XScuGic_Connect() function call.  It is typically
*		a pointer to the device driver instance variable.
*		In this example, we do not care about the callback
*		reference, so we passed it a 0 when connecting the handler to
*		the XScuGic driver and we make no use of it here.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void GPIOInterruptHandler(void *GPIOinstance)
{
	u32 intPending = GpioSwitchGetInt();
	while (intPending > 0){
		if(intPending & SW_BITWIDTH) {
			// got new data
			u32 val = GpioSwitchInput();
			sw.actIn = val;
			GpioSwitchClrInt(SW_BITWIDTH);
			intPending &= ~SW_BITWIDTH;
		}
		//clear any unhandled interrupt
		GpioSwitchClrInt(SW_BITWIDTH);
	}
	intGPIOCount++;
}


// setting up SPI related interrupt system
int SetUpSPIInterrupt(XScuGic *pGICInst, u16 DeviceID, void *pHandler, uint intID, u8 priority, u8 trigger)
{
	XSpiPs *InstancePointer = getSPIInstance(DeviceID);
	if (NULL == InstancePointer) {
		return XST_FAILURE;
	}

	XSpiPs_WriteReg(InstancePointer->Config.BaseAddress, XSPIPS_IDR_OFFSET, 0xFF);
	XSpiPs_WriteReg(InstancePointer->Config.BaseAddress, XSPIPS_SR_OFFSET, 0xFF);
	XSpiPs_SetStatusHandler(InstancePointer, (void *)InstancePointer, SPIStatusHandler);

	// Connect a device driver handler that will be called when an interrupt for the device occurs,
	// the device driver handler performs the specific interrupt processing for the device
	int Status = XScuGic_Connect(pGICInst, intID, (Xil_ExceptionHandler)pHandler, (void *)InstancePointer);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XScuGic_SetPriorityTriggerType(pGICInst, intID, priority, trigger);
	XScuGic_Enable(pGICInst, intID);
	return XST_SUCCESS;
}


// Status callback on the SPI
void SPIStatusHandler(const void *CallBackRef, u32 StatusEvent, u32 RemainingBytes)
{
	spis.Cnt = intSPICount;
	if(StatusEvent == XST_SPI_TRANSFER_DONE){
		intSPICount++;
		spis.Status = StatusEvent;
		spis.RemainingBytes = RemainingBytes;
		spis.Cnt = intSPICount;
	} else {
		spis.Status = 0xFFFF;
		spis.RemainingBytes = 0;
	}
}

// setting up IIC related interrupt system
int SetUpIICInterrupt(XScuGic *pGICInst, u16 DeviceID, void *pHandler, uint intID, u8 priority, u8 trigger)
{
	XIicPs *InstancePointer = getIICInstance(DeviceID);
	if (NULL == InstancePointer) {
		return XST_FAILURE;
	}

	XIicPs_SetStatusHandler(InstancePointer, (void *)InstancePointer, IICStatusHandler);

	// Connect a device driver handler that will be called when an interrupt for the device occurs,
	// the device driver handler performs the specific interrupt processing for the device
	int Status = XScuGic_Connect(pGICInst, intID, (Xil_ExceptionHandler)pHandler, (void *)InstancePointer);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XScuGic_SetPriorityTriggerType(pGICInst, intID, priority, trigger);
	XScuGic_Enable(pGICInst, intID);
	return XST_SUCCESS;
}


// Status callback on the IIC
void IICStatusHandler(void *CallBackRef, u32 StatusEvent)
{
	iics.Status = StatusEvent;
	iics.ActCnt++;
}

void OLEDComm(u32 maxIICInt, IICState* piics){
	// handle IIC interrupts
	if(piics->ActCnt != piics->LastCnt){
		if(piics->Status == XIICPS_EVENT_COMPLETE_SEND){
			printf("   IIC transfer finished: Bytes: %d; Cnt: %ld\n\r", piics->ByteCount, piics->ActCnt);
		} else {
			printf("   IIC transfer finished irregular. Status: %ld", piics->Status);
		}
		piics->Active = 0;
		piics->ByteCount = 0;
		piics->Status = 0;
		piics->LastCnt = piics->ActCnt;
	}
	if((piics->Active == 0) && (piics->ActCnt <= maxIICInt)){
		u8 idxCmd = piics->ActCnt * SZ_CMD;
		u8 count = OLEDCmdList[idxCmd++];
		if(count > 0){
			printf("Start IIC Tx Cmd<%ld>\n\r", piics->ActCnt);
			u8 cmdType = OLEDCmdList[idxCmd++];
			u8 idxBuf =  OLEDCmdList[idxCmd++];
			switch(cmdType){
			case OLED_WR:
				piics->Active = 1;
				IicWriteMaster(IIC_0, OLEDWriteBuffer+idxBuf, count, 0x3C);
				piics->ByteCount = count;
				break;
			case OLED_RD:
				piics->Active = 0;
				//IicReadMaster(IIC_0); no read allowed
				piics->ByteCount = count;
				break;
			default:
				printf("Error: unknown command type\n\r");
			}
		} else {
			printf("All commands done\n\r");
		}
	} else {
		printf("Irregular count\n\r");
	}
}
