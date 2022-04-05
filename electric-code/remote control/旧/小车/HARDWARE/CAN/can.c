/**************************************************************************
���ߣ����δ�Ρ�����
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#include "can.h"
#include <string.h>

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
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//��ʶλģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x01e0>>5)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow =CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =(0x02e0>>5)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������0
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
	CAN_FilterInitStructure.CAN_FilterNumber = 1;	//������1
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//��ʶλģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x03e0 >> 5)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x04e0 >> 5)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������1������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������1
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
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
		CanRxMsg RxMessage;
	static float Goal_Speed = 0;
	static float Goal_Position = 0;
  u8 i;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	switch(RxMessage.StdId)
	{
    // ����������Ϣ
    case 0x01e0 >> 5 : //�����Ĵ��趨�ٶ�
		{
			memcpy(&Goal_Speed,RxMessage.Data,4);
      //Goal_Speed = *((float*)(&RxMessage.Data[0])); //��1
      Goal_Speed = Goal_Speed *1000;
			//printf("ID1:%d\r\n",(int32_t)Goal_Speed);
		  M3508[0].PID.Goal_Speed =(int32_t) Goal_Speed;
			//printf("Goal_Speed 1: %d\r\n",M3508[0].PID.Goal_Speed);
			//printf("0x01e0\r\n");
			//memcpy(&Goal_Speed,RxMessage.Data+4,4);
			//Goal_Speed=*((float*)(&RxMessage.Data[4]));//��2
			//Goal_Speed=Goal_Speed *1;
			//M3508[1].PID.Goal_Speed=(int32_t)Goal_Speed;
			//printf("Goal_Speed 2: %d\r\n",M3508[1].PID.Goal_Speed);
			printf("�����Ĵ��趨�ٶ�Ϊ:%d",M3508[0].PID.Goal_Speed );
			printf("\r\n");
			
			memcpy(&Goal_Speed,RxMessage.Data+4,4);
			Goal_Speed =Goal_Speed *1000;
			M3508[1].PID.Goal_Speed =(int32_t) Goal_Speed;
			printf("�����Ĵ��趨�ٶ�Ϊ:%d",M3508[1].PID.Goal_Speed );
			printf("\r\n");
			break;
    }
    case 0x02e0 >> 5 ://���Ĵ���ʼ��
		{
			memcpy(&Goal_Speed,&RxMessage.Data[0],4);
      //Goal_Speed = *((float*)(&RxMessage.Data[0])); //
      if(Goal_Speed==1) Subtrack_Rest();
			//printf("ID3:%d\r\n",(int32_t)Goal_Speed);
			//printf("Goal_Speed 3: %d\r\n",M3508[2].PID.Goal_Speed);
			//memcpy(&Goal_Speed,&RxMessage.Data[4],4);
      //Goal_Speed = *((float*)(&RxMessage.Data[4])); //
      //Goal_Speed =Goal_Speed *1000;
			//printf("ID4:%d\r\n",(int32_t)Goal_Speed);
		  //M3508[3].PID.Goal_Speed =(int32_t) Goal_Speed;
			//printf("Goal_Speed 4: %d\r\n",M3508[3].PID.Goal_Speed);
			break;
    }
    case 0x03e0 >> 5 : 									//��ǰ���Ĵ�����ǰ���Ĵ��Ƕ�
		{
			memcpy(&Goal_Position,RxMessage.Data,4);
			Goal_Position= Goal_Position *57.29578049;
			if(Goal_Position<0) Goal_Position=0;//������λ
			M3508[2].PID.Goal_Position =207-(int32_t) Goal_Position*1.1111;
			if(M3508[2].PID.Goal_Position<1) M3508[2].PID.Goal_Position=1;//���������λ
			adc_flag[1]=1;
			printf("��ǰ���Ĵ��趨�Ƕ�Ϊ:%d",M3508[2].PID.Goal_Position);
			printf("\r\n");

			memcpy(&Goal_Position,RxMessage.Data+4,4);
			Goal_Position= Goal_Position *57.29578049;
			if(Goal_Position<0) Goal_Position=0;//������λ
			M3508[4].PID.Goal_Position =95+(int32_t) Goal_Position*1.1111;
			if(M3508[4].PID.Goal_Position>298) M3508[4].PID.Goal_Position=298;//���������λ
			adc_flag[3]=1;
			printf("��ǰ���Ĵ��趨�Ƕ�Ϊ:%d",M3508[4].PID.Goal_Position);
			printf("\r\n");
			
			adc_flag[0]=1;
			break;
    }
    case 0x04e0 >> 5 : 									//����Ĵ����Һ��Ĵ��Ƕ�
		{
			memcpy(&Goal_Position,RxMessage.Data,4);	
			Goal_Position= Goal_Position *57.29578049;
			if(Goal_Position<0) Goal_Position=0;//������λ
			M3508[3].PID.Goal_Position =82+(int32_t) Goal_Position*1.1111;
			if(M3508[3].PID.Goal_Position>298) M3508[3].PID.Goal_Position=298;//���������λ
			adc_flag[2]=1;
			printf("����Ĵ��趨�Ƕ�Ϊ:%d",M3508[3].PID.Goal_Position);
			printf("\r\n");
			

			memcpy(&Goal_Position,RxMessage.Data+4,4);
			Goal_Position= Goal_Position *57.29578049;
			if(Goal_Position<0) Goal_Position=0;//������λ
			M3508[5].PID.Goal_Position =219-(int32_t) Goal_Position*1.1111;
			if(M3508[5].PID.Goal_Position<1) M3508[5].PID.Goal_Position=1;//���������λ
			adc_flag[4]=1;
			printf("�Һ��Ĵ��趨�Ƕ�Ϊ:%d",M3508[5].PID.Goal_Position);
			printf("\r\n");
			
			adc_flag[0]=1;
			break;
    }
  }
	return;
}




//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_Msg(u8* msg,u8 len,uint32_t id)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=id;	 // ��׼��ʶ��Ϊ0
  TxMessage.ExtId=0x00;	 // ������չ��ʾ����29λ��
  TxMessage.IDE=CAN_Id_Standard;		  // ʹ����չ��ʶ��
  TxMessage.RTR=CAN_RTR_Data;		  // ��Ϣ����Ϊ����֡��һ֡8λ
  TxMessage.DLC=8;							 // ������֡��Ϣ
  for(i=0;i<len;i++) 
	TxMessage.Data[i]=msg[i]; 
	for(i=len;i<8;i++) 
	TxMessage.Data[i]=0;          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;	
}

//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}

u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef  NVIC_InitStructure;


    //ʹ�����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTBʱ��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//ʹ��CAN2ʱ��	
	
    //��ʼ��GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PB12,PB13
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2); //GPIOB12����ΪCAN2
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2); //GPIOB13����ΪCAN2
	  
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
  	CAN_Init(CAN2, &CAN_InitStructure);   // ��ʼ��CAN2
	
	CAN_FilterInitStructure.CAN_FilterNumber = 14;	//������14
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow =0x0000;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =0x0000;//32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������14
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
	
/*		//���ù�����
		//���ù�����0
	CAN_FilterInitStructure.CAN_FilterNumber = 14;	//������0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x01e0>>5)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow =CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =(0x02e0>>5)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������0
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
  
	//���ù�����1
	CAN_FilterInitStructure.CAN_FilterNumber = 15;	//������1
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x03e0 >> 5)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x04e0 >> 5)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������1
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
	//���ù�����2
	CAN_FilterInitStructure.CAN_FilterNumber = 16;	//������2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0201)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0202)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������2
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
	//���ù�����3
	CAN_FilterInitStructure.CAN_FilterNumber = 17;	//������2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0203)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0204)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������2
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
	//���ù�����4
	CAN_FilterInitStructure.CAN_FilterNumber = 18;	//������2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0205)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0206)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������2
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��	*/
	
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	return 0;
}   
 
