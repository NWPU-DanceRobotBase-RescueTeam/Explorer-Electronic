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
	TIM5_Int_Init(2000-1, 8400-1);   //0.2s
	IWDG_Init(7,400);
	
	Show_Start();//¿ª»ú¶¯»­
				
	while(1)
	{
		IWDG_Feed();
	}
}







