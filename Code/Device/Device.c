#include "main.h"
#include "Device.h"
//#include "dvc_ADI16470.h"
#include "Message.h"
#include "msg_Qsens.h"

//#define USE_EXTERNAL_MTI

extern TIM_HandleTypeDef htim6;

extern UART_HandleTypeDef huart1; //!< <-> Main Port
extern UART_HandleTypeDef huart2; //!< <-> Main Port
extern UART_HandleTypeDef huart3; //!< <-> Debug Port
//extern SPI_HandleTypeDef hspi1;
//extern SPI_HandleTypeDef hspi4;

//ADI_16470_HandleTypeDef g_hImu16470;

LED_DEFINE_TABLE_BEGIN
{ "Live", GPIO_PIN_5,GPIOE },
{ "IN",   GPIO_PIN_4,GPIOE },
{ "OUT1",  GPIO_PIN_3,GPIOE },
{ "OUT2",  GPIO_PIN_2,GPIOE },
//{ "Live", GPIO_PIN_12,GPIOB },
//{ "IN",   GPIO_PIN_13,GPIOB },
//{ "OUT1",  GPIO_PIN_14,GPIOB },
//{ "OUT2",  GPIO_PIN_15,GPIOB },
LED_DEFINE_TABLE_END

//CONSTRUCT_CIRCULAR_BUFFER_MEMORY_POOL(g_USART1_Buffer, 256);
//CONSTRUCT_SYNC_CIRCULAR_BUFFER_MEMORY_POOL(g_USART1_Buffer, 256);
//CONSTRUCT_SYNC_CIRCULAR_BUFFER_MEMORY_POOL(g_USART2_Buffer, 256);
//CONSTRUCT_SYNC_CIRCULAR_BUFFER_MEMORY_POOL(g_USART3_Buffer, 256);
CONSTRUCT_CIRCULAR_BUFFER_MEMORY_POOL(g_USART1_Buffer, 256);
CONSTRUCT_CIRCULAR_BUFFER_MEMORY_POOL(g_USART2_Buffer, 256);
CONSTRUCT_CIRCULAR_BUFFER_MEMORY_POOL(g_USART3_Buffer, 256)

CONSTRUCT_DOUBLE_BUFFER_MEMORY_POOL(g_Usart1OutputBuffer, 1024);
CONSTRUCT_DOUBLE_BUFFER_MEMORY_POOL(g_Usart2OutputBuffer, 1024);
CONSTRUCT_DOUBLE_BUFFER_MEMORY_POOL(g_Usart3OutputBuffer, 1024);
extern ParseParam g_Usart1Parse;
extern ParseParam g_Usart2Parse;
extern ParseParam g_Usart3Parse;

XL_UART_IT_InputStreamTypeDef g_Usart1InputStream;
XL_UART_IT_InputStreamTypeDef g_Usart2InputStream;
XL_UART_IT_InputStreamTypeDef g_Usart3InputStream;

XL_UART_DMA_OutputStreamTypeDef g_Usart1OutputStream;
XL_UART_DMA_OutputStreamTypeDef g_Usart2OutputStream;
XL_UART_DMA_OutputStreamTypeDef g_Usart3OutputStream;
uint8_t usart1Temp;
uint8_t usart2Temp;
uint8_t usart3Temp;

void InitDevice(void)
{
	LED_TurnOffAll();
	g_Usart1InputStream.huart = &huart1;
	g_Usart1InputStream.pCircularBuffer = &g_USART1_Buffer;
	g_Usart1InputStream.pParseParam = &g_Usart1Parse;
	UART_LinkInputStream_IT(&g_Usart1InputStream);

	g_Usart2InputStream.huart = &huart2;
	g_Usart2InputStream.pCircularBuffer = &g_USART2_Buffer;
	g_Usart2InputStream.pParseParam = &g_Usart2Parse;
	UART_LinkInputStream_IT(&g_Usart2InputStream);
	
	g_Usart3InputStream.huart = &huart3;
	g_Usart3InputStream.pCircularBuffer = &g_USART3_Buffer;
	g_Usart3InputStream.pParseParam = &g_Usart3Parse;
	UART_LinkInputStream_IT(&g_Usart3InputStream);

//	HAL_UART_Receive_IT(&huart1, &usart1Temp, 1);
//	HAL_UART_Receive_IT(&huart2, &usart2Temp, 1);
//	HAL_UART_Receive_IT(&huart3, &usart3Temp, 1);

//	ClearSyncCircularBuffer(&g_USART1_Buffer);
//	ClearParseParam(&g_Usart1Parse);
//	ClearSyncCircularBuffer(&g_USART2_Buffer);
//	ClearParseParam(&g_Usart2Parse);
//	ClearSyncCircularBuffer(&g_USART3_Buffer);
//	ClearParseParam(&g_Usart3Parse);
	
	
	g_Usart1OutputStream.huart = &huart1;
	g_Usart1OutputStream.doublebuffer = &g_Usart1OutputBuffer;
	g_Usart2OutputStream.huart = &huart2;
	g_Usart2OutputStream.doublebuffer = &g_Usart2OutputBuffer;
    g_Usart3OutputStream.huart = &huart3;
	g_Usart3OutputStream.doublebuffer = &g_Usart3OutputBuffer;
	
	//	HAL_InitTick(1); // SYSTick 计时器1ms}	
	LED_TurnOnAll();
	//485 RX 
	HAL_GPIO_WritePin(RD_485_1_GPIO_Port, RD_485_1_Pin, GPIO_PIN_RESET);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_Delay(1000);
		
#ifndef USE_EXTERNAL_MTI	 
//	HAL_TIM_Base_Start_IT(&htim6);
#endif

}

