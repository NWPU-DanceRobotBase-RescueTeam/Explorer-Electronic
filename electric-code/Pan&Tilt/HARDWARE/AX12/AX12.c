#include "usart.h"	
#include "delay.h"
#include "AX12.h"	
#if EN_USART3_RX   //如果使能了接收
//串口3中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
//u16 USART_RX_STA=0;       //接收状态标记	

//初始化IO 串口3
//bound:波特率
void uart3_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
 
	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3); //GPIOC10复用为USART3_TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); //GPIOC11复用为USART3_RX
	
	//USART3端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC10，PC11

   //USART3初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
//	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE); //使能串口3的DMA发送
	
    USART_Cmd(USART3, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器

#endif                                                              
	
}


void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART3);//(USART3->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif

void IO_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

	//GPIOC13初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//GPIOB12设置高
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);//GPIOB13设置低
}


//初始水平状态下的初始位置         ID Length  Write Reg   position   speed     check
const u8 num_1[11] =  {0xFF,0xFF,0x01,0x07, 0x03, 0x1E, 0x00,0x00, 0xFF,0x00, 0xD7 };
const u8 num_2[11] =  {0xFF,0xFF,0x02,0x07, 0x03, 0x1E, 0xff,0x01, 0xFF,0x00, 0xD6 };

void AX12_Init()
{
	u8 i;
	IO_Init();            //PB12 PB13端口初始化 
	PB12 = 1;            //软件模拟AX12舵机控制电路接收与发送方的非门
	PB13 = 0;  
	uart3_init(1000000);	 	//串口3初始化为1000000，用于控制舵机
	
	delay_ms(500);			//舵机1方向初始化
	for(i=0;i<11;i++)
	{	
		USART_SendData(USART3, num_1[i]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束	
	}
	delay_ms(500);     //舵机2方向初始化
	for(i=0;i<11;i++)
	{	
		USART_SendData(USART3, num_2[i]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束	
	} 
}

//修改舵机ID
//ID: 		舵机ID
//trgID:	修改后ID	
u8 axModifyID(u8 ID, u8 trgID)
{
	u8 i,check;
	u8 order[8] = {0xff,0xff,0x00,0x04,0x03,0x00,0x00,0x00};
	
	check = 0 ;
	
	order[2] = ID;
	order[5] = 3;//P_ID; 	
	order[6] = trgID; 		
	
	for(i = 2;i<7;i++)
	{
		check = check + order[i];
	}
	
	order[7] = check;
	
	for(i=0;i<8;i++)
	{	
		USART_SendData(USART3,order[i]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束	
	}
	
	delay_ms(500);
	return 0x00;
}



