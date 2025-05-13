/*
 * GPIOControl.c
 *
 *  Routines for accessing the GPIO port in test environment
 */
#include "xparameters.h"

#include "interruptControl.h"
#include "xinterrupt_wrap.h"

#include <xiicps.h>



XScuGic InterruptController;
XScuGic_Config *GicConfig;


// setting up global interrupt system
int SetUpInterruptSystem(UINTPTR BaseAddress){
    // Initialize the interrupt controller driver so that it is ready to use.
	GicConfig = XScuGic_LookupConfig(BaseAddress);
	if (NULL == GicConfig) {
		return XST_FAILURE;
	}

	int Status = XScuGic_CfgInitialize(&InterruptController, GicConfig, GicConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//Perform a self-test to ensure that the hardware was built correctly
	Status = XScuGic_SelfTest(&InterruptController);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	// Setup the Interrupt System
	// Connect the interrupt controller interrupt handler to the hardware
	// interrupt handling logic in the ARM processor.
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(	XIL_EXCEPTION_ID_INT,
									(Xil_ExceptionHandler) XScuGic_InterruptHandler,
									&InterruptController);

	// Enable interrupts in the ARM
	Xil_ExceptionEnable();
	return XST_SUCCESS;
}


// setting up GPIO related interrupt system
int SetUpGPIOInterrupt(XGpio* GPIOInstance, void Handler(XGpio *), u8 priority){
	if (NULL == GPIOInstance) {
		return XST_FAILURE;
	}

    // Register a handler for GPIO Interrupts
    XGpio_Config *Gpio_Config = XGpio_LookupConfig(GPIOInstance->BaseAddress);
    int Status = XSetupInterruptSystem(
                        GPIOInstance, 
                        Handler, 
                        Gpio_Config->IntrId, 
                        Gpio_Config->IntrParent, 
                        priority);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Enable the interrupt for the device and set priority and mode
	GpioSwitchSetInt(1);
	//XScuGic_GetPriorityTriggerType(pGICInst, intID, &prio, &trig);

    XEnableIntrId(Gpio_Config->IntrId, Gpio_Config->IntrParent);

	return Status;
}

// setting up SPI related interrupt system
int SetUpSPIInterrupt(XSpiPs *SPIInstance, void Handler(XSpiPs *, u32, u32), u8 priority)
{
	if (NULL == SPIInstance) {
		return XST_FAILURE;
	}

    // Register a handler for GPIO Interrupts
    XSpiPs_Config *SpiPs_Config = XSpiPs_LookupConfig(SPIInstance->Config.BaseAddress);
    int Status = XSetupInterruptSystem(
                        SPIInstance, 
                        &XSpiPs_InterruptHandler, 
                        SpiPs_Config->IntrId, 
                        SpiPs_Config->IntrParent, 
                        priority);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

    SPIInstance->Config.IntrId = SpiPs_Config->IntrId;
    SPIInstance->Config.IntrParent = SpiPs_Config->IntrParent;
    
    XSpiPs_SetStatusHandler(SPIInstance, SPIInstance, (XSpiPs_StatusHandler) Handler);

	return XST_SUCCESS;
}

// setting up IIC related interrupt system
int SetUpIICInterrupt(XIicPs *IICInstance, void Handler(XIicPs *, u32), u8 priority)
{
	if (NULL == IICInstance) {
		return XST_FAILURE;
	}

	XIicPs_Config *IicPs_Config = XIicPs_LookupConfig(IICInstance->Config.BaseAddress);

    int Status = XSetupInterruptSystem(
                        IICInstance, 
                        XIicPs_MasterInterruptHandler, 
                        IicPs_Config->IntrId, 
                        IicPs_Config->IntrParent, 
                        priority);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	XIicPs_SetStatusHandler(IICInstance, IICInstance, (XIicPs_IntrHandler)Handler);

	return XST_SUCCESS;
}


