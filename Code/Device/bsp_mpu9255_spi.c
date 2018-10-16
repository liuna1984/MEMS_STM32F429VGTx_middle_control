/**
  ******************************************************************************
  * @file    bsp_mpu9255_spi.c
  * @author  
  * @version V1.0
  * @date    27-January-2015
  * @brief   This file includes the MPU9250 driver functions
  
  ******************************************************************************
  */
#include "bsp_mpu9255.h"

IMU_Type IMU_Data;
extern SPI_HandleTypeDef hspi1;

 uint8_t spi1_rxbuf[100];
 uint16_t spi1_rxlen;
 uint8_t spi1_Rx_Data;
/**
  * @brief  Initializes MPU9255
  * @param  None
  * @retval None
  */
#ifdef BUS_SPI
void mpu9255_initi_spi(void)
{
	uint8_t spi1_txbuf[20];
//	int32_t adc_P,adc_T;
//	double var1, var2, temperature, pressure;
//	int16_t InBuffer = 0; 
	
	// Test MPU9255
	CS9255_LOW;
	// Reset Device
  spi1_txbuf[0] = PWR_MGMT_1;
	spi1_txbuf[1] = 0x80;
	HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 2, 100);
	CS9255_HIGH;	
	HAL_Delay(10);
	
	CS9255_LOW;
	// Read Device ID
  spi1_txbuf[0] = WHO_AM_I | MPU9255_READ;
//	HAL_SPI_TransmitReceive(&hspi1, &spi1_txbuf[0], &spi1_rxbuf[0], 1, 100);
	HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 1, 100);
	HAL_SPI_Receive(&hspi1, &spi1_rxbuf[0], 1, 100);
	CS9255_HIGH;
	HAL_Delay(10);
	
	if(spi1_rxbuf[0] == WHO_AM_I_VAL)
	{
    
		CS9255_LOW;
		// Clock Source
		spi1_txbuf[0] = PWR_MGMT_1;
		spi1_txbuf[0] = 0x01;
		HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 2, 100);
		CS9255_HIGH;
		HAL_Delay(10);
	
		CS9255_LOW;
		//  
		spi1_txbuf[0] = SMPLRT_DIV;
		spi1_txbuf[1] = 0x07;
		HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 2, 100);
		CS9255_HIGH;
		HAL_Delay(10);

//	  CS9255_LOW;
//		// Set I2C_MST_EN 
//		spi1_txbuf[0] = USER_CTRL;
//		spi1_txbuf[1] = 0x20;
//	  HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 2, 100);
//		CS9255_HIGH;
//		HAL_Delay(10);
	
//	  CS9255_LOW;
//		// Enable Acc & Gyro
//		spi1_txbuf[0] = PWR_MGMT_2;
//		spi1_txbuf[1] = 0x00;
//	  HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 2, 100);
//		CS9255_HIGH;
//		HAL_Delay(10);
	
		CS9255_LOW;
		//  
		spi1_txbuf[0] = CONFIG;
		spi1_txbuf[1] = 0x05;
		HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 2, 100);
		CS9255_HIGH;
		HAL_Delay(10);
	
		CS9255_LOW;
		//  Gyro Full Scale Select [4:3]: 00 = 250dps; 01= +500 dps; 10 = +1000 dps; 11 = +2000 dps;
		spi1_txbuf[0] = GYRO_CONFIG;
		spi1_txbuf[1] = 0x00;
		HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 2, 100);
		CS9255_HIGH;
		HAL_Delay(10);
	
		CS9255_LOW;
		// Accel Full Scale Select[4:3]: ±2g (00), ±4g (01), ±8g (10), ±16g (11)
		spi1_txbuf[0] = ACCEL_CONFIG;
		spi1_txbuf[1] = 0x18;
		HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 2, 100);
		CS9255_HIGH;
		HAL_Delay(10);
	
		CS9255_LOW;
		// Set Acc Data Rates
		spi1_txbuf[0] = ACCEL_CONFIG_2;
		spi1_txbuf[1] = 0x04;
		HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 2, 100);
		CS9255_HIGH;
		HAL_Delay(10);
	
