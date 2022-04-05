#include "main.h"
#include "PSINS.h"
#include "kfapp.h"

CMahony ahrs;
CVect3 wm, vm, att;
int timcnt1 = 0, timcnt2 = 0;

int main(void)
{
	mcu_init();
	ahrs = CMahony(4.0);
	double ts = 0.01, tk = 0.0;
		 
	while(1)
	{
		if(GAMT_OK_flag==1)
		{
			timcnt1 = TIM2->CNT;
			GAMT_OK_flag = 0;
			tk += ts;
			wm.i = mpu_Data_value.Gyro[0];  wm.j = mpu_Data_value.Gyro[1];  wm.k = mpu_Data_value.Gyro[2];
			vm.i = mpu_Data_value.Accel[0]; vm.j = mpu_Data_value.Accel[1]; vm.k = mpu_Data_value.Accel[2]; 
			ahrs.Update(wm, vm, O31, ts);
			att = q2att(ahrs.qnb); 
			out_data.Att[0] = att.i/glv.deg; out_data.Att[1] = att.j/glv.deg; out_data.Att[2] = CC180toC360(att.k)/glv.deg;
			timcnt2 = TIM2->CNT;
		}	
	}
}
