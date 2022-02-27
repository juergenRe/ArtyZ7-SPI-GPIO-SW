/*
 * GPIODef.h
 *
 *  Created on: 03.05.2020
 *      Author: s91153
 */

#ifndef SRC_GPIOCONTROL_H_
#define SRC_GPIOCONTROL_H_

#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xgpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PORT_CNT 1					// number of hardware ports

#define SW_CHANNEL 1				// channel 1 read-only for switches and buttons
#define LED_CHANNEL 2				// channel 2 is write-only for LEDs

#define SW_CHANNEL_DIR 0xFFFFFFFF	// channel 1 read-only for switches and buttons
#define LED_CHANNEL_DIR	0x0			// channel 2 is write-only for LEDs

#define SW_BITWIDTH	0x0000000F		// This is the width of the GPIO channel for the switch inputs
#define LED_BITWIDTH 0x0000000F		// width of LEDs

//extern XGpio axi_gpio[];

XGpio *getGPIOInstance(u16 DeviceID);
int GpioInit(u16 DeviceID);
int GpioInitDvc(u16 DeviceID);	    //init a gpio device by setting direction and reset output
void GpioLEDOutput(u32 dataOut);	// write LEDs
u32 GpioSwitchInput(void);			// read button port
void GpioSwitchSetInt(u8 enable);	// enable/disable reception of interrupts
void GpioSwitchClrInt(u32 mask);	// clears a received interrupt and rearms the device
u32 GpioSwitchGetInt(void);			// get actual interrupts set

//void GpioDriverHandler(void *CallBackRef);

#ifdef __cplusplus
}
#endif

#endif /* SRC_GPIOCONTROL_H_ */
