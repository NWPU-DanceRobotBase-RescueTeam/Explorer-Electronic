/**************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力
**************************************************************************/
#include <math.h>
#include <string.h>
#include "can.h"
#include "delay.h"
#include "usart.h"
#include "Motor_Control.h"
#include "MAXON_Motor.h"
#include "pot.h"
#include "tim.h"

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
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

#if CAN1_RX0_INT_ENABLE 
	NVIC_InitTypeDef  NVIC_InitStructure;
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


	//配置过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = 0;	//过滤器0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit; 	//32位宽 

	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x01e0 >> 5) << 5;	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = (0x0be0 >> 5) << 5;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x02e0 >> 5) << 5; //32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化

	CAN_FilterInitStructure.CAN_FilterNumber = 1;	//过滤器1
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽 

	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x03e0 >> 5) << 5;	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x04e0 >> 5) << 5; //32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器1
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化

#if CAN1_RX0_INT_ENABLE

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//FIFO0消息挂号中断允许.		    

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
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
	CanRxMsg RxMessage;
	static float Goal_Speed = 0;
	static float Goal_Position = 0;
	static u16 a = 20;
//	times_numb = 0;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	if (a++ > 200) a = 20;
	switch (RxMessage.StdId)
	{
	// 接受主控消息
	case 0x01e0 >> 5: //主履带设定速度
	{
		memcpy(&Goal_Speed, &RxMessage.Data[0], 4);
		printf("左主履带设定速度为:%.2f",Goal_Speed );
		printf("\r\n");
		Goal_Speed = Goal_Speed * 300;
		MAXON_MOTOR[0] = (int32_t)Goal_Speed;
//		printf("左主履带设定速度为:%d",MAXON_MOTOR[0] );
//		printf("\r\n");

		memcpy(&Goal_Speed, &RxMessage.Data[4], 4);
		printf("右主履带设定速度为:%.2f",Goal_Speed );
		printf("\r\n");
		Goal_Speed = -Goal_Speed * 300;
		MAXON_MOTOR[1] = (int32_t)Goal_Speed;
//		printf("右主履带设定速度为:%d",MAXON_MOTOR[1] );
//		printf("\r\n");
		break;
	}
//	case 0x02e0 >> 5:
//	{
//		memcpy(&Goal_Speed, RxMessage.Data, 4);
//		if (fabs(Goal_Speed - 1) <= 0.1)
//			Subtrack_Rest();
//		printf("111\r\n");
//		break;
//	}
	case 0x03e0 >> 5: 					//左前副履带、右前副履带角度
	{
		memcpy(&Goal_Position, RxMessage.Data, 4);
		Goal_Position = Goal_Position * 50 * 1.2f;
		if (Goal_Position < 0)
			Goal_Position = 0;//反向限位
		M3508[0].PID.Goal_Position = Left_Front - (int32_t)Goal_Position;
		if (M3508[0].PID.Goal_Position < 40)
			M3508[0].PID.Goal_Position = 2;//正向最大限位
		M3508[0].Start_Flag = 1;
		printf("左前副履带设定角度为:%d",M3508[0].PID.Goal_Position);
		printf("\r\n");

		memcpy(&Goal_Position, RxMessage.Data + 4, 4);
		Goal_Position = Goal_Position * 50 * 1.2f;
		if (Goal_Position < 0)
			Goal_Position = 0;//反向限位
		M3508[2].PID.Goal_Position = Right_Front + (int32_t)Goal_Position;
		if (M3508[2].PID.Goal_Position > 288)
			M3508[2].PID.Goal_Position = 288;//正向最大限位
		M3508[2].Start_Flag = 1;
		printf("右前副履带设定角度为:%d",M3508[2].PID.Goal_Position);
		printf("\r\n");

		break;
	}
	case 0x04e0 >> 5: 									//左后副履带、右后副履带角度
	{
		memcpy(&Goal_Position, RxMessage.Data, 4);
		Goal_Position = Goal_Position * 50 * 1.2f;
		if (Goal_Position < 0)
			Goal_Position = 0;//反向限位
		M3508[1].PID.Goal_Position = Left_Rear + (int32_t)Goal_Position;
		if (M3508[1].PID.Goal_Position > 288)
			M3508[1].PID.Goal_Position = 288;//正向最大限位
		M3508[1].Start_Flag = 1;
		printf("左后副履带设定角度为:%d",M3508[1].PID.Goal_Position);
		printf("\r\n");


		memcpy(&Goal_Position, RxMessage.Data + 4, 4);
		Goal_Position = Goal_Position * 50 * 1.2f;
		if (Goal_Position < 0)
			Goal_Position = 0;//反向限位
		M3508[3].PID.Goal_Position = Right_Rear - (int32_t)Goal_Position;
		if (M3508[3].PID.Goal_Position < 40)
			M3508[3].PID.Goal_Position = 2;//正向最大限位
		M3508[3].Start_Flag = 1;
		printf("右后副履带设定角度为:%d",M3508[3].PID.Goal_Position);
		printf("\r\n");
		break;
	}
//		case 0x0be0 >> 5 :		
//		{
//			memcpy(&shut,RxMessage.Data,4);	
//			if(fabs(shut-0)>=0.1&&a>=20)
//			{
//				relay_OFF();
//				a = 0;
//				times_numb1 =0;
//			}
//			break;
//		}
	}
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
	TxMessage.StdId = id;	 // 标准标识符为0
	TxMessage.ExtId = 0x00;	 // 设置扩展标示符（29位）
	TxMessage.IDE = CAN_Id_Standard;		  // 使用扩展标识符
	TxMessage.RTR = CAN_RTR_Data;		  // 消息类型为数据帧，一帧8位
	TxMessage.DLC = 8;							 // 发送两帧信息
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

