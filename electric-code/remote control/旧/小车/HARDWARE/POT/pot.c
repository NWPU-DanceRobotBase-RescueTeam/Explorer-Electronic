/**************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力
**************************************************************************/
#include "pot.h"

float v[4]={0};
float angle[4]={0};
u8 adc_flag[5];


/**************************************************************************
函数功能：电位器电压采集
入口参数：无
返回  值：无
**************************************************************************/
void get_pot_adc()
{
	  u16 adc[4]={0,0,0,0};
		u8 i;
		adc[0]=Get_Adc_Average(ADC_Channel_0,5);
		adc[1]= (ADC_Channel_1,5);
		adc[2]=Get_Adc_Average(ADC_Channel_2,5);
		adc[3]=Get_Adc_Average(ADC_Channel_3,5);
			for(i=0;i<4;i++)
		{
			v[i]=(float)(adc[i])*(3.3/4096);
			adc[i]=v[i];
		}
		//printf("%f\r\n",v[0]);
}


/**************************************************************************
函数功能：电位器角度转换
入口参数：无
返回  值：无
**************************************************************************/

void adc_angle_transform()
{
	u8 i;
	for(i=0;i<4;i++)
	{
		angle[i]=v[i]*300/3.3;
		M3508[i+2].Angle=angle[i];
	}
	printf("M1_Angle=%f\r\n",M3508[4].Angle);
	printf("\r\n");
	//printf("angle= %f\r\n",angle[0]);
}

/**************************************************************************
函数功能：副履带复位
入口参数：无
返回  值：无
**************************************************************************/
void Subtrack_Rest()
{
	get_pot_adc();
	adc_angle_transform();
	M3508[2].PID.Goal_Position=207;//左前初始化角度
	M3508[3].PID.Goal_Position=82;//左后初始化角度
	M3508[4].PID.Goal_Position=95;//右前初始化角度
	M3508[5].PID.Goal_Position=219;//右后初始化角度
	adc_flag[0]=1;
	adc_flag[1]=1;
	adc_flag[2]=1;
	adc_flag[3]=1;
	adc_flag[4]=1;
}

/**************************************************************************
函数功能：副履带位置控制
入口参数：目标角度（0-300），电机ID
返回  值：0 达到目标位置，1 未达到目标位置
**************************************************************************/

u8 Angle_Position_Control(float Goal_Position,u8 Motor_ID)
{
	u8 Return_Flag = 1;
	float Angle_Position_error;
	int	Set_speed,kp=120;
	Angle_Position_error=angle[Motor_ID-2]-Goal_Position;
	Set_speed=fabs(Angle_Position_error)*kp;
	if(fabs(Angle_Position_error) > 5)//恒定速度区
	{
		if(angle[Motor_ID-2]< Goal_Position){
			// 速度的正负号需要调
			switch(Motor_ID)
			{
				case 2:
				Motor_Speed_Control(2040, Motor_ID);
				break;
				case 3:
				Motor_Speed_Control(-2000, Motor_ID);
				break;
				case 4:
				Motor_Speed_Control(-2000, Motor_ID);
				break;
				case 5:
				Motor_Speed_Control(2002, Motor_ID);
				break;
			}
		}
		else{
			// 速度的正负号需要调
			switch(Motor_ID)
			{
				case 2:
				Motor_Speed_Control(-2040, Motor_ID);
				break;
				case 3:
				Motor_Speed_Control(2000, Motor_ID);
				break;
				case 4:
				Motor_Speed_Control(2000, Motor_ID);
				break;
				case 5:
				Motor_Speed_Control(-2002, Motor_ID);
				break;
			}
		}
		// 未达到目标位置
		Return_Flag = 1;
	}
	else	if(fabs(Angle_Position_error) > 0.5)//速度缓冲区
	{
		if(angle[Motor_ID-2]< Goal_Position){
			// 速度的正负号需要调
			switch(Motor_ID)
			{
				case 2:
				Motor_Speed_Control(Set_speed, Motor_ID);
				break;
				case 3:
				Motor_Speed_Control(-Set_speed, Motor_ID);
				break;
				case 4:
				Motor_Speed_Control(-Set_speed, Motor_ID);
				break;
				case 5:
				Motor_Speed_Control(Set_speed, Motor_ID);
				break;
			}
		}
		else{
			// 速度的正负号需要调
			switch(Motor_ID)
			{
				case 2:
				Motor_Speed_Control(-Set_speed, Motor_ID);
				break;
				case 3:
				Motor_Speed_Control(Set_speed, Motor_ID);
				break;
				case 4:
				Motor_Speed_Control(Set_speed, Motor_ID);
				break;
				case 5:
				Motor_Speed_Control(-Set_speed, Motor_ID);
				break;
			}
		}
		// 未达到目标位置
		Return_Flag = 1;
	}
	else{
		Motor_Speed_Control(0, Motor_ID);
		// 已达到目标位置
		Return_Flag = 0;
	}
	//CAN_Send_Control_Value();
	return Return_Flag;
}
