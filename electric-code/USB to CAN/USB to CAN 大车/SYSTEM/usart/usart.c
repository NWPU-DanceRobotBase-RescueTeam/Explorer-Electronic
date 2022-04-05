#include "sys.h"
#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

Queue USART_RX_Queue , USART_TX_Queue;

uint8_t Buffer[2][RX_LEN_GSM];//双缓存
uint8_t finish_flag = 0;
u8 now_number = 0,message_pointer;

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
	
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//使能DMA1时钟
	
	//DMA发送中断初始化
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	//DMA通道配置
	DMA_DeInit(DMA2_Stream5);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);	//外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Buffer[0];	//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //dma传输方向
	DMA_InitStructure.DMA_BufferSize = RX_LEN_GSM;  //设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //设置DMA的内存递增模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //外设数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //内存数据字长
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //设置DMA的传输模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;    //设置DMA的优先级别
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  //指定如果FIFO模式或直接模式将用于指定的流 ： 不使能FIFO模式  
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;    //指定了FIFO阈值水平    
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;       //指定的Burst转移配置内存传输 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  //指定的Burst转移配置外围转移 */  
	DMA_Init(DMA2_Stream5, &DMA_InitStructure);
	DMA_DoubleBufferModeConfig(DMA2_Stream5, (uint32_t)Buffer[1], DMA_Memory_0);
    DMA_DoubleBufferModeCmd(DMA2_Stream5, ENABLE);
	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);   
	DMA_Cmd(DMA2_Stream5,ENABLE);



 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);

	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}


#if EN_USART1_RX
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t Re_Data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Re_Data = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		if (!Full_Queue(USART_RX_Queue))  //队列是否已满
		{
			Push_Queue(&USART_RX_Queue , Re_Data);   //  把串口数据塞进队列
		}
	}
}

#endif	



void DMA2_Stream5_IRQHandler(void)  //DMA传输完成中断
{
//	uint16_t i;
    if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5) == SET)
    {
		
        DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);
//		for(i = 0;i<11;i++)putchar(Buffer[0][i]);
//		for(i = 0;i<11;i++)putchar(Buffer[1][i]);
		finish_flag = 1;
    }
 
}

