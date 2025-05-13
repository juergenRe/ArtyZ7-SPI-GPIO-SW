/*
 * GPIOControl.c
 *
 *  Routines for accessing the GPIO port in test environment
 */
#include "xparameters.h"
#include "xgpio.h"

#include "GPIOControl.h"

#define	XGPIO_0_BASEADDRESS	XPAR_XGPIO_0_BASEADDR
#define XGPIO_1_BASEADDRESS  XPAR_XGPIO_1_BASEADDR

XGpio axi_gpio; 					// driver array instances
XGpio ck_gpio;                      // ChipKit outputs


	/*
	 * Initialize the GPIO driver so that it's ready to use:
	 * - init driver with DeviceID
	 * - set data directions for both channels
	 * - set output to 0
	 */
int GpioInitDvc(void) {
    // set-up LEDs and switches
    XGpio_Config *GpioConfig_0 = XGpio_LookupConfig(XGPIO_0_BASEADDRESS);
    int Status = XGpio_Initialize(&axi_gpio, XGPIO_0_BASEADDRESS);
    if (Status != XST_SUCCESS)  {
        return XST_FAILURE;
	 }

	 // Set the direction for all switches to be inputs, for all LEDs to be outputs
	 XGpio_SetDataDirection(&axi_gpio, SW_CHANNEL, SW_CHANNEL_DIR);
	 XGpio_SetDataDirection(&axi_gpio, LED_CHANNEL, LED_CHANNEL_DIR);
	 XGpio_DiscreteWrite(&axi_gpio, LED_CHANNEL, 0);

     // set up chipkit as outputs
    XGpio_Config *GpioConfig_1 = XGpio_LookupConfig(XGPIO_1_BASEADDRESS);
    Status = XGpio_Initialize(&ck_gpio, XGPIO_1_BASEADDRESS);
    if (Status != XST_SUCCESS)  {
        return XST_FAILURE;
	 }

	 // Set the direction for all switches to be inputs, for all LEDs to be outputs
	 XGpio_SetDataDirection(&ck_gpio, CK_CHANNEL, CK_CHANNEL_DIR);
	 XGpio_DiscreteWrite(&ck_gpio, CK_CHANNEL, 0);

	 return XST_SUCCESS;
}

XGpio* getGPIOInstance(void) {
    return &axi_gpio;
}

XGpio* getCKGPIOInstance(void) {
    return &ck_gpio;
}

void GpioLEDOutput(u32 dataOut){
 	 XGpio_DiscreteWrite(&axi_gpio, LED_CHANNEL, (dataOut & LED_BITWIDTH));
}

u32 GpioSwitchInput(void){
	 return (XGpio_DiscreteRead(&axi_gpio, SW_CHANNEL) & SW_BITWIDTH);
}

void GpioSwitchSetInt(u8 enable){
	 if (enable != 0){
		 GpioSwitchClrInt(SW_BITWIDTH);
		 XGpio_InterruptGlobalEnable(&axi_gpio);
		 XGpio_InterruptEnable(&axi_gpio, SW_BITWIDTH);
	 } else {
		 XGpio_InterruptGlobalDisable(&axi_gpio);
		 XGpio_InterruptDisable(&axi_gpio, SW_BITWIDTH);
	 }
}

void GpioSwitchClrInt(u32 mask){
	 XGpio_InterruptClear(&axi_gpio, mask);
}

u32 GpioSwitchGetInt(void){
	 return XGpio_InterruptGetStatus(&axi_gpio);
}

//========================================================
//
// ChipKit output handling
//
//========================================================

void CkSetBit(u8 bitNr) {
    XGpio_DiscreteWrite(&ck_gpio, CK_CHANNEL, 
        ((XGpio_DiscreteRead(&ck_gpio, CK_CHANNEL) | (1U << bitNr)) & (u32) CK_BITWIDTH));
}

void CkClrBit(u8 bitNr) {
    volatile u32 actVal = XGpio_DiscreteRead(&ck_gpio, CK_CHANNEL);
    volatile u32 bm = ~(1U << bitNr) & (u32) CK_BITWIDTH;
    volatile u32 bm2 = ~((u32) CK_BITWIDTH);
    bm |= bm2;
    volatile u32 newVal = actVal & bm;
    XGpio_DiscreteWrite(&ck_gpio, CK_CHANNEL, newVal);
}

void CkToggleBit(u8 bitNr) {
    volatile u32 actVal = XGpio_DiscreteRead(&ck_gpio, CK_CHANNEL);
    volatile u32 bm = (1U << bitNr) & (u32) CK_BITWIDTH;
    volatile u32 newVal = actVal ^ bm;
    XGpio_DiscreteWrite(&ck_gpio, CK_CHANNEL, newVal);
}