u8 CAN1_Receive_Msg(u8* buf)
{
    u32 i;
    CanRxMsg RxMessage;

    if (CAN_MessagePending(CAN1, CAN_FIFO0) == 0)
        return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
    for (i = 0; i < RxMessage.DLC; i++)
        buf[i] = RxMessage.Data[i];
    return RxMessage.DLC;
}

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

void CAN2_Mode_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

#if CAN2_RX0_INT_ENABLE 
	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

	//使能相关时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTB时钟	                   											 

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
	CAN_Init(CAN2, &CAN_InitStructure);   // 初始化CAN2

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


/*		//配置过滤器
		//设置过滤器0
	CAN_FilterInitStructure.CAN_FilterNumber = 14;	//过滤器0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽

	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x01e0>>5)<<5;	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow =CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =(0x02e0>>5)<<5; //32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化

	//设置过滤器1
	CAN_FilterInitStructure.CAN_FilterNumber = 15;	//过滤器1
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽

	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x03e0 >> 5)<<5;	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x04e0 >> 5)<<5; //32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器1
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化

	//设置过滤器2
	CAN_FilterInitStructure.CAN_FilterNumber = 16;	//过滤器2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽

	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0201)<<5;	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0202)<<5; //32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器2
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化

	//设置过滤器3
	CAN_FilterInitStructure.CAN_FilterNumber = 17;	//过滤器2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽

	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0203)<<5;	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0204)<<5; //32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器2
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化

	//设置过滤器4
	CAN_FilterInitStructure.CAN_FilterNumber = 18;	//过滤器2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32位宽

	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0205)<<5;	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0206)<<5; //32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器2
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化	*/

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
  * @brief  CAN2发送一组数据
  * @param  len:数据长度(最大为8)	
  *         msg:数据指针,最大为8个字节
  * @retval 0,成功;
  *  		其他,失败;
  */

u8 CAN2_Send_Msg(u8* msg, u8 len, u32 id)
{
	u8 mbox;
	u16 i = 0;
	CanTxMsg TxMessage;
	TxMessage.StdId = id;	 // 标准标识符为0
	TxMessage.ExtId = 0x00;	 // 设置扩展标示符（29位）
	TxMessage.IDE = CAN_Id_Standard;		  // 使用扩展标识符
	TxMessage.RTR = CAN_RTR_Data;		  // 消息类型为数据帧，一帧8位
	TxMessage.DLC = 8;							 // 发送两帧信息
	for (i = 0; i < len; i++)
		TxMessage.Data[i] = msg[i];
	for (i = len; i < 8; i++)
		TxMessage.Data[i] = 0;
	mbox = CAN_Transmit(CAN2, &TxMessage);
	i = 0;
	while ((CAN_TransmitStatus(CAN2, mbox) == CAN_TxStatus_Failed) && (i < 0XFFF))
		i++;	//等待发送结束
	if (i >= 0XFFF)
		return 1;
	return 0;
}

/**
  * @brief  CAN2接收数据,现无用
  * @param  buf:数据缓存区
  * @retval 0,无数据被收到
  *         其他,接收的数据长度
  */

u8 CAN2_Receive_Msg(u8* buf)
{
    u32 i;
    CanRxMsg RxMessage;
    if (CAN_MessagePending(CAN2, CAN_FIFO0) == 0)
        return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//读取数据	
    for (i = 0; i < RxMessage.DLC; i++)
        buf[i] = RxMessage.Data[i];
    return RxMessage.DLC;
}

/**
  * @brief  CAN2中断服务函数,M3508数据回传
  * @param  void
  * @retval void
  */

#if CAN2_RX0_INT_ENABLE

void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	u8 i;
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
	switch (RxMessage.StdId)
	{
	case 0x0201:
	{
		for (i = 0; i < 8; ++i)
		{
			//储存原始信息
			M3508[0].Original_Data[i] = RxMessage.Data[i];
		}
		//进行消息的转换
		Data_Convert(0);
//		printf("0x201\r\n");
//		printf("Angle:%f\r\n",M3508[0].Angle);
//		Motor_Length_Calc();
//		M3508[0].Angle=current_angle[0];
		break;
	}
	case 0x0202:
	{
		for (i = 0; i < 8; ++i)
		{
			//储存原始信息
			M3508[1].Original_Data[i] = RxMessage.Data[i];
		}
		//进行消息的转换
		Data_Convert(1);
//		printf("0x202\r\n");
//		printf("Angle:%f\r\n",M3508[1].Angle);
//		Motor_Length_Calc();
//		M3508[1].Angle=current_angle[1];
		break;
	}
	case 0x0203:
	{
		for (i = 0; i < 8; ++i)
		{
			//储存原始信息
			M3508[2].Original_Data[i] = RxMessage.Data[i];
		}
		//进行消息的转换
		Data_Convert(2);
//		printf("0x203\r\n");
//		printf("Angle:%f\r\n",M3508[2].Angle);
//		printf("%d\r\n",M3508[2].Speed);
//		printf("0x202");
//		Motor_Length_Calc();
//		M3508[2].Angle=current_angle[2];
		break;
	}
	case 0x0204:
	{
		for (i = 0; i < 8; ++i)
		{
			//储存原始信息
			M3508[3].Original_Data[i] = RxMessage.Data[i];
		}
		//进行消息的转换
		Data_Convert(3);
//		printf("0x204\r\n");
//		printf("Angle:%f\r\n",M3508[3].Angle);
//		printf("0x202");
//		Motor_Length_Calc();
//		M3508[3].Angle=current_angle[3];
		break;
	}
	default:
		break;
	}
}
#endif





