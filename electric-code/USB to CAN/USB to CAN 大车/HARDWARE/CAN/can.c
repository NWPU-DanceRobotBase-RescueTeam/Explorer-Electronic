/**************************************************************************
作者:瑞瑞
没有特别幸运，请先特别努力
**************************************************************************/
#include "P_bsp.h"

CanTxMsg TxMessage;
CanRxMsg RxMessage;
CAN_STA sta = CAN_START;
extern uint8_t IRQ_CAN;


/**************************************************************************
函数功能：CAN初始化函数
入口参数：tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
					tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
					tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
					brp :波特率分频器.范围:1~1024;  tq=(brp)*tpclk1
					波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp)
					FilterID:标识符ID
返回  值：无
**************************************************************************/	
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	

  	GPIO_InitTypeDef GPIO_InitStructure; 
		CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef  NVIC_InitStructure;

    //使能相关时钟
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
	
    //初始化GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA11,PA12
	
	  //引脚复用映射配置
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11复用为CAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12复用为CAN1
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode= mode;	 //模式设置 
  	CAN_InitStructure.CAN_SJW=tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // 初始化CAN1 
	
		//配置过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = 0;	//过滤器0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = Set_ID << 5 & 0xffff;
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_RTR_DATA | CAN_ID_STD;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x00;//0xffc0;//接受固定标准ID，允许最后一位有不同
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x00;//0x0005;	//标准数据/远程帧
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
	return 0;
}   
 
#if CAN1_RX0_INT_ENABLE	//使能RX0中断
//中断服务函数			    
void CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	int i=0;
    CAN_Receive(CAN1, 0, &RxMessage);
	for(i=0;i<8;i++)
	  printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
}
#endif


void CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0))
	{
		//TaskCanCommadDeal();
		CAN_Receive(CAN1 , CAN_FIFO0 , &RxMessage);
		IRQ_CAN = 1;
	}
//	CAN_Receive(CAN1 , CAN_FIFO0 , &RxMessage);
//	printf("%x\n%c\n",RxMessage.ExtId,RxMessage.Data[0]);
}




/**
  * @brief 	设置报文信息
  * @param  None
  * @retval None
  */
uint8_t CAN_SetMsg(void)
{
	static uint32_t	char_temp;
	static uint8_t count_temp = 0;
	static uint8_t check_sum = 0;
	if(now_number != DMA_GetCurrentMemoryTarget(DMA2_Stream5)) 
	{
		now_number = DMA_GetCurrentMemoryTarget(DMA2_Stream5);
		message_pointer = 0;
	}
	while(1)
	{
		//printf("2\n");
		if(message_pointer >= RX_LEN_GSM) 
			{
				finish_flag = 0;
				message_pointer = 0;
				return ERROR;
			}
		
	//	Prin_Queue(&USART_RX_Queue);
		
		switch(sta)
		{	
			case CAN_START:	char_temp = Buffer[!now_number][message_pointer++];
											TxMessage.DLC = char_temp;
											if(TxMessage.DLC != 8 )
											{
												sta = CAN_START;
												return ERROR;
											}
											TxMessage.IDE = CAN_ID_STD;
											TxMessage.RTR = CAN_RTR_DATA;
											sta = CAN_DATASTD1;
														
			case CAN_DATASTD1:
								char_temp = Buffer[!now_number][message_pointer++];
								check_sum = char_temp;
								sta = CAN_DATASTD2;
								break;
			
			
			case CAN_DATASTD2:
								char_temp = (uint32_t)Buffer[!now_number][message_pointer++] + (char_temp<< 8);
								TxMessage.StdId =	char_temp>>5;    //11位       
								sta = CAN_DATA;
								break;
												
												
												
			case CAN_DATA:
								check_sum += (TxMessage.Data[count_temp++] = Buffer[!now_number][message_pointer++]);
								if(count_temp == TxMessage.DLC) 
								{
									count_temp = 0;
									sta = CAN_CHECK;
								}
								break;						
			case CAN_CHECK:
								sta = CAN_START;
								if(check_sum == Buffer[!now_number][message_pointer++]) 
									return SUCCESS;
								else 
									return ERROR;

			}
	}
	
}
  
 







