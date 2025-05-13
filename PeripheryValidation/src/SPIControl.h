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

#define SPI_DESELECT_ALL    0x0F
#define SPI_SELECT_1        0x00
#define SPI_SELECT_2        0x01        // not connected
#define SPI_SELECT_3        0x02        // not connected


XSpiPs *getSPIInstance(void);
int SpiInitMaster(UINTPTR BaseAddress);
u32 SpiReadWrite(u8 *pTx, u8 *pRx, u32 ByteCount);
void SpiSetSlaveSelect(u8 cs);

#ifdef __cplusplus
}
#endif

#endif /* SRC_SPICONTROL_H_ */
