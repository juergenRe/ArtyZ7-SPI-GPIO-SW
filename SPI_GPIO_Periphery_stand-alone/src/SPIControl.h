/*
 * SPIControl.h
 *
 *  Created on: 11.02.2022
 *      Author: rej
 */

#ifndef SRC_SPICONTROL_H_
#define SRC_SPICONTROL_H_

#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xspips.h"

#ifdef __cplusplus
extern "C" {
#endif

// main clock is 166MHz
#define SPI_CNT 1			//number of internal SPI controllers used
//extern XSpiPs *SpiCtrl;

XSpiPs *getSPIInstance(u16 DeviceID);
int SpiInitMaster(u16 DeviceID);
u32 SpiReadWrite(u16 DeviceID, u8 *pTx, u8 *pRx, u32 ByteCount);

#ifdef __cplusplus
}
#endif

#endif /* SRC_SPICONTROL_H_ */
