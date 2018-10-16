/**
  ******************************************************************************
  * @file    MPU9255.h
  * @author  
  * @version V1.0
  * @date    27-January-2015
  * @brief   Header file for MPU9250.c module.
  ******************************************************************************
  */


#ifndef __BSP_MPU9255_H
#define __BSP_MPU9255_H

#include "stm32f4xx_hal.h"
#include "main.h"
#define BUS_SPI
#define MPU9255
#ifdef MPU9255
// define MPU9255 register address
//****************************************
#define	SMPLRT_DIV		  0x19	    // Sample Rate Divider. Typical values:0x07(125Hz) 1KHz internal sample rate
#define	CONFIG				  0x1A	    // Low Pass Filter.Typical values:0x06(5Hz)
#define	GYRO_CONFIG		  0x1B	    // Gyro Full Scale Select. Typical values:0x10(1000dps)
#define	ACCEL_CONFIG	  0x1C	    // Accel Full Scale Select. Typical values:0x01(2g)
#define	ACCEL_CONFIG_2	0x1D	    // Accelerometer Configuration 2
#define INT_PIN_CFG     0x37      // INT Pin / Bypass Enable Configuration
#define INT_ENABLE      0x38      // Interrupt Enable
#define I2C_MST_CTRL    0x4D      // I2C Master Control

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08

#define USER_CTRL       0x6A      // User Control
#define	PWR_MGMT_1	  	0x6B			// Power Management 1. Typical values:0x00(run mode)
#define	PWR_MGMT_2	  	0x6C			// Power Management 2
#define	WHO_AM_I		  	0x75			// identity of the device

#define	GYRO_ADDRESS   0xD0	  //Gyro and Accel device address
#define MAG_ADDRESS    0x18   //compass device address
#define ACCEL_ADDRESS  0xD0 

#define ADDRESS_AD0_LOW     0xD0 //address pin low (GND), default for InvenSense evaluation board
#define ADDRESS_AD0_HIGH    0xD1 //address pin high (VCC)
#define DEVICE_ADDR         ADDRESS_AD0_LOW
#define WHO_AM_I_VAL				0x73 //identity of MPU9250 is 0x71. identity of MPU9255 is 0x73.identity of ICM-20948 is 0xEA.

#define	ASAX			0x10	//MAG X -axis sensitivity adjustment value(Fuse ROM)
#define	ASAY			0x11	//MAG Y -axis sensitivity adjustment value(Fuse ROM)
#define	ASAZ			0x12	//MAG Y -axis sensitivity adjustment value(Fuse ROM)


typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
}MPU9255_TypeDef;

typedef struct
{
	int16_t X_Off_Err;
	int16_t Y_Off_Err;
	int16_t Z_Off_Err;
}MPU9255_TypeDef_Off;

typedef struct
{
	uint8_t Index;
	int16_t AvgBuffer[8];
}MPU9255_AvgTypeDef;
 
// ????????
typedef union
{
    struct
    {
			int16_t ACCEL_X;
			int16_t ACCEL_Y;
			int16_t ACCEL_Z;
			int16_t TEMP;
			int16_t GYRO_X;
			int16_t GYRO_Y;
			int16_t GYRO_Z;
			int16_t MAG_X;
			int16_t MAG_Y;
			int16_t MAG_Z;
    }Data;

    struct
    {
      uint8_t  Data[20];   
      
    }Buf;
	
}IMU_Type;



#endif	

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

/*****************************************************************************/
#ifdef BUS_IO

#include "I2C_IO.h"
#include "stdint.h"
#include "stdbool.h"

extern int16_t magn[3];
extern int16_t accel[3], gyro[3];
extern int16_t tempreture;

void MPU9255_Init_io(void);
void MPU9255_READ_MAG_io(int16_t magn[3]);
void MPU9255_READ_GYRO_io(int16_t gyro[3]);
void MPU9255_READ_ACCEL_io(int16_t accel[3]);
void MPU9255_READ_TEMP_io(int16_t* tempreture);
bool MPU9255_Check_io(void);
void MPU9255_CalAvgValue_io(uint8_t *pIndex, int16_t *pAvgBuffer, int16_t InVal, int32_t *pOutVal);
void MPU9255_InitGyrOffset_io(void);

#endif
/*****************************************************************************/

/*****************************************************************************/
#ifdef BUS_I2C

extern I2C_HandleTypeDef hi2c1;

extern uint8_t i2c1_rxbuf[100];
extern uint16_t i2c1_rxlen;
extern uint8_t i2c1_Rx_Data;
	 
void mpu9255_initi_i2c(void);
void read_mpu9255_i2c(void);

#endif
/*****************************************************************************/

/*****************************************************************************/
#ifdef BUS_SPI



#define MPU9255_READ         0x80      // SPI Address format, bit7 - > Read (1) or Write (0) operation ;

#define GPIO_CS9255          GPIO_PIN_4
#define PORT_CS9255          GPIOA

#define CS9255_LOW           HAL_GPIO_WritePin(PORT_CS9255, GPIO_CS9255, GPIO_PIN_RESET);          
#define CS9255_HIGH          HAL_GPIO_WritePin(PORT_CS9255, GPIO_CS9255, GPIO_PIN_SET);

void mpu9255_initi_spi(void);
void read_mpu9255_spi(void);

#endif
/*****************************************************************************/

#endif
