/*
 * IICControl.c
 *
 *  Routines for accessing the IIC port in test environment
 */
#include "xparameters.h"

#include "IICControl.h"


XIicPs IicCtrl[IIC_CNT];			/* Instance of the IIC Device */

static IICState iics;				// actual iic status
//static IICState *piics = &iics;				// pointer to status word


// Status callback on the IIC (interrupt routine)
void IICStatusHandler(void *CallBackRef, u32 StatusEvent)
{
	iics.Status = StatusEvent;
	iics.ActCnt++;
	iics.Active -= 1;
	if(iics.Active > 0){
		printf("Deactivation Error. Cnt: %d", iics.Active);
	}
}

int IicIsSendDone(void){
	return (iics.Status == XIICPS_EVENT_COMPLETE_SEND);
}

int IicIsError(void){
	return ((iics.Status & (XIICPS_EVENT_TIME_OUT | XIICPS_EVENT_ERROR | XIICPS_EVENT_NACK | XIICPS_EVENT_TX_OVR)) > 0);
}

int IicIsOperationDone(void){
	return (iics.ActCnt != iics.LastCnt);
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

XIicPs *getIICInstance(u16 DeviceID){
	// check for valid device ID
	if (DeviceID < IIC_CNT)
		return (IicCtrl + DeviceID);
	return NULL;
}

int IicInit(u16 DeviceID){
	int Status;
	XIicPs_Config *IicConfig;
	XIicPs *InstancePtr = getIICInstance(DeviceID);

	/*
	 * Initialize the IIC device.
	 */
	IicConfig = XIicPs_LookupConfig(DeviceID);
	if ((NULL == IicConfig) || (NULL == InstancePtr))  {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(InstancePtr, IicConfig, IicConfig->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to check hardware build.
	 */
	Status = XIicPs_SelfTest(InstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	xil_printf("%s self test succeeded\r\n", __func__);

	Status = XIicPs_SetOptions(InstancePtr, XIICPS_7_BIT_ADDR_OPTION);
	if (Status != XST_SUCCESS) {
		xil_printf("%s XIicPs_SetOptions fail\n", __func__);
		return XST_FAILURE;
	}

	Status = XIicPs_SetSClk(InstancePtr, 100000);
	xil_printf("IIC 0 config finish\n");

	IicAckOperation();
	return Status;
}

u32 IicWriteMaster(u16 DeviceID, u8 *pTx, u32 ByteCount, u16 SlaveAddress){
	XIicPs *InstancePtr = getIICInstance(DeviceID);
	if(NULL == InstancePtr)
		return XST_FAILURE;

	if(iics.Active || (iics.ActCnt != iics.LastCnt)){
		printf("Not ready to write to IIC\n\r");
		return XST_DEVICE_BUSY;
	}
	iics.Active += 1;
	iics.Status = 0;
	XIicPs_MasterSend(InstancePtr, pTx, ByteCount, SlaveAddress);
	return XST_SUCCESS;
}

// setting up IIC related interrupt system
int SetUpIICInterrupt(XScuGic *pGICInst, u16 DeviceID, void pHandler(XIicPs *), uint intID, u8 priority, u8 trigger)
{
	XIicPs *InstancePointer = getIICInstance(DeviceID);
	if (NULL == InstancePointer) {
		return XST_FAILURE;
	}

	XIicPs_SetStatusHandler(InstancePointer, (void *)InstancePointer, IICStatusHandler);

	// Connect a device driver handler that will be called when an interrupt for the device occurs,
	// the device driver handler performs the specific interrupt processing for the device
	int Status = XScuGic_Connect(pGICInst, intID, (Xil_ExceptionHandler)pHandler, (void *)InstancePointer);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XScuGic_SetPriorityTriggerType(pGICInst, intID, priority, trigger);
	XScuGic_Enable(pGICInst, intID);
	return XST_SUCCESS;
}


