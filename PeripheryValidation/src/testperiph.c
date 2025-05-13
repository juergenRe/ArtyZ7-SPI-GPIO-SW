#include <sleep.h>
#include <stdio.h>
#include <xinterrupt_wrap.h>
#include <xstatus.h>

#include "lcd/oled_ssd1306.h"
#include "nano_gfx_types.h"
#include "platform.h"
#include "ssd1306_1bit.h"
#include "ssd1306_generic.h"
#include "xparameters.h"
#include "xil_printf.h"

#include "interruptControl.h"
#include "GPIOControl.h"
#include "SPIControl.h"
#include "IICControl.h"

#include "ssd1306.h"

#define XSCUGIC_DIST_BASEADDR XPAR_XSCUGIC_0_BASEADDR
#define XSPI_BASEADDR       XPAR_SPI0_BASEADDR
#define XIIC_BASEADDR       XPAR_I2C0_BASEADDR

#define MSK_LED 0x0F

#define SW_DEBOUNCE  2000

#define OLED_ADDR 0x3C				//IIC address of OLED device

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

void GPIOInterruptHandler(XGpio *GPIOInstance);
void SPIStatusHandler(XSpiPs* SPIInstance, u32 StatusEvent, u32 RemainingBytes);

static void AssertPrint(const char8 *FilenamePtr, s32 LineNumber){
	xil_printf("ASSERT: File Name: %s ", FilenamePtr);
	xil_printf("Line Number: %d\r\n",LineNumber);
}

// --- GPIO related definitions
static volatile ButtonState sw;		// actual button status
static volatile u32 intGPIOCount;	// count called interrupts

// --- SPI related definitions
static volatile u32 intSPICount;	// count called SPI interrupts
static volatile SPIState spis;		// actual spi status

#define SPI_SCREEN 0				// number of slave to be addressed for screen display
#define SPI_BUF_SIZE 128
unsigned char SPIWriteBuffer[SPI_BUF_SIZE] = {'a', '1', 'z'};
unsigned char SPIReadBuffer[SPI_BUF_SIZE+1];

// --- IIC related definitions
#define IIC_BUF_SIZE 128
#define LCD_ADDR 0x3C
u8 IICWriteBuffer[IIC_BUF_SIZE] = {
    0x80, 0x8D, 0x80, 0x14, 0x80, 0xAF, 0x80, 0xA4, 0x80, 0x81, 0x80, 0x7F,
    0x80, 0x40, 0x80, 0x20, 0x80, 0x00, 0x80, 0x21, 0x80, 0x00, 0x80, 0x7F,
    0x80, 0x22, 0x80, 0x00, 0x80, 0x07,
    0x40, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
    0x80, 0xA7, 0x80, 0x22, 0x80, 0x02, 0x80, 0x07,
    0x40, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F
    };
u8 IICReadBuffer[IIC_BUF_SIZE+1];
u8 *pChar;
u8 msgDef[] = {0, 30, 47, 55, 72, 0xff};
u8 *actMsg = msgDef;
static u8 procState = 0;

void processInput(u8 sw, u8 *procState, SAppMenu *appMenu);

