
#include "OLEDComm.h"

#include "sleep.h"

#include "../IICControl.h"

OLEDCommIIC::OLEDCommIIC(void){
	DeviceID = 0xFFFF;
	DvcAddress = 0;
}

int OLEDCommIIC::initialize (u16 d_id, u16 dvcAddress){
	DeviceID = d_id;
	DvcAddress = dvcAddress;
	DvcStatus = IicInit(DeviceID);
	Cnt = 0;
	return DvcStatus;
}

u32 OLEDCommIIC::sendData(u8 *data, u32 length){
	u32 res = IicWriteMaster(DeviceID, data, length, DvcAddress);
	if (res != XST_SUCCESS)
		return res;

	while(!IicIsOperationDone())
		sleep(0.01);

	Cnt = IicGetIntCount();
	DvcStatus = IicGetActStatus();
	IicAckOperation();
	if(IicIsSendDone())
		return XST_SUCCESS;
	else
		return DvcStatus;
}

void OLEDCommIIC::useInterrupts(XScuGic *pGICInst, uint intID, u8 priority, u8 trigger){
	SetUpIICInterrupt(pGICInst, DeviceID, XIicPs_MasterInterruptHandler, intID, priority, trigger);
}


