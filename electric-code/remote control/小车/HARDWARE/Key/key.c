#include "key.h"
#include "tim.h"
#include "delay.h"
#include "led.h"

/**
  * @brief  ���س�ʼ��
  * @param  void
  * @retval void
  */
  
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);//ʹ�� GPIOA,GPIOC ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;// KEY3 KEY4 ��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP ��Ӧ���� 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  ��������ģʽ�ı�
  * @param  void
  * @retval void
  */

s16 adcxleft, adcyright;
u8 mode = 0;

void Mode_Change(void)
{
	adcxleft = (s16)tadcxleft;
	adcyright = (s16)tadcyright;

	if (key1 == 1)    //1��
	{
		adcxleft = (adcxleft + 100) / 2 + 50;
		adcyright = adcyright + 100;
		mode = 1;
	}
	else if (key3 == 1)    //2��
	{
		adcxleft = adcxleft + 100;
		adcyright = adcyright * 2 + 100;
		mode = 2;
	}
	else if (key4 == 1)    //3��
	{
		adcxleft = adcxleft * 2 + 100;
		adcyright = adcyright * 4 + 100;
		mode = 3;
	}
	else           //0��
	{
		adcxleft = 100;//��ҡ��x��
		adcyright = 100;//��ҡ��y��
		mode = 0;
	}
}

void Key_Scan(void)
{
	if (key5 == 0)
	{
		delay_ms(50);
		if (key5 == 0)
		{
			delay_ms(50);
			send_message[1] = 0x01;
			delay_ms(50);
		}
	}
	else if (key6 == 0)
	{
		delay_ms(50);
		if (key6 == 0)
		{
			delay_ms(50);
			send_message[1] = 0x13;
			delay_ms(50);
		}
	}
	else if (key7 == 0)
	{
		delay_ms(50);
		if (key7 == 0)
		{
			delay_ms(50);
			send_message[1] = 0x25;
			delay_ms(50);
			LED1 = ~LED1;


		}
	}
	else if (key8 == 0)
	{
		delay_ms(50);
		if (key8 == 0)
		{
			LED2 = ~LED2;
			delay_ms(50);
			send_message[1] = 0x37;
			delay_ms(50);

		}
	}
}
