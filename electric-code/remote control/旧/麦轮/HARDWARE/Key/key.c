#include "key.h"


//key1  PC0    key3  PA0      key4  PA3

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);//ʹ�� GPIOA,GPIOC ʱ��
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3;// KEY3 KEY4 ��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP ��Ӧ���� 
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


