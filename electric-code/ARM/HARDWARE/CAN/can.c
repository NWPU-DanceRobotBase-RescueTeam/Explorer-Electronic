#include <string.h>
#include <math.h>
#include "can.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "arm.h"
#include "pot.h"
#include "GIM8008.h"
#include "Motor_Control.h"

/**
  * @brief  CAN1初始化函数
  * @param  tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
			tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
			tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
			brp :波特率分频器.范围:1~1024;  tq=(brp)*tpclk1
			波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp)
			FilterID:标识符ID
  * @retval void
  */
void CAN1_Mode_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode)
{
  	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef CAN_InitStructure;
  	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
#if CAN1_RX0_INT_ENABLE 
   	NVIC_InitTypeDef NVIC_InitStructure;
#endif
	
    //使能相关时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
	
    //初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA11,PA12
	
	//引脚复用映射配置
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1); //GPIOA11复用为CAN1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); //GPIOA12复用为CAN1
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM = DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM = DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM = DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART = ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM = DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP = DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode = mode;	 //模式设置 
  	CAN_InitStructure.CAN_SJW = tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1 = tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2 = tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler = brp;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // 初始化CAN1 
    
	//配置过滤器0
    CAN_FilterInitStructure.CAN_FilterNumber = 0;	  //过滤器0
  	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 
  	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x2f<<5;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow = 0x37<<5;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x3f<<5;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x47<<5;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
		
	//配置过滤器1
 	CAN_FilterInitStructure.CAN_FilterNumber = 1;	  //过滤器1
  	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 
  	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x5f<<5;////32位ID
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器1关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
		
#if CAN1_RX0_INT_ENABLE
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
#endif
}   
 
/**
  * @brief  CAN1中断服务函数,工控数据发送，运行与底盘有关函数
  * @param  void
  * @retval void
  */

#if CAN1_RX0_INT_ENABLE	//使能RX0中断

void CAN1_RX0_IRQHandler(void)
{
	static s16 base_receive = 0, pwm4_receive = 0, pwm5_receive = 0, pwm6_receive = 0, pwm7_receive = 0;
//	static float gim_receive = 0;
	s16 int_temp = 0;
	float float_temp = 0;
	float radian = 0, radian2 = 0;
	CanRxMsg RxMessage;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	switch (RxMessage.StdId)
	{
	case 0x2f:  //0x05e0>>5
		memcpy(&radian, RxMessage.Data, 4);		      // -2.356 - 0.0 - 2.356
		printf("3:%.2f\r\n", radian);
		radian = radian + 2.238f;
		int_temp = (int)(radian * SERVO_RAD1 + 500);
		if (int_temp <= 500)
			int_temp = 500;
		else if (int_temp >= 2500)
			int_temp = 2500;
		if (int_temp != base_receive)
		{
			base_receive = int_temp;
			servo_set(ID3, base_receive);
		}
		printf("3:%.2d\r\n", base_receive);
		break;

	case 0x37:  //0x06e0>>5					
		memcpy(&radian, RxMessage.Data, 4);
		float_temp = -radian;             				// 0 - 3.14
		if (float_temp <= -4)
			float_temp = -4;
		else if (float_temp >= 0)
			float_temp = 0;
		if (fabs(float_temp - first_arm_goal_position) < 0.5)
		{
			if (float_temp != first_arm_goal_position)
			{
				first_arm_goal_position = float_temp;
				Send_8008_Control((first_arm_goal_position + 0.25f) * 0.8f, 0, GIM_Kp, GIM_Kd, 0);
			}
			printf("1:%.2f\r\n", first_arm_goal_position);
		}

		memcpy(&radian2, &RxMessage.Data[4], 4);              // 0 - -2.16
		radian2 = -radian2;
		//printf("2:%.2f\r\n", radian2);
		float_temp = (float)(radian2 * MOTOR_RAD);
		if (float_temp >= 270)
			float_temp = 270.0f;
		else if (float_temp <= 0)
			float_temp = 0;
		if (float_temp != second_arm_goal_postion)
		{
			second_arm_goal_postion = float_temp;
			Motor[0].Start_Flag = 1;
//			max_speed = 85;
//			Motor[0].PID.Error_Sum = 0;
		}
		printf("2:%.2f\r\n", second_arm_goal_postion);
		
		break;

	case 0x3f:  //0x07e0>>5
		memcpy(&radian, RxMessage.Data, 4);                 // -1.57 - 0 - 1.57
		radian = -radian + 1.57f;
		int_temp = (int)(radian * SERVO_RAD + 500);
		if (int_temp <= 500)
			int_temp = 500;
		else if (int_temp >= 2500)
			int_temp = 2500;
		if (int_temp != pwm4_receive)
		{
			pwm4_receive = int_temp;
			servo_set(ID4, pwm4_receive);
		}
		printf("4:%d\r\n", pwm4_receive);

		memcpy(&radian, &RxMessage.Data[4], 4);             // -1.81 - 0 - 0.3  
		radian = -radian + 1.09f;
		int_temp = (int)(radian * SERVO_RAD + 500);
		if (int_temp <= 1000)
			int_temp = 1000;
		else if (int_temp >= 2350)
			int_temp = 2350;
		if (int_temp != pwm5_receive)
		{
			pwm5_receive = int_temp;
			servo_set(ID5, pwm5_receive);
		}
		printf("5:%d\r\n", pwm5_receive);
		break;

	case 0x47:  //0x08e0>>5
		memcpy(&radian, RxMessage.Data, 4);               // 0 - 1
		radian = radian + 1.53f;
		int_temp = (int)(radian * SERVO_RAD + 500);
		if (int_temp <= 1450)
			int_temp = 1450;
		else if (int_temp >= 2150)
			int_temp = 2150;
		if (int_temp != pwm7_receive)
		{
			pwm7_receive = int_temp;
			servo_set(ID7, pwm7_receive);
		}
		printf("7:%d\r\n", pwm7_receive);

		memcpy(&radian, &RxMessage.Data[4], 4);           //  -1.57 - 0 - 1.57
		radian = -radian + 1.57f;
		int_temp = (int)(radian * SERVO_RAD + 500);
		if (int_temp <= 500)
			int_temp = 500;
		else if (int_temp >= 2500)
			int_temp = 2500;
		if (int_temp != pwm6_receive)
		{
			pwm6_receive = int_temp;
			servo_set(ID6, pwm6_receive);
		}
		printf("6:%d\r\n", pwm6_receive);
		break;

//	case 0x5f:	//0x0be0>>5
//		memcpy(&float_temp, &RxMessage.Data[4], 4);
//		if (fabs(float_temp - 1) <= 0.01)
//		{
//			EXTI_GenerateSWInterrupt(EXTI_Line1);
//		}
//		break;

	default:
		break;
	}
	LED0 = ~LED0;
}
#endif

