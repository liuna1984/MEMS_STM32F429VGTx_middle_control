#include "Global.h"
#include "Device.h"
#include "Message.h"
#include "MainEntry.h"
#include "msg_Qsens.h"
extern XL_UART_DMA_OutputStreamTypeDef g_Usart1OutputStream;
extern XL_UART_DMA_OutputStreamTypeDef g_Usart2OutputStream;
extern XL_UART_DMA_OutputStreamTypeDef g_Usart3OutputStream;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DoubleBuffer g_Usart1OutputBuffer;
extern DoubleBuffer g_Usart2OutputBuffer;
extern DoubleBuffer g_Usart3OutputBuffer;

extern unsigned char f125ms;
uint8_t SendFlag=1;
uint8_t SendOverFlag=1;
uint32_t RecCount=0;
void HeartPulse(void);
void MainEntry(void)
{
	int i=0;
	uint8_t	pBuffer=0;
	uint16_t CommandAddr;
	uint8_t	DelayReady=0;
	InitDevice();
	InitMessage();
	
    while (1)
    {
		HeartPulse();
		ParseFrameUsart1();
		ParseFrameUsart2();
		ParseFrameUsart3();
/*每隔125ms，发送1包数据*/
//		if(SendFlag==1)
//		{
//			
//			//for(i=0;i<8;i++)
//			//if(SendOverFlag==1|f125ms==1)
//			if(f125ms==1)
//			{
//				f125ms=0;
//				SendOverFlag=0;
//				CommandAddr=(0x06<<8)|(i+1);
//				i++;
//				QSENS_MakeImuFrame2(&pBuffer,1,&g_Usart2OutputBuffer,CommandAddr,2);
//			  //  SendDoubleBuffer(g_Usart3OutputStream.doublebuffer, &huart3);
//				SendDoubleBuffer(&g_Usart2OutputBuffer, &huart2);
//			}
//			
//			if(i>=8) i=0;				
//			
//		}
/*等待反馈后发送*/
		if(SendFlag==1)
		{
			
			//for(i=0;i<8;i++)
			
			if(SendOverFlag==1|DelayReady==1)
			{
				SendOverFlag=0;
				DelayReady=0;
				CommandAddr=(0x06<<8)|(i+1);
				i++;
				QSENS_MakeImuFrame2(&pBuffer,1,&g_Usart2OutputBuffer,CommandAddr,2);
			  //  SendDoubleBuffer(g_Usart3OutputStream.doublebuffer, &huart3);
				SendDoubleBuffer(&g_Usart2OutputBuffer, &huart2);
			}
			else
			{
				HAL_Delay(1);
				RecCount++;
				if(RecCount>60)
				{
					DelayReady=1;
					RecCount=0;
				}
			}
			if(i>=8) i=0;				
			
		}
/*1秒都发出去*/
//		if(f125ms==1&&SendFlag==1)
//		{
//			f125ms=0;
//			for(i=0;i<8;i++)
//			{
//				CommandAddr=(0x06<<8)|(i+1);
//				QSENS_MakeImuFrame2(&pBuffer,1,&g_Usart2OutputBuffer,CommandAddr,2);
//	          //  SendDoubleBuffer(g_Usart3OutputStream.doublebuffer, &huart3);
//			    SendDoubleBuffer(&g_Usart2OutputBuffer, &huart2);
//				
//			}
//		}
    }
}

void HeartPulse()
{
	static long counter = 0;
	counter++;
	if (counter>100000)
	{
		counter = 0;
		LED_Toggle(LED1);
//		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_2);
		
	}
}
