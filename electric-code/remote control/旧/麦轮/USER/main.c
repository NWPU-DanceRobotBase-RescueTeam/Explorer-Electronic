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

void start_animation()
{
		LCD_P16X16Str(2,1,"Ð¡Ô®Ð¡Ô®",RED,BLACK);
		LCD_P16X16Str(0,2,"Ò»Â·»ð»¨´øÉÁµç£¡",CYAN,BLACK);
		LCD_P16X16Str(0,4,"¿ª»úÖÐ",YELLOW,BLACK); 
		delay_ms(300);
		LCD_P8X16Str(6,4,"..",YELLOW,BLACK);
		delay_ms(300);
		LCD_P8X16Str(8,4,"..",YELLOW,BLACK);
		delay_ms(300);
		LCD_P8X16Str(10,4,"..",YELLOW,BLACK);
		delay_ms(200);
		LCD_P8X16Str(12,4,"..",YELLOW,BLACK);
		delay_ms(200);
		LCD_P8X16Str(14,4,"..",YELLOW,BLACK);
		delay_ms(200);	
		LCD_CLS();   //OLEDÇåÆÁ
}

int main()
{
	u8 i;
	u8 send_message1[8] = {0xff,0xfa,0x02,0x0a,0x0a,0xfe,0x0d,0x0a};
  u8 send_message2[8] = {0xff,0xfc,0x02,0x0a,0x0a,0xfe,0x0d,0x0a};
  u8 send_message3[8] = {0xff,0xf0,0x02,0x0a,0x0a,0xfe,0x0d,0x0a};
	u8 send_message[8] = {0xff,0xff,0x02,0x0a,0x0a,0xfe,0x0d,0x0a};
	s16 adcxleft,adcyright,adcyleft,mode=0,lastmode=0;
	s16 iadcxleft,iadcyright,iadcyleft;
	u8 qian[5],hou[5],zuo[5],you[5];
//	s16 fulvdai[4]={0}; //×óÇ°¡¢×óºó¡¢ÓÒÇ°¡¢ÓÒºó
	delay_init(168);
	LED_Init();
	Adc_Init();
	KEY_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	TIM5_Int_Init(1000-1, 8400-1);   //0.1s
	
  LCD_init();
	LCD_CLS();            //OLEDÇåÆÁ   
	
	start_animation();//¿ª»ú¶¯»­
	
	LCD_P16X16Str(0,1,"Ç°",CYAN,BLACK);     //ÏÔÊ¾ºº×Ö×Ö·û´®				
	LCD_P8X16Str(2,1,":",CYAN,BLACK);
	LCD_P16X16Str(4,1,"ºó",CYAN,BLACK); 
	LCD_P8X16Str(10,1,":",CYAN,BLACK);
	LCD_P16X16Str(0,2,"×ó",CYAN,BLACK); 
	LCD_P8X16Str(2,2,":",CYAN,BLACK);
	LCD_P16X16Str(4,2,"ÓÒ",CYAN,BLACK); 
	LCD_P8X16Str(10,2,":",CYAN,BLACK);
	LCD_P16X16Str(0,4,"µµÎ»",CYAN,BLACK); 
	LCD_P8X16Str(4,4,":",CYAN,BLACK);
	LCD_P16X16Str(3,4,"ÎÞ¡¤¡¤¡¤",WHITE,BLACK);
				
	
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
		adcyleft=tadcyleft;
		if(key4==1 && key7==1 && key8==1)    //ÂóÂÖ
		{
			adcyright=adcyright+100;
			adcxleft=adcxleft+100;
			mode=3;
		}
		else if(key4==1 && key7==0 && key8==1)    //ÂóÂÖ×óÐ±
		{
			adcyright=adcyright+100;
			mode=4;
		}
		else if(key4==1 && key7==1 && key8==0)    //ÂóÂÖÓÒÐ±
		{
			adcyleft=adcyleft+100;
			mode=5;
		}
		else if(key1==1)    //1µµ
		{
			adcxleft=(adcxleft+100)/2+50;
			adcyright=adcyright+100;
			mode=1;
		}
		else if(key3==1)    //2µµ
		{
			adcxleft=adcxleft+100;
			adcyright=adcyright*2+100;
			mode=2;
		}
		else         //0µµ
		{
			adcxleft = 100;//×óÒ¡¸ËxÖá
			adcyright = 100;//ÓÒÒ¡¸ËyÖá
			mode=0;
		}
				
				
		if(mode==0||mode==1||mode==2)
		{
	    iadcxleft=(int)adcxleft-100;
		  iadcyright=(int)adcyright-100;
		
		  if(iadcxleft>0)
		  {
		  	sprintf(you,"%3d",iadcxleft);
		  	sprintf(zuo,"%3d",0);
		  }
		  else
		  {
		  	sprintf(zuo,"%3d",-iadcxleft);
		  	sprintf(you,"%3d",0);
		  }
		  if(iadcyright>0)
		  {
		  	sprintf(qian,"%3d",iadcyright);
		  	sprintf(hou,"%3d",0);
		  }
		  else
		  {
		  	sprintf(hou,"%3d",-iadcyright);
	  		sprintf(qian,"%3d",0);
	  	}       
//      sprintf(qian,"%d  ",iadcyright);
//			sprintf(zuo,"%d  ",iadcxleft);
//       LCD_P8X16Str(2,1,"123456789",BLUE,BLACK);         //ÏÔÊ¾Ó¢ÎÄ8*16×Ö·û´®
			
		 
			send_message[3] = adcxleft;
		  send_message[4] = adcyright;
			
			for(i=0;i<8;i++)
	  	{
			  putchar(send_message[i]);
		  }
		}
		
		else if(mode==3)
		{
			iadcxleft=(int)(adcxleft-100);
			iadcyright=(int)(adcyright-100);
			if(iadcxleft>0)
			{
					sprintf(you,"%3d",iadcxleft);
					sprintf(zuo,"%3d",0);
			}
			else
			{
					sprintf(zuo,"%3d",-iadcxleft);
					sprintf(you,"%3d",0);
			}
			
			
			send_message1[3] = adcxleft;
			send_message1[4] = 100;			
			for(i=0;i<8;i++)
			{
				putchar(send_message1[i]);
			}
		}
		else if(mode==4)
		{
			
			iadcyright=(int)(adcyright-100); 
			if(iadcyright>0)
			{
					sprintf(qian,"%3d",iadcyright);
					sprintf(hou,"%3d",0);
			}
			else
			{
					sprintf(hou,"%3d",-iadcyright);
					sprintf(qian,"%3d",0);
			}  
			
			send_message2[3] = adcyright;
			send_message2[4] = 100;   
			for(i=0;i<8;i++)
			{
				putchar(send_message2[i]);
			}
		}
		else if(mode==5)
		{
			iadcyleft=(int)(adcyleft-100);
			if(iadcyleft>0)
			{
					sprintf(qian,"%3d",iadcyleft);
					sprintf(hou,"%3d",0);
			}
			else
			{
					sprintf(hou,"%3d",-iadcyleft);
					sprintf(qian,"%3d",0);
			} 
			
			send_message3[3] = adcyleft;
			send_message3[4] = 100;
			for(i=0;i<8;i++)
			{
				putchar(send_message3[i]);
			}
		}
		if(mode!=lastmode)
		{
		  LCD_CLS();            //OLEDÇåÆÁ   
			if(mode==3)
			{
				LCD_P16X16Str(0,3,"×ó",CYAN,BLACK); 
				LCD_P8X16Str(2,3,":",CYAN,BLACK);
				LCD_P16X16Str(4,3,"ÓÒ",CYAN,BLACK); 
				LCD_P8X16Str(10,3,":",CYAN,BLACK);
				LCD_P16X16Str(0,4,"µµÎ»",CYAN,BLACK); 
				LCD_P8X16Str(4,4,":",CYAN,BLACK);	
			}
			else if(mode==4||mode==5)
			{
				LCD_P16X16Str(0,2,"Ç°",CYAN,BLACK); 
				LCD_P8X16Str(2,2,":",CYAN,BLACK);
				LCD_P16X16Str(4,2,"ºó",CYAN,BLACK); 
				LCD_P8X16Str(10,2,":",CYAN,BLACK);
				LCD_P16X16Str(0,4,"µµÎ»",CYAN,BLACK); 
				LCD_P8X16Str(4,4,":",CYAN,BLACK);	
			}
			else if(mode==0||mode==1||mode==2)
			{
				LCD_P16X16Str(0,1,"Ç°",CYAN,BLACK);     //ÏÔÊ¾ºº×Ö×Ö·û´®				
				LCD_P8X16Str(2,1,":",CYAN,BLACK);
				LCD_P16X16Str(4,1,"ºó",CYAN,BLACK); 
				LCD_P8X16Str(10,1,":",CYAN,BLACK);
				LCD_P16X16Str(0,2,"×ó",CYAN,BLACK); 
				LCD_P8X16Str(2,2,":",CYAN,BLACK);
				LCD_P16X16Str(4,2,"ÓÒ",CYAN,BLACK); 
				LCD_P8X16Str(10,2,":",CYAN,BLACK);
				LCD_P16X16Str(0,4,"µµÎ»",CYAN,BLACK); 
				LCD_P8X16Str(4,4,":",CYAN,BLACK);	
			}
				if(mode==0)
					LCD_P16X16Str(3,4,"ÎÞ¡¤¡¤¡¤",WHITE,BLACK);
				else if(mode==1)
					LCD_P16X16Str(3,4,"¡¤µÍ¡¤¡¤",YELLOW,BLACK);
				else if(mode==2)
					LCD_P16X16Str(3,4,"¡¤¡¤ÖÐ¡¤",ORANGE,BLACK);
				else if(mode==3)
				{
				  LCD_P16X16Str(3,4,"ÂóÂÖÄ£Ê½",RED,BLACK);
				  LCD_P16X16Str(0,1,"¡¤¡¤¡¤Æ½ÒÆ¡¤¡¤¡¤",RED,BLACK);
				}
				else if(mode==4)
				{
					LCD_P16X16Str(3,4,"ÂóÂÖÄ£Ê½",RED,BLACK);
					LCD_P16X16Str(0,1,"¡¤¡¤×óÐ±·½¡¤¡¤¡¤",RED,BLACK);
				}
				else if(mode==5)
				{
					LCD_P16X16Str(3,4,"ÂóÂÖÄ£Ê½",RED,BLACK);
					LCD_P16X16Str(0,1,"¡¤¡¤¡¤ÓÒÐ±·½¡¤¡¤",RED,BLACK);
				}
			
		}
		if(mode==3)
		{
			
			LCD_P8X16Str(3,3,zuo,RED,BLACK);
			LCD_P8X16Str(11,3,you,RED,BLACK);
		}
		else if(mode==4)
		{
			LCD_P8X16Str(3,2,qian,RED,BLACK);
			LCD_P8X16Str(11,2,hou,RED,BLACK);
		}
		else if(mode==5)
		{
			LCD_P8X16Str(3,2,qian,RED,BLACK);
			LCD_P8X16Str(11,2,hou,RED,BLACK);
		}
		else if(mode==0)
		{	
								
				LCD_P8X16Str(3,1,qian,WHITE,BLACK);
				LCD_P8X16Str(11,1,hou,WHITE,BLACK);
				LCD_P8X16Str(3,2,zuo,WHITE,BLACK);
				LCD_P8X16Str(11,2,you,WHITE,BLACK);
				
		}
		else if(mode==1)
		{	
				LCD_P8X16Str(3,1,qian,YELLOW,BLACK);
				LCD_P8X16Str(11,1,hou,YELLOW,BLACK);
				LCD_P8X16Str(3,2,zuo,YELLOW,BLACK);
				LCD_P8X16Str(11,2,you,YELLOW,BLACK);
				
				
		}
		else if(mode==2)
		{	
				LCD_P8X16Str(3,1,qian,ORANGE,BLACK);
				LCD_P8X16Str(11,1,hou,ORANGE,BLACK);
				LCD_P8X16Str(3,2,zuo,ORANGE,BLACK);
				LCD_P8X16Str(11,2,you,ORANGE,BLACK);
				
		}
		
		lastmode=mode;
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







