/*
 * IICControl.c
 *
 *  Routines for accessing the IIC port in test environment
 */
#include "xparameters.h"

#include "IICControl.h"


XIicPs IicCtrl;			/* Instance of the IIC Device */

static IICState iics;				// actual iic status
//static IICState *piics = &iics;				// pointer to status word

XIicPs *getIicInstance(void) {
    return &IicCtrl;    
}

int IicInit(UINTPTR BaseAddress){
	int Status;
	XIicPs_Config *IicConfig;

	/*
	 * Initialize the IIC device.
	 */
	IicConfig = XIicPs_LookupConfig(BaseAddress);
	if (NULL == IicConfig) {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(&IicCtrl, IicConfig, IicConfig->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to check hardware build.
	 */
	Status = XIicPs_SelfTest(&IicCtrl);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	xil_printf("%s self test succeeded\r\n", __func__);

	Status = XIicPs_SetOptions(&IicCtrl, XIICPS_7_BIT_ADDR_OPTION);
	if (Status != XST_SUCCESS) {
		xil_printf("%s XIicPs_SetOptions fail\n", __func__);
		return XST_FAILURE;
	}

	Status = XIicPs_SetSClk(&IicCtrl, 100000);
	xil_printf("IIC 0 config finish\n");

	IicAckOperation();
	return Status;
}

int IicIsSendDone(void){
	return (iics.Status == XIICPS_EVENT_COMPLETE_SEND);
}

int IicIsError(void){
	return ((iics.Status & (XIICPS_EVENT_TIME_OUT | XIICPS_EVENT_ERROR | XIICPS_EVENT_NACK | XIICPS_EVENT_TX_OVR)) > 0);
}

int IicIsNewInt(void){
	return (iics.ActCnt != iics.LastCnt);
}

int IicIsActive(void) {
    return (iics.Active > 0);
}

void IicAckOperation(void){
	iics.LastCnt = iics.ActCnt;
	iics.Active = 0;
	iics.Status = 0;
}

u32 IicGetIntCount(void){
	return iics.ActCnt;
}

u32 IicGetActStatus(void){
	return iics.Status;
}

u32 IicWriteMaster(u8 *pTx, u32 ByteCount, u16 SlaveAddress){
	if(iics.Active){
		printf("Not ready to write to IIC\n\r");
		return XST_DEVICE_BUSY;
	}
	iics.Active += 1;
	iics.Status = 0;
	XIicPs_MasterSend(&IicCtrl, pTx, ByteCount, SlaveAddress);
	return XST_SUCCESS;
}


// Status callback on the IIC (interrupt routine)
void IICStatusHandler(XIicPs *IICInstance, u32 StatusEvent)
{   
	iics.Status = StatusEvent;
    if(iics.Status == XIICPS_EVENT_COMPLETE_SEND){
        iics.ActCnt++;
        iics.Active -= 1;
        if(iics.Active > 0){
            printf("Deactivation Error. Cnt: %d\n", iics.Active);
        }
        printf("IIC Completed\n\r");
    }
}


