#ifndef SRC_INTERRUPTCONTROL_H			/* prevent circular inclusions */
#define SRC_INTERRUPTCONTROL_H			/**< by using protection macros */

#include "xil_assert.h"
#include "xil_types.h"
#include "xstatus.h"
#include "xscugic.h"
#include <sys/types.h>
#include <xgpio.h>
#include "xspips.h"
#include "xiicps.h"

#include "GPIOControl.h"

#ifdef __cplusplus
extern "C" {
#endif

int SetUpInterruptSystem(UINTPTR BaseAddress);
int SetUpGPIOInterrupt(XGpio* GPIOInstance, void Handler(XGpio *), u8 priority);
int SetUpSPIInterrupt(XSpiPs *SPIInstance, void Handler(XSpiPs *, u32, u32), u8 priority);
int SetUpIICInterrupt(XIicPs *IICInstance, void Handler(XIicPs *, u32), u8 priority);

#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */
