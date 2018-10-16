#ifndef GLOBAL_H_201704091620
#define GLOBAL_H_201704091620 
/**
* @file
* @author Qiaoyy@xlnav.com
* @date
* @brief
* Copyright (c) 2018 XiLang Co. Ltd.
* All rights reserved.
*/

/* Includes ------------------------------------------------------------------*/
#include "Includes.h"

/* Exported macro ------------------------------------------------------------*/
/**
  * @brief Ϊ˫���������侲̬�ڴ�
  * @param buf ˫������
  * @param size ˫��������С������Ϊ����0������
  * @warning ֻ��Ϊȫ�ֱ�������ʹ�ã������ں����ڲ�����
  */
#define CONSTRUCT_DOUBLE_BUFFER_MEMORY_POOL(buf, size) uint8_t buf##_memory[2][size];\
                                                DoubleBuffer buf = {{buf##_memory[0], buf##_memory[1]},{0,0}, 0, size};
 /**
  * @brief Ϊѭ�����������侲̬�ڴ�
  * @param buf ѭ��������
  * @param size ѭ����������С������Ϊ����0������
  * @warning ֻ��Ϊȫ�ֱ�������ʹ�ã������ں����ڲ�����
  */
#define CONSTRUCT_CIRCULAR_BUFFER_MEMORY_POOL(buf, size) uint8_t buf##_memory[size];\
                                                  CircularBuffer buf = {buf##_memory, 0, 0, 0, 0, 0, size, 0, 0};									
#define CONSTRUCT_SYNC_CIRCULAR_BUFFER_MEMORY_POOL(buf, size) unsigned char buf##_memory[size];\
														 unsigned char buf##_sync_memory[size];\
                                                  SyncCircularBuffer buf = {buf##_memory, buf##_sync_memory, 0, 0, 0, 0, 0, size, 0, 0};													  
#define PUSH_VAR_DOUBLEBUFFER(var, buf) PushBytesDoubleBuffer((uint8_t *)(&(var)), sizeof(var), (buf));

#define	BUFFER_NO_ERROR 0x00											  
#define BUFFER_ERROR_OVERLAB 0x01	
#define BUFFER_ERROR_PARAM_NO_MATCH 0x02
#define	BUFFER_ERROR_POP_MORE_THAN_PUSH 0x03		

 /**
  * @brief �Ƕ�ֵתΪ����ֵ
  * @param x �Ƕ�ֵ
  */
#define DEG2RAD(x) ((x)/180*3.14159265358979)

 /**
  * @brief ����ֵתΪ�Ƕ�ֵ
  * @param x ����ֵ
  */
#define RAD2DEG(x) ((x)*180/3.14159265358979)
/* Exported types ------------------------------------------------------------*/
/**
  * @brief  ѭ��������
  */
typedef struct
{
	uint8_t *m_ucDataBuffer;
	volatile uint16_t m_ulPopTimes;
	volatile uint16_t m_ulPushTimes;
	volatile uint16_t m_nBufferHeadPosition;
	volatile uint16_t m_nBufferTailPosition;
	volatile uint16_t m_nBufferLength;
	volatile uint16_t m_nBufferSize;
	volatile uint8_t m_ucErrorCode;
	volatile uint8_t m_ucLock;
}CircularBuffer;

/**
  * @brief  ����ͬ����־��ѭ��������
  */
typedef struct
{
	uint8_t *m_ucDataBuffer;
	uint8_t *m_ucSync;
	volatile uint16_t m_ulPopTimes;
	volatile uint16_t m_ulPushTimes;
	volatile uint16_t m_nBufferHeadPosition;
	volatile uint16_t m_nBufferTailPosition;
	volatile uint16_t m_nBufferLength;
	volatile uint16_t m_nBufferSize;
	volatile uint8_t m_ucErrorCode;
	volatile uint8_t m_ucLock;
}SyncCircularBuffer;

/**
  * @brief  ˫������
  */
typedef struct
{
	uint8_t *m_ucDataBuffer[2];
	volatile uint16_t m_nBufferLength[2];
	volatile uint16_t m_nBufferNumber;
	volatile uint16_t m_nBufferSize;
	volatile uint8_t m_ucErrorCode;
}DoubleBuffer;
/* Exported constants --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* ѭ�����麯�� *****************************************************************/
int PopCircularBuffer(uint8_t *ucByte, CircularBuffer* pBuf);
int PushCircularBuffer(uint8_t ucByte, CircularBuffer* pBuf);
void ClearCircularBuffer(CircularBuffer* pBuf);
/* ����ͬ����־��ѭ�����麯�� *****************************************************/
int PopSyncCircularBuffer(uint8_t *ucByte, SyncCircularBuffer* pBuf, uint8_t *ucSync);
int PushSyncCircularBuffer(uint8_t ucByte, SyncCircularBuffer* pBuf);
void ClearSyncCircularBuffer(SyncCircularBuffer* pBuf);
void MarkSyncCircularBuffer(uint8_t deviceID, SyncCircularBuffer* pBuf);
/* ˫���������� *****************************************************************/
int PopDoubleBuffer(uint8_t *ucBlock, DoubleBuffer* pBuf);
int PushDoubleBuffer(uint8_t ucByte, DoubleBuffer* pBuf);
void ClearDoubleBuffer(DoubleBuffer* pBuf);
uint16_t GetDoubleBufferLength(DoubleBuffer* pBuf);
uint8_t* GetDoubleBuffer(DoubleBuffer* pBuf);
void PushBytesDoubleBuffer(uint8_t *pBytes, uint16_t nLength, DoubleBuffer* pBuf);
void PushWordDoubleBuffer(uint16_t word, DoubleBuffer* pBuf);
#endif
/************************ (C) COPYRIGHT XiLang CO. Ltd. *******END OF FILE*****/
