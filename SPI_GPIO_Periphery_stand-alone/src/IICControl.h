/*
 * IICControl.h
 *
 *  Created on: 15.02.2022
 *      Author: rej
 */

#ifndef SRC_IICCONTROL_H_
#define SRC_IICCONTROL_H_

#include <stdio.h>

#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xiicps.h"
#include "xscugic.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IIC_CNT 1			//number of internal IIC controllers used

typedef struct {
	u32 ActCnt;
	u32 LastCnt;
	u32 Status;
	u8 Active;
	u8 IsSend;
} IICState;


int SetUpIICInterrupt(XScuGic *pGICInst, u16 DeviceID, void pHandler(XIicPs *), uint intID, u8 priority, u8 trigger);
void IICStatusHandler(void *CallBackRef, u32 StatusEvent);

XIicPs *getIICInstance(u16 DeviceID);
int IicInit(u16 DeviceID);
u32 IicWriteMaster(u16 DeviceID, u8 *pTx, u32 ByteCount, u16 SlaveAddress);


int IicIsSendDone(void);
int IicIsError(void);
int IicIsOperationDone(void);
void IicAckOperation(void);
u32 IicGetIntCount(void);
u32 IicGetActStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* SRC_IICCONTROL_H_ */

