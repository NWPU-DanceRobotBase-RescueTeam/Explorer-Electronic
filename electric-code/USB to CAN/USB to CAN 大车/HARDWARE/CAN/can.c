/**************************************************************************
����:����
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#include "P_bsp.h"

CanTxMsg TxMessage;
CanRxMsg RxMessage;
CAN_STA sta = CAN_START;
extern uint8_t IRQ_CAN;


/**************************************************************************
�������ܣ�CAN��ʼ������
��ڲ�����tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
					tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
					tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
					brp :�����ʷ�Ƶ��.��Χ:1~1024;  tq=(brp)*tpclk1
					������=Fpclk1/((tbs1+1+tbs2+1+1)*brp)
					FilterID:��ʶ��ID
����  ֵ����
**************************************************************************/	
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	

  	GPIO_InitTypeDef GPIO_InitStructure; 
		CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef  NVIC_InitStructure;

    //ʹ�����ʱ��
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA11,PA12
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
	  
  	//CAN��Ԫ����
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//����Զ����߹���	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  	CAN_InitStructure.CAN_NART=ENABLE;	//��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
  	CAN_InitStructure.CAN_Mode= mode;	 //ģʽ���� 
  	CAN_InitStructure.CAN_SJW=tsjw;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1 
	
		//���ù�����
	CAN_FilterInitStructure.CAN_FilterNumber = 0;	//������0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = Set_ID << 5 & 0xffff;
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_RTR_DATA | CAN_ID_STD;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x00;//0xffc0;//���̶ܹ���׼ID���������һλ�в�ͬ
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x00;//0x0005;	//��׼����/Զ��֡
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������0
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
	return 0;
}   
 
#if CAN1_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
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
  * @brief 	���ñ�����Ϣ
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
								TxMessage.StdId =	char_temp>>5;    //11λ       
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
  
 