#if CAN2_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
void CAN2_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	int i=0;
    CAN_Receive(CAN2, 0, &RxMessage);
	for(i=0;i<8;i++)
	printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
}
#endif

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN2_Send_Msg(u8* msg,u8 len,uint32_t id)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;		
  TxMessage.StdId=id;	 // ��׼��ʶ��Ϊ0
  TxMessage.ExtId=0x00;	 // ������չ��ʾ����29λ��
  TxMessage.IDE=CAN_Id_Standard;		  // ʹ����չ��ʶ��
  TxMessage.RTR=CAN_RTR_Data;		  // ��Ϣ����Ϊ����֡��һ֡8λ
  TxMessage.DLC=8;							 // ������֡��Ϣ
  for(i=0;i<len;i++) 
	TxMessage.Data[i]= msg[i];		
	for(i=len;i<8;i++) 
	TxMessage.Data[i]=0;          
  mbox= CAN_Transmit(CAN2, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;		

}
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 CAN2_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN2,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}


/**
  * @brief  CAN1�жϷ�����
  * @param  void
  * @retval void
  */


void CAN2_RX0_IRQHandler(void){
	CanRxMsg RxMessage;
	u8 i;
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
	switch(RxMessage.StdId)
	{
		case 0x0201 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[0].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(0);
			//printf("0x201\r\n");
			//printf("Speed:%d\r\n",M3508[0].Speed);
			//Motor_Length_Calc();
			M3508[0].Angle=angle[0];
      break;
    }
    case 0x0202 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[1].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(1);
			
			//printf("0x202");
			//Motor_Length_Calc();
			M3508[1].Angle=angle[1];
			break;
    }
		case 0x0203 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[2].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(2);
			//printf("%d\r\n",M3508[2].Speed);
			//printf("0x202");
			//Motor_Length_Calc();
			M3508[2].Angle=angle[2];
			break;
    }
		case 0x0204 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[3].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(3);
			
			//printf("0x202");
			//Motor_Length_Calc();
			M3508[3].Angle=angle[3];
			break;
    }
		case 0x0205 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[4].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(4);
			
			//printf("0x202");
			break;
    }
		case 0x0206 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[5].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(5);
			
			//printf("0x202");
			break;
    }
  }
	return;
}





