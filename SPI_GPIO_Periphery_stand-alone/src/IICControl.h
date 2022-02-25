/*
 * IICControl.h
 *
 *  Created on: 15.02.2022
 *      Author: rej
 */

#ifndef SRC_IICCONTROL_H_
#define SRC_IICCONTROL_H_

#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xiicps.h"

#define IIC_CNT 1			//number of internal IIC controllers used

XIicPs *getIICInstance(u16 DeviceID);
int IicInit(u16 DeviceID);
u32 IicWriteMaster(u16 DeviceID, u8 *pTx, u32 ByteCount, u16 SlaveAddress);
#endif /* SRC_IICCONTROL_H_ */

