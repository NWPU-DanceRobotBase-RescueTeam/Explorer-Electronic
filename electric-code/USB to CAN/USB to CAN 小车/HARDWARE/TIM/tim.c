/**************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力
**************************************************************************/

#include "tim.h"


/**************************************************************************
函数功能：定时器3初始化
入口参数：arr:重装载值 , psc: 预分频值
返回  值：无
**************************************************************************/

void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM5,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
			 
}


/**************************************************************************
函数功能：定时器3中断服务函数
入口参数：无
返回  值：无
**************************************************************************/

void TIM5_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
			static u8 i=0,j=0;
      TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx更新中断标志 

//			//电位器的限位
			//get_pot_adc();
			//adc_angle_transform();
			
//			if(angle[0]>233) M3508[2].PID.Goal_Speed = 0;
//			if(angle[1]>233) M3508[3].PID.Goal_Speed = 0;
//			if(angle[2]>233) M3508[4].PID.Goal_Speed = 0;
//			if(angle[3]>233) M3508[5].PID.Goal_Speed = 0;
			
				Motor_Speed_Control(-M3508[0].PID.Goal_Speed, 0);
				Motor_Speed_Control(M3508[1].PID.Goal_Speed, 1);
//			Motor_Speed_Control(M3508[2].PID.Goal_Speed, 2);
//			Motor_Speed_Control(M3508[3].PID.Goal_Speed, 3);
//			Motor_Speed_Control(M3508[4].PID.Goal_Speed, 4);
//			Motor_Speed_Control(M3508[5].PID.Goal_Speed, 5);

			//printf("M1_Goal_Speed:%d\r\n",M3508[0].PID.Goal_Speed);
			//printf("M2_Goal_Speed:%d\r\n",M3508[1].PID.Goal_Speed);
			
//				for(i = 0; i < 1; ++i)
//				{
//					// 检测突变
//					if((M3508[i].Angle - M3508[i].Last_Angle) < -4095)
//					{
//						M3508[i].Last_Angle = M3508[i].Angle;
//						// 已经一圈了
//						M3508[i].Round += 360;
//					}
//					else if((M3508[i].Angle - M3508[i].Last_Angle) > 4095)
//					{
//						M3508[i].Last_Angle = M3508[i].Angle;
//						// 已经一圈了
//						M3508[i].Round -= 360;
//					}
//				}
//					M3508[i].Motor_Length = M3508[i].Round + M3508[i].Angle * 0.043951;
				
				
//					i++;
//					if(i==100)
//					{					
//						printf("angle= %f\r\n",angle[0]);
//						printf("M1_Angle:%d\r\n",M3508[0].Angle);
//						printf("M1_Goal_Position:%d\r\n",M3508[0].PID.Goal_Position);
//						printf("adc_flag:%d\r\n",adc_flag[0]);
//						printf("\r\n");
//					i=0;
//					}
			if(adc_flag[0])
			{			
			for(i=1;i<5;i++)
				{
						if(adc_flag[i]==1)
							adc_flag[i]=Angle_Position_Control(M3508[i+1].PID.Goal_Position,i+1);
						else if(adc_flag[i]==0)
							Motor_Speed_Control(0,i+1);
				}		
//			adc_flag[1]=Angle_Position_Control(M3508[0].PID.Goal_Position,0);
//			adc_flag[2]=Angle_Position_Control(M3508[1].PID.Goal_Position,1);
//			adc_flag[3]=Angle_Position_Control(M3508[2].PID.Goal_Position,2);
//			adc_flag[4]=Angle_Position_Control(M3508[3].PID.Goal_Position,3);
			adc_flag[0]=(adc_flag[1]||adc_flag[2]||adc_flag[3]||adc_flag[4]);
//			CAN_Send_Control_Value();
			}
			else
			{
			Motor_Speed_Control(0,2);
			Motor_Speed_Control(0,3);
			Motor_Speed_Control(0,4);
			Motor_Speed_Control(0,5);				
			}
			CAN2_Send_Control_Value();
			//
//			if(i>9)
//			{
//				printf("%d,%d\r\n",M3508[5].PID.Goal_Speed,M3508[5].Speed);
//				i=0;
//			}
//			else i++;
			//printf("%d,%d,%d,%d\n",M3508[0].PID.Goal_Speed,M3508[0].Speed,M3508[1].PID.Goal_Speed,M3508[1].Speed);
//			if(i>9) 
//			{
//				printf("M1_Speed:%d\r\n",M3508[0].Speed);
//				printf("M1_Goal_Speed:%d\r\n",M3508[0].PID.Goal_Speed);
//				i=0;
//			}
//			else i++;
			//Motor_Info_Send();
			
//		send_can_data(angle[0],angle[1],(0x11e0>>5));
//		delay_ms(10);  //以前：3   现在：10    必须加延时，CAN通信才稳定
//		send_can_data(angle[2],angle[3],(0x12e0>>5));
		  // LED1=!LED1;
			//Motor_Info_Send();//
		}
	
}