/**
  * @brief  CAN1发送一组数据
  * @param  len:数据长度(最大为8)	
  *         msg:数据指针,最大为8个字节
  * @retval 0,成功;
  *  		其他,失败;
  */

u8 CAN1_Send_Msg(u8* msg, u8 len, u32 id)
{
	u8 mbox;
	u16 i = 0;
	CanTxMsg TxMessage;
	TxMessage.StdId = id;	 // 设置标准标识符
	TxMessage.ExtId = 0x00;	 // 设置扩展标示符（29位）
	TxMessage.IDE = CAN_Id_Standard;		  // 使用标准帧
	TxMessage.RTR = CAN_RTR_Data;		  // 消息类型为数据帧，一帧最多8个字节（8*8=64位），一个字节8位（一个数据）
	TxMessage.DLC = len;					 // 发送一帧信息（8个字节）
	for (i = 0; i < len; i++)
		TxMessage.Data[i] = msg[i];
	for (i = len; i < 8; i++)
		TxMessage.Data[i] = 0;
	mbox = CAN_Transmit(CAN1, &TxMessage);
	i = 0;
	while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0XFFF))
		i++;	//等待发送结束
	if (i >= 0XFFF)
		return 1;
	return 0;
}

/**
  * @brief  CAN1接收数据,现无用
  * @param  buf:数据缓存区
  * @retval 0,无数据被收到
  *         其他,接收的数据长度
  */

//u8 CAN1_Receive_Msg(u8 *buf)
//{		   		   
// 	u32 i;
//	CanRxMsg RxMessage;
//    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
//    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
//    for(i=0;i<RxMessage.DLC;i++)
//		buf[i]=RxMessage.Data[i]; 
//	delay_ms(100);
//	LED0 = ~LED0;
//	return RxMessage.StdId;	
//}

/**
  * @brief  CAN2初始化函数
  * @param  tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
			tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
			tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
			brp :波特率分频器.范围:1~1024;  tq=(brp)*tpclk1
			波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp)
			FilterID:标识符ID
  * @retval void
  */
  
void CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
  	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef CAN_InitStructure;
  	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
#if CAN2_RX0_INT_ENABLE 
   	NVIC_InitTypeDef NVIC_InitStructure;
