#include "main.h"
#include "PSINS.h"
#include "ahrsapp.h"

void INSUartOut(const CSINS &sins);
CVect3 wm, vm, att, gpsvn, gpspos, eb, db;
int timcnt1 = 0, timcnt2 = 0, iter;
double ts = 0.01, yaw0, time_consume = 0.0, time_consume1 = 0;

int main(void)
{
	mcu_init();
	CAHRSApp kf(0.0);

//	eb = CVect3(-4.26,1.1,-0.85);  // ÍÓÂÝÁãÆ« deg/s
	eb = CVect3(-2.2,-0.5,0);  // ÍÓÂÝÁãÆ« deg/s
	db = CVect3(0.0,0.0,-0.1/9.8);  // ¼Ó¼ÆÆ«Öµ m/s
	
	while(1)
	{
		if(GAMT_OK_flag==1)
		{
			timcnt1 = TIM2->CNT;
			GAMT_OK_flag = 0;
			wm = (*(CVect3*)mpu_Data_value.Gyro-eb)*(glv.dps*ts);
			vm = (*(CVect3*)mpu_Data_value.Accel-db)*(glv.g0*ts);
			if(GPS_OK_flag)
			{
				GPS_OK_flag = 0;
				if(gps_Data_value.GPS_numSV>6&&gps_Data_value.GPS_pDOP<5.0f)
				{
					gpsvn = *(CVect3*)gps_Data_value.GPS_Vn; gpspos = *(CVect3*)gps_Data_value.GPS_Pos;
				}
			}
			else
			{
				gpsvn = gpspos = O31;
			}
			kf.Update(wm, vm, gpsvn, gpspos, ts);  iter = kf.iter;
			INSUartOut(kf.sins);
		}
	}
}

void INSUartOut(const CSINS &sins)
{
	att = q2att(sins.qnb); 
	
	out_data.Att[0] = att.i/glv.deg; 
	out_data.Att[1] = att.j/glv.deg; 
	out_data.Att[2] = CC180toC360(att.k)/glv.deg;
	
	out_data.Vn[0] = sins.vn.i; out_data.Vn[1] = sins.vn.j; out_data.Vn[2] = sins.vn.k;
	int deg;
	deg = sins.pos.j/glv.deg;
	out_data.Pos[0] = deg;  out_data.Pos[1] = sins.pos.j/glv.deg-deg;
	deg = sins.pos.i/glv.deg;
	out_data.Pos[2] = deg;  out_data.Pos[3] = sins.pos.i/glv.deg-deg;
	out_data.Pos[4] = sins.pos.k;		
	timcnt2 = TIM2->CNT;
	time_consume = (timcnt2 - timcnt1)*0.1;  // ms
	if(time_consume1<time_consume) time_consume1 = time_consume;
	if(fmod(sins.tk,5.0)<ts) time_consume1 = 0.0;
//	out_data.Pos[4] = (iter+3) + time_consume/100.0;	
}
