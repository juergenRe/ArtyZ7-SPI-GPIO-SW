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


XIicPs *getIicInstance(void);
int IicInit(UINTPTR BaseAddress);
u32 IicWriteMaster(u8 *pTx, u32 ByteCount, u16 SlaveAddress);


int IicIsSendDone(void);
int IicIsError(void);
int IicIsNewInt(void);
int IicIsActive(void);
void IicAckOperation(void);
u32 IicGetIntCount(void);
u32 IicGetActStatus(void);
void IICStatusHandler(XIicPs *IICInstance, u32 StatusEvent);

#ifdef __cplusplus
}
#endif

#endif /* SRC_IICCONTROL_H_ */

