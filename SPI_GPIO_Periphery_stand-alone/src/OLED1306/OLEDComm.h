#ifndef _OLEDComm_H_
#define _OLEDComm_H_

#include <stdio.h>
#include "xscugic.h"

class OLEDComm {
	public:
		virtual int initialize(u16 d_id, u16 slaveAddress) = 0;
		virtual u32 sendData(u8 *data, u32 length) = 0;
		virtual void useInterrupts(XScuGic *pGICInst, uint intID, u8 priority, u8 trigger) = 0;
	protected:
		u32 Cnt;
		u32 DvcStatus;
		u16 DeviceID;
		u16 DvcAddress;
		u8 Active;
};

class OLEDCommIIC: public OLEDComm {
public:
	OLEDCommIIC(void);
	int initialize(u16 d_id, u16 slaveAddress);
	void useInterrupts(XScuGic *pGICInst, uint intID, u8 priority, u8 trigger);
	u32 sendData(u8 *data, u32 length);
};

#endif /* _OLEDComm_H_ */