int main ()
{

    //static XScuTimer scutimer;

	// Setup an assert call back to get some info if we assert.
	Xil_AssertSetCallback(AssertPrint);

    init_platform();

    print("\n\n---Entering main---\n\r");

    // Init GPIO
    int status = GpioInitDvc();
    
    printf("--- GPIO initialized. Status: %d ---\n\r", status);

    status = SpiInitMaster(XSPI_BASEADDR);
    printf("--- SPI initialized. Status: %d ---\n\r", status);

    status = IicInit(XIIC_BASEADDR);
    printf("--- IIC initialized. Status: %d ---\n\r", status);

    //set up interrupts
    int step = 0;
    status = SetUpInterruptSystem(XSCUGIC_DIST_BASEADDR);
    if (status == XST_SUCCESS) {
        step += 1;
        status = SetUpGPIOInterrupt(getGPIOInstance(), GPIOInterruptHandler, XINTERRUPT_DEFAULT_PRIORITY);
    }        
    if (status == XST_SUCCESS) {
        step += 1;
        status = SetUpSPIInterrupt(getSPIInstance(), SPIStatusHandler, XINTERRUPT_DEFAULT_PRIORITY);
    }    
    if (status == XST_SUCCESS) {
        step += 1;
        status = SetUpIICInterrupt(getIicInstance(), IICStatusHandler, XINTERRUPT_DEFAULT_PRIORITY);
    }

    printf("--- Set-up interrupt system done. Last step: %d Status: %d ---\n\r", step, status);
    if (status != XST_SUCCESS) {
        print("--- Exit due to error ---\n\r");
        return 1;
    }

    printf("--- Set-up LCD\n\r");
    ssd1306_128x64_i2c_init();
    ssd1306_clearScreen();
    //ssd1306_setFixedFont(ssd1306xled_font8x16);
    ssd1306_setFixedFont(ssd1306xled_font6x8);

    // wait till LCD is ready, it takes a bit
    while(ssd1306_isActive())
        usleep(100);

    ssd1306_printFixed(0, 0, "!\"#$%&'()*+,-./0123456789:;<=>?\0", STYLE_NORMAL);
    //ssd1306_printFixedN(4, 0, "-Arty-Z20-\0", STYLE_BOLD, 1);    
    //ssd1306_print("-Arty-Z20-\0");

    // set up menu
    SAppMenu appMenu;
    const char entry1[] = "Invert";
    const char entry2[] = "Flip";

    const char* items[] = {entry1, entry2};    
    int count = 2;

    ssd1306_clearScreen();
    ssd1306_createMenu(&appMenu, items, count);
    ssd1306_showMenu(&appMenu);    

    print("--- Entering main loop ---\n\r");
    int running = 1;
    u32 runCnt = 0;
	u32 lastGPIOCount = intGPIOCount;
	u32 lastSPICount = intSPICount;
    pChar = IICWriteBuffer;

    while (running != 0){
        runCnt++;
    	if (intGPIOCount != lastGPIOCount){
    		lastGPIOCount = intGPIOCount;
    		printf("<%u> ---Catches interrupt. Count: %u switch: %u\n\r", (unsigned int) runCnt, (unsigned int)lastGPIOCount, (unsigned int)sw.actIn);
            if (sw.lastIn != sw.actIn){
                sw.lastIn = sw.actIn;
                processInput(sw.actIn, &procState, &appMenu);
                GpioLEDOutput(sw.actIn);
                printf("<%u> GPIO Int Count: %d\n\r", (unsigned int)runCnt, (unsigned int)intGPIOCount);
                CkClrBit(0);
            }
        }
    	if (intGPIOCount > 40) break;  

    	// handle SPI transfer
    	if(intSPICount != lastSPICount){
    		if (spis.Status == XST_SPI_TRANSFER_DONE) {
                SpiSetSlaveSelect(SPI_DESELECT_ALL);
    			SPIReadBuffer[spis.ByteCount] = 0x0;
    			printf("<%u>   SPI Input: %s ; rem. Bytes: %d; Cnt: %u\n\r", (unsigned int)runCnt, SPIReadBuffer, spis.RemainingBytes, (unsigned int)spis.Cnt);
    		}
    		printf("<%u> Clearing internal read buffer.\n\r", (unsigned int)runCnt);
    		for(int i = 0;i<spis.ByteCount;i++)
    			SPIReadBuffer[i] = 0x00;
    		spis.ByteCount = 0;
    		spis.RemainingBytes = 0;
    		spis.Status = 0;
    		spis.Active = 0;
    		lastSPICount = intSPICount;
    	}
    	if((spis.Active == 0) && (intSPICount == 0)){
    		printf("<%u> Start SPI Tx %u\n\r", (unsigned int)runCnt, (unsigned int)intSPICount);
    		SpiReadWrite(SPIWriteBuffer, SPIReadBuffer, 3);
    		spis.Active = 1;
    		spis.ByteCount = 3;
    	}
    }

    print("--- Exiting main ---");
    return 0;
}

// sw0: toggle screen inverse/normal
// sw1: flip horizontal
#define BTN_UP ((u8)1)
#define BTN_DOWN ((u8)2)
#define BTN_X ((u8)4)
#define BTN_SEL ((u8)8)

#define INVERT 1
#define FLIP 2

void processInput(u8 sw, u8 *procState, SAppMenu *appMenu) {
    if (sw & BTN_UP){
        ssd1306_menuUp(appMenu);
        ssd1306_updateMenu(appMenu);
    } else if (sw & BTN_DOWN) {
        ssd1306_menuDown(appMenu);
        ssd1306_updateMenu(appMenu);
    } else if (sw & BTN_SEL) {
        int sel = ssd1306_menuSelection(appMenu);
        switch (sel) {
            case 0: 
                if((*procState & INVERT) != 0) {
                    ssd1306_normalMode();
                    *procState = *procState & ~INVERT;
                }
                else {
                    ssd1306_invertMode();
                    *procState = *procState | INVERT;
                }
                break;
            case 1:
                if((*procState & FLIP) != 0) {
                    ssd1306_flipHorizontal(0);
                    ssd1306_flipVertical(0);
                    *procState = *procState & ~FLIP;
                }
                else {
                    ssd1306_flipHorizontal(1);
                    ssd1306_flipVertical(1);
                    *procState = *procState | FLIP;
                }
                break;
            default:
                break;
        }
    }
}

void GPIOInterruptHandler(XGpio *GPIOInstance)
{
	u32 intPending = GpioSwitchGetInt();
    CkSetBit(0);
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

// Status callback on the SPI
void SPIStatusHandler(XSpiPs* SPIInstance, u32 StatusEvent, u32 RemainingBytes)
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
