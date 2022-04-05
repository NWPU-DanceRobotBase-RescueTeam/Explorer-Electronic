#include "usart.h"	
#include "IMU.h"	
#include "led.h"
#include "queue.h"
#include "delay.h"
#include "dma.h"

#define MAX_DATA 20
#if EN_USART2_RX   //如果使能了接收
//串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
//u16 USART_RX_STA=0;       //接收状态标记	

//初始化IO 串口2
//bound:波特率
void uart2_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
 
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2_TX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2_RX
	
	//USART2端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3

   //USART2初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
	
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE); //使能串口2的DMA接收
	
	USART_Cmd(USART2, ENABLE);  //使能串口2
	
			// 开启 串口空闲IDEL 中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	
	
#if EN_USART2_RX	

	//Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

#endif                                                              
	
}

int Receive_Flag=0;
int RecvLen=0;
 //串口2全局中断服务函数
void USART2_IRQHandler(void)       
{

      if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) 
       {
           DMA_Cmd(DMA1_Stream5,DISABLE); //关闭DMA传输
           RecvLen=USART2->DR;//清除中断标志位
           RecvLen=USART2->SR;
           RecvLen=MAX_DATA-DMA_GetCurrDataCounter(DMA1_Stream5);  //收到的数据长度

		   DMA_Cmd(DMA1_Stream5,ENABLE); //开启DMA传输
		   Receive_Flag=1;
      }  
}
 

void USART2_SendMessage(u8 len ,u16* Message)
{
	u8 t;
	for(t=0;t<len;t++)
	{
		USART_SendData(USART2, Message[t]);         
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
}

void IMU_init()     //imu初始化
{
	uart2_init(460800);
	delay_ms(10);
}

#endif

