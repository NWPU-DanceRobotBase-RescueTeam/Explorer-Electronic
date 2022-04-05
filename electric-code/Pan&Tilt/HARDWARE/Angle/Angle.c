#include "Angle.h"
#include "IMU.h"
#include <math.h>


float Angle[3];    
ANGLE RollAngle,PitchAngle;   

float roll,pitch;
float RollLast,PitchLast;		
extern volatile u8 Roll[11];
extern volatile u8 Pitch[11];
void Angle_Init()         //角度数据结构体的初始化
{
	RollAngle.val.zero = 0;
	RollAngle.val.fir = 0;
	RollAngle.val.sec = 0;
	RollAngle.realval = 0;
	RollAngle.pid.Kp = 0.06;//回传速率100HZ
	RollAngle.pid.Ki = 0;
	RollAngle.pid.Kd = 0.002;
	
	PitchAngle.val.zero = 0;
	PitchAngle.val.fir = 0;
	PitchAngle.val.sec = 0;
	PitchAngle.realval = 0;
	PitchAngle.pid.Kp = 0.06;
	PitchAngle.pid.Ki = 0;
	PitchAngle.pid.Kd = 0.002;
}

void AngleIsFinite()		
{

			if(isfinite(Angle[ROLL]))		
			{
				RollLast=Angle[ROLL];
			}
			else	Angle[ROLL]=RollLast;

			if(isfinite(Angle[PITCH]))		
			{
				PitchLast=Angle[PITCH];
			}
			else	Angle[PITCH]=PitchLast;
			
}

void AngleUpdate()		
{
			u8 t=0;
			u16 sum=0;
			if(fabs(RollAngle.realval)>35)		
			{
				if(RollAngle.realval>0)	
					RollAngle.realval=35;
				else 
					RollAngle.realval=-35;
			}
			roll = (-RollAngle.realval + 150.0f )/300.0f*1024;	
			Roll[6] =(int) roll % 256;
			Roll[7] =(int) roll / 256;
			sum=0;
			for(t=2;t<=9;t++)	
				sum = sum + Roll[t];		//计算校验位
			Roll[10]=(uint8_t)(~(sum));	
			
			pitch = (PitchAngle.realval + 150.0f )/300.0f*1024;
			Pitch[6] =(int)  pitch % 256;
			Pitch[7] =(int)  pitch / 256;
			sum=0;
			for(t=2;t<=9;t++)
				sum = sum + Pitch[t];		//计算校验位
			Pitch[10]=(uint8_t)(~(sum));			
}

void AngleControl(float value,ANGLE *Angle)
{
	float p = 1,d = 1;
	
	Angle->val.sec = (Angle->val.fir);		
	Angle->val.fir = (Angle->val.zero);

	if(fabs(value-(Angle->val.zero)) > 10.0)   //相差过大
		Angle->val.zero = Angle->val.fir;   //???
	else 
		Angle->val.zero = value;
	
	if(Angle->val.zero>180)  // -180~180
		Angle->val.zero = Angle->val.zero - 360;
	if(Angle->val.zero<-180) 
		Angle->val.zero = Angle->val.zero + 360;
	
	if(fabs(Angle->val.zero)>2.0)   
	{
		p = 1;
		d = 1.3;
	}
	
	if(fabs(Angle->val.zero) < 0.2) 
		Angle->val.zero = 0 ;   
	
	Angle->realval = Angle->realval 
	                    + p*Angle->pid.Kp * Angle->val.zero
	                    + d*Angle->pid.Kd * (Angle->val.zero - Angle->val.fir);  
	
	if(fabs(Angle->realval) > 85)                     
	{
		if(Angle->realval> 0) 
			Angle->realval = 85;
		else 
			Angle->realval = -85;
	}
	
}
