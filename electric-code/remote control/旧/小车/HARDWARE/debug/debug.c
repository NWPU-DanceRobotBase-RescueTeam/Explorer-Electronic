#include "debug.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "fsm.h"
#include "Motor_Control.h"
#include "tim.h"
#include "adc.h"
#include "pot.h"
#include "key.h"
u8 send_message[8] = {0xff,0xff,0x02,0x0a,0x0a,0xfe,0x0d,0x0a};

u8 i,receive_length=0,message_length,mode;

void Report_Current()   //监控电机速度
{
	printf("设定速度为：");
	for(i=0;i<2;i++)
	{
		printf("%d ",M3508[i].PID.Goal_Speed);
	}
	printf("\r\n");
	printf("实际速度为：");
	for(i=0;i<2;i++)
	{
		printf("%d ",M3508[i].Speed);
	}
	printf("\r\n");
	printf("\r\n");
}



void Report_ADC()				//监控电机角度
{
		printf("总flag=%d\r\n",adc_flag[0]);
		printf("\r\n");
		printf("左前角度= %f\r\n",angle[0]);
		printf("adc_flag=%d\r\n",adc_flag[1]);
		printf("\r\n");
		printf("左后角度= %f\r\n",angle[1]);
		printf("adc_flag=%d\r\n",adc_flag[2]);
		printf("\r\n");
		printf("右前角度= %f\r\n",angle[2]);
		printf("adc_flag=%d\r\n",adc_flag[3]);
		printf("\r\n");
		printf("右后角度 %f\r\n",angle[3]);
		printf("adc_flag=%d\r\n",adc_flag[4]);
		printf("\r\n");
}

void debug()
{
	u8 usart_receive_message[7],i;
	receive_length=Usart_Receivecheck();
	if(receive_length)
		{
			message_length=FSM(receive_length,usart_receive_message,&mode);
			switch(mode)
			{
				case 0XFF:                          //直行
					for(i=0;i<message_length;i=i+2)
					{
						//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
						if(i==0||i==4)  M3508[i/2].PID.Goal_Speed=((usart_receive_message[i]<<8)+usart_receive_message[i+1]);
						else 	M3508[i/2].PID.Goal_Speed=((usart_receive_message[i]<<8)+usart_receive_message[i+1]);
					}
					Report_Current();
					break;
					case 0XFE:                          //后退
					for(i=0;i<message_length;i=i+2)
					{
						//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
						if(i==0||i==4)  M3508[i/2].PID.Goal_Speed=-((usart_receive_message[i]<<8)+usart_receive_message[i+1]);
						else 	M3508[i/2].PID.Goal_Speed=-((usart_receive_message[i]<<8)+usart_receive_message[i+1]);
					}
					Report_Current();
					break;
					case 0XFD:												//向左旋转
					for(i=0;i<message_length;i=i+2)
					{
						//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
							M3508[i/2].PID.Goal_Speed=((usart_receive_message[i]<<8)+usart_receive_message[i+1]);
					}
					Report_Current();
					break;
					case 0XFC:												//向右旋转
					for(i=0;i<message_length;i=i+2)
					{
						//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
							M3508[i/2].PID.Goal_Speed=-((usart_receive_message[i]<<8)+usart_receive_message[i+1]);
					}
					Report_Current();
					break;
					case 0XFB:							//PID调节
					if(message_length==7)
					{
						for(i=0;i<7;i++)
						{
								M3508[i].PID.Kp = usart_receive_message[0]/10;
								M3508[i].PID.Ki = usart_receive_message[1]/10;
								M3508[i].PID.Kd = usart_receive_message[2]/10;
								M3508[1].Current_Limit = ((usart_receive_message[3]<<8)+usart_receive_message[4]);
								Error_Sum_set=((usart_receive_message[5]<<8)+usart_receive_message[6]);
						}
						printf("PID设置成功！\r\n");
					}
					else 
					{
						printf("未能成功设置PID！\r\n");
					}
					break;
					case 0XFA:												//设定正位置
					for(i=0;i<message_length;i=i+2)
					{
						//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
							if(i==0) M3508[2].PID.Goal_Position=207-((usart_receive_message[i]<<8)+usart_receive_message[i+1])*1.1111;
							else if(i==2) M3508[3].PID.Goal_Position=82+((usart_receive_message[i]<<8)+usart_receive_message[i+1])*1.1111;
							else if(i==4) M3508[4].PID.Goal_Position=95+((usart_receive_message[i]<<8)+usart_receive_message[i+1])*1.1111;
							else if(i==6) M3508[5].PID.Goal_Position=219-((usart_receive_message[i]<<8)+usart_receive_message[i+1])*1.1111;
							adc_flag[i/2+1]=1;
							printf("位置角度设置为：%d\r\n",M3508[i/2+2].PID.Goal_Position);
					}
					adc_flag[0]=1;
					break;
			}
			receive_length=0;
		}
}