//	  CS9255_LOW;
//		// turn on Bypass Mode 
//		spi1_txbuf[0] = INT_PIN_CFG;
//		spi1_txbuf[1] = 0x02;
//	  HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 2, 100);
//		CS9255_HIGH;
//		HAL_Delay(10);
		
	}
	
}

/**
  * @brief  mpu9255 data read 
  * @param  None
  * @retval None
  */
void read_mpu9255_spi()
{
	uint8_t spi1_txbuf[20];
	int16_t InBuffer = 0; 

	CS9255_LOW;
	
  // Read Data for accel	
	spi1_txbuf[0] = ACCEL_XOUT_H | MPU9255_READ;
//	spi1_txbuf[1] = ACCEL_XOUT_L | MPU9255_READ;
//	spi1_txbuf[2] = ACCEL_YOUT_H | MPU9255_READ;
//	spi1_txbuf[3] = ACCEL_YOUT_L | MPU9255_READ;
//	spi1_txbuf[4] = ACCEL_ZOUT_H | MPU9255_READ;
//	spi1_txbuf[5] = ACCEL_ZOUT_L | MPU9255_READ;
//	spi1_txbuf[6] = TEMP_OUT_H | MPU9255_READ;
//	spi1_txbuf[7] = TEMP_OUT_L | MPU9255_READ;
//	spi1_txbuf[8] = GYRO_XOUT_H | MPU9255_READ;
//	spi1_txbuf[9] = GYRO_XOUT_L | MPU9255_READ;
//	spi1_txbuf[10] = GYRO_YOUT_H | MPU9255_READ;
//	spi1_txbuf[11] = GYRO_YOUT_L | MPU9255_READ;
//	spi1_txbuf[12] = GYRO_ZOUT_H | MPU9255_READ;
//	spi1_txbuf[13] = GYRO_ZOUT_L | MPU9255_READ;
//	HAL_SPI_TransmitReceive(&hspi1, &spi1_txbuf[0], &IMU_Data.Buf.Data[0], 14, 100);
	HAL_SPI_Transmit(&hspi1, &spi1_txbuf[0], 1, 100);
	HAL_SPI_Receive(&hspi1, &IMU_Data.Buf.Data[0], 16, 100);
	
	//  
	IMU_Data.Data.ACCEL_X = (IMU_Data.Data.ACCEL_X>>8&0xFF)|(IMU_Data.Data.ACCEL_X&0xFF)<<8;
	IMU_Data.Data.ACCEL_Y = (IMU_Data.Data.ACCEL_Y>>8&0xFF)|(IMU_Data.Data.ACCEL_Y&0xFF)<<8;
	IMU_Data.Data.ACCEL_Z = (IMU_Data.Data.ACCEL_Z>>8&0xFF)|(IMU_Data.Data.ACCEL_Z&0xFF)<<8;	
	//
	IMU_Data.Data.GYRO_X = (IMU_Data.Data.GYRO_X>>8&0xFF)|(IMU_Data.Data.GYRO_X&0xFF)<<8;
	IMU_Data.Data.GYRO_Y = (IMU_Data.Data.GYRO_Y>>8&0xFF)|(IMU_Data.Data.GYRO_Y&0xFF)<<8;
	IMU_Data.Data.GYRO_Z = (IMU_Data.Data.GYRO_Z>>8&0xFF)|(IMU_Data.Data.GYRO_Z&0xFF)<<8;
	//  
	InBuffer = (IMU_Data.Buf.Data[6]<<8)|IMU_Data.Buf.Data[7];
	IMU_Data.Data.TEMP = (int16_t)((((double)(InBuffer-0)/333.87)+21.0)* 100) ;
	
	CS9255_HIGH;
	
}

#endif


