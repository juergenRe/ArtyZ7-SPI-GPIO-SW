/*
 * IICControl.c
 *
 *  Routines for accessing the IIC port in test environment
 */
#include "xparameters.h"

#include "IICControl.h"


XIicPs IicCtrl[IIC_CNT];			/* Instance of the IIC Device */


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

	return Status;
}

u32 IicWriteMaster(u16 DeviceID, u8 *pTx, u32 ByteCount, u16 SlaveAddress){
	XIicPs *InstancePtr = getIICInstance(DeviceID);
	if(NULL == InstancePtr)
		return XST_FAILURE;

	XIicPs_MasterSend(InstancePtr, pTx, ByteCount, SlaveAddress);
	return XST_SUCCESS;
}

