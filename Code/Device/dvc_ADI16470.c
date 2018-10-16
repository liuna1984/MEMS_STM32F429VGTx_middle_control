/**
 * @file dvc_ADI16470.c
 * @author Qiaoyy@xlnav.com
 * @version V1.0
 * @date 2018-04-27
 * @brief  ADI 16470 驱动程序
 * Copyright (c) 2018 XiLang Co. Ltd.
 * All rights reserved.
 */

/* Includes ------------------------------------------------------------------*/
#include "dvc_ADI16470.h"
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief
 * @param
 * @return
*/


void mydelay(uint32_t timecount);
int ADI_16470_ReadDeviceNumber(ADI_16470_HandleTypeDef* pImu)
{
	unsigned short buffer[2] = { ADI_16470_PROD_ID,0 };

	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(pImu->phspi, (uint8_t*)&buffer[0], 1, 0xFFFF);
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_SET);
	mydelay(50);
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(pImu->phspi, (uint8_t*)&buffer[1], 1, 0xFFFF);
	pImu->nProductID = buffer[1];
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_SET);
	
	HAL_Delay(10);
	
	// ADI 写入寄存器方法 一个Word 高字节为 地址|0x80(最高位变为1) 低字节为要写入的值
	// 以下为将0x04写入0x5c地址 将0x00写入0x5D地址 
	buffer[0] = 0xDC04;
	buffer[1] = 0xDD00;
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(pImu->phspi, (uint8_t*)&buffer[0], 1, 0xFFFF);
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_SET);
	mydelay(50);
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(pImu->phspi, (uint8_t*)&buffer[1], 1, 0xFFFF);
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_SET);
	
	HAL_Delay(10);
	buffer[0] = 0x5C5D;
	buffer[1] = 0x0000;
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(pImu->phspi, (uint8_t*)&buffer[0], 1, 0xFFFF);
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_SET);
	mydelay(50);
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(pImu->phspi, (uint8_t*)&buffer[1], 1, 0xFFFF);
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_SET);
	
	return 1;
}

unsigned short outbuffer[10];
int ADI_16470_ReadburstImu(ADI_16470_HandleTypeDef* pImu)
{
	unsigned short cmd;
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_RESET);
	cmd = 0x6800;
	HAL_SPI_Transmit(pImu->phspi, (uint8_t*)&cmd, 1, 100);
	HAL_SPI_Receive(pImu->phspi, (uint8_t*)&outbuffer[0], 10, 100);
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_SET);
	return 1;
}

int ADI_16470_ReadImu(ADI_16470_HandleTypeDef* pImu)
{
	unsigned short cmd[14];
	int i;
	cmd[0] = 0x0400;
	cmd[1] = 0x0600;

	cmd[2] = 0x0800;
	cmd[3] = 0x0A00;

	cmd[4] = 0x0C00;
	cmd[5] = 0x0E00;

	cmd[6] = 0x1000;
	cmd[7] = 0x1200;

	cmd[8] = 0x1400;
	cmd[9] = 0x1600;

	cmd[10] = 0x1800;
	cmd[11] = 0x1A00;

	cmd[12] = 0x1c00;
	cmd[13] = 0x7200;
	for (i = 0; i < 14; i++)
	{
		HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(pImu->phspi, (uint8_t*)&cmd[i], (uint8_t*)&pImu->usBuffer[i], 1, 100);
		HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_SET);
		mydelay(50);
	}
	ADI_16470_ParseImu(pImu);
	return 1;
}
void mydelay(uint32_t timecount)
{
	int i, j;
	for (i = 0; i < timecount; i++)
	{
		for (j = 0; j < timecount; j++)
		{
			;
		}
	}
}

int ADI_16470_ParseImu(ADI_16470_HandleTypeDef* pImu)
{
	pImu->fGyrX = (float)((short)pImu->usBuffer[2]) * 0.1f + (float)((short)pImu->usBuffer[1]) / 65536.0f * 0.1f;
	pImu->fGyrY = (float)((short)pImu->usBuffer[4]) * 0.1f + (float)((short)pImu->usBuffer[3]) / 65536.0f * 0.1f;
	pImu->fGyrZ = (float)((short)pImu->usBuffer[6]) * 0.1f + (float)((short)pImu->usBuffer[5]) / 65536.0f * 0.1f;
	pImu->fAccX = (float)((short)pImu->usBuffer[8]) * 0.00125f + (float)((short)pImu->usBuffer[7]) / 65536.0f * 0.00125f;
	pImu->fAccY = (float)((short)pImu->usBuffer[10]) * 0.00125f + (float)((short)pImu->usBuffer[9]) / 65536.0f * 0.00125f;
	pImu->fAccZ = (float)((short)pImu->usBuffer[12]) * 0.00125f + (float)((short)pImu->usBuffer[11]) / 65536.0f * 0.00125f;
	pImu->fTempretrue = (float)(pImu->usBuffer[13]) * 0.1f;
	return 1;
}

int ADI_16470_ReadStatus(ADI_16470_HandleTypeDef* pImu)
{
	unsigned short buffer[2] = { ADI_16470_DIAG_STAT,0 };
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(pImu->phspi, (uint8_t*)&buffer[0], 1, 0xFFFF);
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_SET);
	mydelay(50);
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(pImu->phspi, (uint8_t*)&buffer[1], 1, 0xFFFF);
	pImu->Status = buffer[1];
	HAL_GPIO_WritePin(pImu->NSS_GPIOx, pImu->NSS_GPIO_Pin, GPIO_PIN_SET);
	return 1;
}

/************************ (C) COPYRIGHT XiLang CO. Ltd. ****** END OF FILE ****/
