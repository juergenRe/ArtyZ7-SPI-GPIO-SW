#ifndef _OLEDComm_H_
#define _OLEDComm_H_

#include <stdio.h>
#include "xscugic.h"

#define OLED_WR_CMD 0x80			//Co=1, D/C#=0
#define OLED_RD_STATUS 0x80			//Co=1, D/C#=0

#define NONE			ulong(0x0)
#define IIC_INTERFACE 	0x01
#define SPI_INTERFACE 	0x02

// abstract interface definition
class OLEDComm {
	public:
		virtual int initialize(u16 d_id, u16 slaveAddress) = 0;
		virtual u32 sendData(u8 *data, u32 length) = 0;
		virtual void useInterrupts(XScuGic *pGICInst, uint intID, u8 priority, u8 trigger) = 0;
		virtual u32 sendCommand(u8 cmd) = 0;
	protected:
		u32 Cnt;
		u32 DvcStatus;
		u16 DeviceID;
		u16 DvcAddress;
		u8 Active;
};

// specific implementation: IIC
class OLEDCommIIC: public OLEDComm {
public:
	OLEDCommIIC(void);
	int initialize(u16 d_id, u16 slaveAddress);
	void useInterrupts(XScuGic *pGICInst, uint intID, u8 priority, u8 trigger);
	u32 sendData(u8 *data, u32 length);
	u32 sendCommand(u8 cmd);
};

#endif /* _OLEDComm_H_ */


