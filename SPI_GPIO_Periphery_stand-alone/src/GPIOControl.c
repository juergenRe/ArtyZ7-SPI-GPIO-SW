/*
 * GPIOControl.c
 *
 *  Routines for accessing the GPIO port in test environment
 */
#include "xparameters.h"
#include "xgpio.h"

#include "GPIOControl.h"

XGpio axi_gpio[1]; 					// driver array instances


XGpio *getGPIOInstance(u16 DeviceID){
	// check for valid device ID
	if (DeviceID < PORT_CNT)
		return (axi_gpio + DeviceID);
	return NULL;
}

// init both devices together
int GpioInit(u16 DeviceID){
	int status = GpioInitDvc(DeviceID);
	return status;
}

	/*
	 * Initialize the GPIO driver so that it's ready to use:
	 * - init driver with DeviceID
	 * - set data directions for both channels
	 * - set output to 0
	 */
int GpioInitDvc(u16 DeviceID ) {
	 int Status = XGpio_Initialize(axi_gpio, DeviceID);
	 if (Status != XST_SUCCESS)  {
		  return XST_FAILURE;
	 }

	 // Set the direction for all switches to be inputs, for all LEDs to be outputs
	 XGpio_SetDataDirection(axi_gpio, SW_CHANNEL, SW_CHANNEL_DIR);
	 XGpio_SetDataDirection(axi_gpio, LED_CHANNEL, LED_CHANNEL_DIR);
	 XGpio_DiscreteWrite(axi_gpio, LED_CHANNEL, 0);
	 return XST_SUCCESS;
}

void GpioLEDOutput(u32 dataOut){
 	 XGpio_DiscreteWrite(axi_gpio, LED_CHANNEL, (dataOut & LED_BITWIDTH));
}

u32 GpioSwitchInput(void){
	 return (XGpio_DiscreteRead(axi_gpio, SW_CHANNEL) & SW_BITWIDTH);
}

void GpioSwitchSetInt(u8 enable){
	 if (enable != 0){
		 GpioSwitchClrInt(SW_BITWIDTH);
		 XGpio_InterruptGlobalEnable(axi_gpio);
		 XGpio_InterruptEnable(axi_gpio, SW_BITWIDTH);
	 } else {
		 XGpio_InterruptGlobalDisable(axi_gpio);
		 XGpio_InterruptDisable(axi_gpio, SW_BITWIDTH);
	 }
}

void GpioSwitchClrInt(u32 mask){
	 XGpio_InterruptClear(axi_gpio, mask);
}

u32 GpioSwitchGetInt(void){
	 return XGpio_InterruptGetStatus(axi_gpio);
}