#endif

    //使能相关时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//使能CAN2时钟	
	
    //初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PB12,PB13
	
	  //引脚复用映射配置
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2); //GPIOB12复用为CAN2
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); //GPIOB13复用为CAN2
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM = DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM = DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM = DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART = ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM = DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP = DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode = mode;	 //模式设置 
  	CAN_InitStructure.CAN_SJW = tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1 = tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2 = tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler = brp;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN2, &CAN_InitStructure);   // 初始化CAN1 
    
		//配置过滤器
		//设置过滤器0
		
	CAN_SlaveStartBank(14);

	CAN_FilterInitStructure.CAN_FilterNumber = 14;	//过滤器0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽 

	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
	
//	CAN_FilterInitStructure.CAN_FilterNumber = 14;	//过滤器0
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽 
//	
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x01e0>>5)<<5;	//32位ID
//	CAN_FilterInitStructure.CAN_FilterIdLow =CAN_Id_Standard | CAN_RTR_Data;

//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =(0x02e0>>5)<<5; //32位MASK
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
//	
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器0
//	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
//  
//	//设置过滤器1
//	CAN_FilterInitStructure.CAN_FilterNumber = 15;	//过滤器1
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽 
//	
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x03e0 >> 5)<<5;	//32位ID
//	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x04e0 >> 5)<<5; //32位MASK
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
//	
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器1关联到FIFO0
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器1
//	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
//	
//	//设置过滤器2
//	CAN_FilterInitStructure.CAN_FilterNumber = 16;	//过滤器2
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽 
//	
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0201)<<5;	//32位ID
//	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0202)<<5; //32位MASK
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
//	
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器2关联到FIFO0
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器2
//	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
//	
//	//设置过滤器3
//	CAN_FilterInitStructure.CAN_FilterNumber = 17;	//过滤器3
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽 
//	
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0203)<<5;	//32位ID
//	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0204)<<5; //32位MASK
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
//	
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器3关联到FIFO0
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器3
//	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
//	
//	//设置过滤器4
//	CAN_FilterInitStructure.CAN_FilterNumber = 18;	//过滤器4
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽 
//	
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0205)<<5;	//32位ID
//	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0206)<<5; //32位MASK
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
//	
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器4关联到FIFO0
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器4
//	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
	
#if CAN2_RX0_INT_ENABLE 
	
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
#endif
}   
 


/**
  * @brief  CAN1发送一组数据
  * @param  len:数据长度(最大为8)	
  *         msg:数据指针,最大为8个字节
  * @retval 0,成功;
  *  		其他,失败;
  */

u8 CAN2_Send_Msg(u8* msg, u8 len, uint32_t id)
{
	u8 mbox;
	u16 i = 0;
	CanTxMsg TxMessage;
	TxMessage.StdId = id;	 // 标准标识符
	TxMessage.ExtId = 0x00;	 // 设置扩展标示符（29位）
	TxMessage.IDE = CAN_Id_Standard;		  // 标准帧
	TxMessage.RTR = CAN_RTR_Data;		  // 消息类型为数据帧，一帧8位（一个数据，字节）
	TxMessage.DLC = len;							 // 发送一帧信息
	for (i = 0; i < len; i++)
		TxMessage.Data[i] = msg[i];
	mbox = CAN_Transmit(CAN2, &TxMessage);
	i = 0;
	while ((CAN_TransmitStatus(CAN2, mbox) == CAN_TxStatus_Failed) && (i < 0XFFF))
		i++;	//等待发送结束
	if (i >= 0XFFF)return 1;
	return 0;
}

/**
  * @brief  CAN2接收数据,现无用
  * @param  buf:数据缓存区
  * @retval 0,无数据被收到
  *         其他,接收的数据长度
  */

//u8 CAN2_Receive_Msg(u8 *buf)
//{		   		   
// 	u32 i;
//	CanRxMsg RxMessage;
//    if( CAN_MessagePending(CAN2,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
//    CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//读取数据	
//    for(i=0;i<RxMessage.DLC;i++)
//    buf[i]=RxMessage.Data[i];  
//	return RxMessage.DLC;	
//}


/**
  * @brief  CAN2中断服务函数
  * @param  void
  * @retval void
  */

#if CAN2_RX0_INT_ENABLE 

void CAN2_RX0_IRQHandler(void)
{
	u8 i;
	CanRxMsg RxMessage;
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);

	switch (RxMessage.StdId)
	{
	case 0x0201:
		for (i = 0; i < 8; ++i)
		{
			//储存原始信息
			Motor[0].Original_Data[i] = RxMessage.Data[i];
		}
		//进行消息的转换
		Data_Convert(0);
//		printf("CAN2");
//		Motor_Length_Calc();
		break;
	}
	
//      for(i=0; i < 8; ++i){
//        
//        Motor[RxMessage.StdId%0x10-1].Original_Data[i] = RxMessage.Data[i];
//      }
//      Data_Convert(RxMessage.StdId%0x10-1);
//	  Motor_Length_Calc();
}

#endif











