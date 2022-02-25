/*
 * SPIControl.c
 *
 *  Routines for accessing the SPI port in test environment
 */
#include "xparameters.h"

#include "SPIControl.h"

#define SpiPs_RecvByte(BaseAddress) \
		(u8)XSpiPs_In32((BaseAddress) + XSPIPS_RXD_OFFSET)

#define SpiPs_SendByte(BaseAddress, Data) \
		XSpiPs_Out32((BaseAddress) + XSPIPS_TXD_OFFSET, (Data))

XSpiPs SpiCtrl[SPI_CNT]; 					// driver array instances

XSpiPs *getSPIInstance(u16 DeviceID){
	// check for valid device ID
	if (DeviceID < SPI_CNT)
		return (SpiCtrl + DeviceID);
	return NULL;
}

// configure SPI
// Manual SS
// Auto Start
// prescaler = 128
// CLK_POL = 0 (Clk is zero when no transmission runs)
// CLK_PH = 1 (no inactivation of SS during back to back transfers). Driving at rising edge, sampling at falling edge
int SpiInitMaster(u16 DeviceID){
	int Status;
	XSpiPs_Config *SpiConfig;
	XSpiPs *InstancePtr = getSPIInstance(DeviceID);

	/*
	 * Initialize the SPI device.
	 */
	SpiConfig = XSpiPs_LookupConfig(DeviceID);
	if ((NULL == SpiConfig) || (NULL == InstancePtr))  {
		return XST_FAILURE;
	}

	Status = XSpiPs_CfgInitialize(InstancePtr, SpiConfig, SpiConfig->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to check hardware build.
	 */
	Status = XSpiPs_SelfTest(InstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	xil_printf("%s self test succ\r\n", __func__);

	XSpiPs_Disable(InstancePtr);

	Status = XSpiPs_SetSlaveSelect(InstancePtr, 0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XSpiPs_SetOptions(InstancePtr, XSPIPS_MASTER_OPTION | XSPIPS_CLK_PHASE_1_OPTION | XSPIPS_FORCE_SSELECT_OPTION);
	if (Status != XST_SUCCESS) {
		xil_printf("%s XSpiPs_SetOptions fail\n", __func__);
		return XST_FAILURE;
	}
	/*
	 * Clear the slave selects when manual CS has been selected.
	 */
	if (XSpiPs_IsManualChipSelect(InstancePtr) == TRUE) {
		u32 ConfigReg = XSpiPs_ReadReg(InstancePtr->Config.BaseAddress, XSPIPS_CR_OFFSET);
		ConfigReg |= XSPIPS_CR_SSCTRL_MASK;
		XSpiPs_WriteReg(InstancePtr->Config.BaseAddress, XSPIPS_CR_OFFSET, ConfigReg);
	}


	Status = XSpiPs_SetClkPrescaler(InstancePtr, XSPIPS_CLK_PRESCALE_128);
	if (Status != XST_SUCCESS) {
		xil_printf("%s XSpiPs_SetClkPrescaler fail\n", __func__);
		return XST_FAILURE;
	}
	XSpiPs_Enable(InstancePtr);
	xil_printf("spi 0 config finish\n");

	return XST_SUCCESS;
}

u32 SpiReadWrite(u16 DeviceID, u8 *pTx, u8 *pRx, u32 ByteCount){
	XSpiPs *InstancePtr = getSPIInstance(DeviceID);
	if(NULL == InstancePtr)
		return XST_FAILURE;

	return XSpiPs_Transfer(InstancePtr, pTx, pRx, ByteCount);
}




