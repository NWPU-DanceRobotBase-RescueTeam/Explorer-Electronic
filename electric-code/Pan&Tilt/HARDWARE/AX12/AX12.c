#include "usart.h"	
#include "delay.h"
#include "AX12.h"	
#if EN_USART3_RX   //���ʹ���˽���
//����3�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART3_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART_RX_STA=0;       //����״̬���	

//��ʼ��IO ����3
//bound:������
void uart3_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
 
	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3); //GPIOC10����ΪUSART3_TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); //GPIOC11����ΪUSART3_RX
	
	//USART3�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC10��PC11

   //USART3��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
//	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ���3��DMA����
	
    USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��NVIC�Ĵ���

#endif                                                              
	
}


void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART3);//(USART3->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
#endif

void IO_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

	//GPIOC13��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//GPIOB12���ø�
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);//GPIOB13���õ�
}


//��ʼˮƽ״̬�µĳ�ʼλ��         ID Length  Write Reg   position   speed     check
const u8 num_1[11] =  {0xFF,0xFF,0x01,0x07, 0x03, 0x1E, 0x00,0x00, 0xFF,0x00, 0xD7 };
const u8 num_2[11] =  {0xFF,0xFF,0x02,0x07, 0x03, 0x1E, 0xff,0x01, 0xFF,0x00, 0xD6 };

void AX12_Init()
{
	u8 i;
	IO_Init();            //PB12 PB13�˿ڳ�ʼ�� 
	PB12 = 1;            //���ģ��AX12������Ƶ�·�����뷢�ͷ��ķ���
	PB13 = 0;  
	uart3_init(1000000);	 	//����3��ʼ��Ϊ1000000�����ڿ��ƶ��
	
	delay_ms(500);			//���1�����ʼ��
	for(i=0;i<11;i++)
	{	
		USART_SendData(USART3, num_1[i]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���	
	}
	delay_ms(500);     //���2�����ʼ��
	for(i=0;i<11;i++)
	{	
		USART_SendData(USART3, num_2[i]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���	
	} 
}

//�޸Ķ��ID
//ID: 		���ID
//trgID:	�޸ĺ�ID	
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
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���	
	}
	
	delay_ms(500);
	return 0x00;
}



