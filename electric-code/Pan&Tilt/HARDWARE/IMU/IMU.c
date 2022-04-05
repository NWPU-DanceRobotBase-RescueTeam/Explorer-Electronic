#include "usart.h"	
#include "IMU.h"	
#include "led.h"
#include "queue.h"
#include "delay.h"
#include "dma.h"

#define MAX_DATA 20
#if EN_USART2_RX   //���ʹ���˽���
//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART_RX_STA=0;       //����״̬���	

//��ʼ��IO ����2
//bound:������
void uart2_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
 
	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2_TX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2_RX
	
	//USART2�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3

   //USART2��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE); //ʹ�ܴ���2��DMA����
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2
	
			// ���� ���ڿ���IDEL �ж�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	
	
#if EN_USART2_RX	

	//Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

#endif                                                              
	
}

int Receive_Flag=0;
int RecvLen=0;
 //����2ȫ���жϷ�����
void USART2_IRQHandler(void)       
{

      if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) 
       {
           DMA_Cmd(DMA1_Stream5,DISABLE); //�ر�DMA����
           RecvLen=USART2->DR;//����жϱ�־λ
           RecvLen=USART2->SR;
           RecvLen=MAX_DATA-DMA_GetCurrDataCounter(DMA1_Stream5);  //�յ������ݳ���

		   DMA_Cmd(DMA1_Stream5,ENABLE); //����DMA����
		   Receive_Flag=1;
      }  
}
 

void USART2_SendMessage(u8 len ,u16* Message)
{
	u8 t;
	for(t=0;t<len;t++)
	{
		USART_SendData(USART2, Message[t]);         
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}

void IMU_init()     //imu��ʼ��
{
	uart2_init(460800);
	delay_ms(10);
}

#endif

