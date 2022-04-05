#include "LED.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "tim.h"
#include "adc.h"
#include "key.h"
#include "OLED.h"
#include "iwdg.h"
//#include "LQ12864.h"

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	delay_init(168);
	LED_Init();
	Adc_Init();
	KEY_Init();
	LCD_init();
	IWDG_Init(7,400);
	TIM5_Int_Init(2000-1, 8400-1);   //0.1s
	Show_Start();//¿ª»ú¶¯»­
	
	base.Float = 1.57;  first_arm.Float = 0.00;  second_arm.Float = 0.00;  small_rotation.Float = 1.57;  small_oscillation.Float = 1.09;  
	paw_open_close.Float = 1.41;  paw_rotation.Float = 1.57;
	reset = 0, spin = 0;
	
	while(1)
	{
		IWDG_Feed();
		Key_Scan();
	}
}