unsigned char f125ms = 0;

MSG_QSENS_DatagramImuTypeDef ADI_data;
uint8_t GpsSyncFlag = 0x00;
extern uint8_t SendFlag;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static int AddrCount=0;
	uint16_t CommandAddr=0;
	uint8_t	pBuffer=0;
	if (htim->Instance == htim6.Instance)
	{
		///@todo 在这里补充输出函数
		f125ms = 1;
//		if(SendFlag==1)
//		{
//			
//			//for(i=0;i<8;i++)
//			//if(SendOverFlag==1|f125ms==1)
//			
//			//	SendOverFlag=0;
//				CommandAddr=(0x06<<8)|(AddrCount+1);
//				AddrCount++;
//				QSENS_MakeImuFrame2(&pBuffer,1,&g_Usart2OutputBuffer,CommandAddr,2);
//			  //  SendDoubleBuffer(g_Usart3OutputStream.doublebuffer, &huart3);
//				SendDoubleBuffer(&g_Usart2OutputBuffer, &huart2);
//		        if(AddrCount>=8) AddrCount=0;				
//			
//		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		RecieveInputStream_IT(huart, &g_Usart1InputStream);
//		PushSyncCircularBuffer(usart1Temp, &g_USART1_Buffer);
//		HAL_UART_Receive_IT(&huart1, &usart1Temp, 1);
	}
	else if (huart->Instance == USART2)
	{
		RecieveInputStream_IT(huart, &g_Usart2InputStream);
//		PushSyncCircularBuffer(usart2Temp, &g_USART2_Buffer);
//		HAL_UART_Receive_IT(&huart2, &usart2Temp, 1);		
	}
	else if (huart->Instance == USART3)
	{
		RecieveInputStream_IT(huart, &g_Usart3InputStream);
//		PushSyncCircularBuffer(usart3Temp, &g_USART3_Buffer);
//		HAL_UART_Receive_IT(&huart3, &usart3Temp, 1);		
	}
	else
	{
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if (HAL_UART_GetError(huart) == HAL_UART_ERROR_ORE)
	{
		huart->RxState = HAL_UART_STATE_READY;
	}
	else if (HAL_UART_GetError(huart) == HAL_UART_ERROR_DMA)
	{
		huart->RxState = HAL_UART_STATE_READY;
	}
	else
	{
	}

	if (huart->Instance == USART1)
	{
		HAL_UART_Receive_IT(&huart1, &(g_Usart1InputStream.ucTemp), 1);
	//	HAL_UART_Receive_IT(&huart1, &usart1Temp, 1);
	}
	else if (huart->Instance == USART2)
	{
		HAL_UART_Receive_IT(&huart2, &(g_Usart2InputStream.ucTemp), 1);
	//	HAL_UART_Receive_IT(&huart2, &usart2Temp, 1);
	}
	else if (huart->Instance == USART3)
	{
		HAL_UART_Receive_IT(&huart3, &(g_Usart3InputStream.ucTemp), 1);
		//HAL_UART_Receive_IT(&huart3, &usart3Temp, 1);
	}
	else
	{
	}
}

int fputc(int ch, FILE* f)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 100);
	return ch;
}

void SendOutputBuffer(void)
{
	SendDoubleBuffer(g_Usart3OutputStream.doublebuffer, &huart3);
}