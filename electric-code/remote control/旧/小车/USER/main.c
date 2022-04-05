#include "LED.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "fsm.h"
#include "Motor_Control.h"
#include "tim.h"
#include "debug.h"
#include "adc.h"
#include "pot.h"
#include "can.h"
#include "key.h"
#include "OLED.h"
//#include "LQ12864.h"


int main()
{
	u8 i;
	u8 send_message[8] = {0xff,0xff,0x02,0x0a,0x0a,0xfe,0x0d,0x0a};
	u8 adcxleft,adcyright,mode=0;
	int iadcxleft,iadcyright;
	char qian[5],hou[5],zuo[5],you[5];
	delay_init(168);
	LED_Init();
	Adc_Init();
	KEY_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	TIM5_Int_Init(1000-1, 8400-1);   //0.1s
	
   LCD_init();
	LCD_CLS();            //OLEDÇåÆÁ   
//	TIM3_Int_Init(50, 7000-1);
//	Subtrack_Rest();//¸±ÂÄ´ø¸´Î»
	while(1)
	{
//		adcxleft = limiting(-(Get_Adc_Average(ADC_Channel_13,5)-2048)/200,100);//×óÒ¡¸ËxÖá      -10µ½10
////	adcyleft = limiting((Get_Adc_Average(ADC_Channel_12,5)-2060)/200,100);//×óÒ¡¸ËyÖá
////	adcxright = limiting(-(Get_Adc_Average(ADC_Channel_4,5)-2050)/200,100);//ÓÒÒ¡¸ËxÖá
//		adcyright = limiting((Get_Adc_Average(ADC_Channel_5,5)-2048)/200,100);//ÓÒÒ¡¸ËyÖá
		delay_ms(100);
		adcxleft=tadcxleft;
		adcyright=tadcyright;
			
		if(key1==1)    //1µµ
		{
			adcxleft=adcxleft+100;
			adcyright=adcyright+100;
			mode=1;
		}
		else if(key3==1)    //2µµ
		{
			adcxleft=adcxleft*2+100;
			adcyright=adcyright*2+100;
			mode=2;
		}
		else if(key4==1)    //3µµ
		{
			adcxleft=adcxleft*3+100;
			adcyright=adcyright*3+100;
			mode=3;
		}
		else           //0µµ
		{
			adcxleft = 100;//×óÒ¡¸ËxÖá
			adcyright = 100;//ÓÒÒ¡¸ËyÖá
			mode=0;
		}
		
	 iadcxleft=(int)adcxleft-100;
		iadcyright=(int)adcyright-100;
		
		if(iadcxleft>0)
		{
			sprintf(you,"%d ",iadcxleft);
			sprintf(zuo,"%d ",0);
		}
		else
		{
			sprintf(zuo,"%d ",-iadcxleft);
			sprintf(you,"%d ",0);
		}
		if(iadcyright>0)
		{
			sprintf(qian,"%d ",iadcyright);
			sprintf(hou,"%d ",0);
		}
		else
		{
			sprintf(hou,"%d ",-iadcyright);
			sprintf(qian,"%d ",0);
		}       
//       LCD_P8X16Str(2,1,"123456789",BLUE,BLACK);         //ÏÔÊ¾Ó¢ÎÄ8*16×Ö·û´®
		LCD_P16X16Str(1,1,"Ç°",YELLOW,BLACK);     //ÏÔÊ¾ºº×Ö×Ö·û´®
		LCD_P8X16Str(4,1,":",WHITE,BLACK);
		LCD_P8X16Str(5,1,qian,WHITE,BLACK);
    LCD_P16X16Str(4,1,"ºó",YELLOW,BLACK); 
		LCD_P8X16Str(10,1,":",WHITE,BLACK);
		LCD_P8X16Str(11,1,hou,WHITE,BLACK);
		LCD_P16X16Str(1,3,"×ó",YELLOW,BLACK); 
		LCD_P8X16Str(4,3,":",WHITE,BLACK);
		LCD_P8X16Str(5,3,zuo,WHITE,BLACK);
		LCD_P16X16Str(4,3,"ÓÒ",YELLOW,BLACK); 
		LCD_P8X16Str(10,3,":",WHITE,BLACK);
		LCD_P8X16Str(11,3,you,WHITE,BLACK);
		
		
		LCD_P16X16Str(2,4,"µµÎ»",YELLOW,BLACK); 
		LCD_P8X16Str(8,4,":",WHITE,BLACK);
		
    if(mode==0)
			LCD_P16X16Str(5,4,"ÎÞ",WHITE,BLACK);
		else if(mode==1)
			LCD_P16X16Str(5,4,"µÍ",YELLOW,BLACK);
		else if(mode==2)
			LCD_P16X16Str(5,4,"ÖÐ",ORANGE,BLACK);
		else if(mode==3)
			LCD_P16X16Str(5,4,"¸ß",RED,BLACK);
		
		if(key5==0)
		{
			delay_ms(5);
			if(key5==0)
			{
				delay_ms(50);
				send_message[1]=0x01;
				for(i=0;i<8;i++)
				{
					putchar(send_message[i]);
				}
				delay_ms(50);
				send_message[1]=0xff;
		  }
		}
		else if(key6==0)
		{
			delay_ms(5);
			if(key6==0)
			{
				delay_ms(50);
				send_message[1]=0x13;
				for(i=0;i<8;i++)
				{
					putchar(send_message[i]);
				}
				delay_ms(50);
				send_message[1]=0xff;
		  }
		}
		else if(key7==0)
		{
			delay_ms(5);
			if(key7==0)
			{
				send_message[1]=0x25;
				delay_ms(50);
				for(i=0;i<8;i++)
				{
					putchar(send_message[i]);
				}
				delay_ms(50);
				send_message[1]=0xff;
		  }
		}
		else if(key8==0)
		{
			delay_ms(5);
			if(key8==0)
			{
				delay_ms(50);
				send_message[1]=0x37;
				for(i=0;i<8;i++)
				{
					putchar(send_message[i]);
				}
				delay_ms(50);
				send_message[1]=0xff;
		  }
		}
		

		
		
//		if(adcxleft==0x0d)
//			adcxleft=0x0c;
		send_message[3] = adcxleft;
		send_message[4] = adcyright;
		
		for(i=0;i<8;i++)
		{
			putchar(send_message[i]);
		}
//		printf("adcxleft = %d   ",adcxleft);
//		printf("adcxright = %d\r\n",adcxright);
//		printf("adcyleft = %d   ",adcyleft);
//		printf("adcyright = %d\r\n",adcyright);
//		debug();
//		if(adc_flag[0]==1)
//		{
//			get_pot_adc();
//			adc_angle_transform();
//		}
//	//		Report_Current();
//			Report_ADC();
//		delay_ms(1000);
//		GPIO_ResetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);
//		delay_ms(1000);
//		LED0=~LED0;
	}
}







