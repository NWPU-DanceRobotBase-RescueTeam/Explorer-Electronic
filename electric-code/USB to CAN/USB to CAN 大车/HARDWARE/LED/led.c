#include "led.h" 

////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ��PC13Ϊ����ڣ���ʹ������ڵ�ʱ��   
//LED IO��ʼ��
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOCʱ��

  //GPIOC13,��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//LED0��LED1��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_SetBits(GPIOA,GPIO_Pin_15);//GPIOC13,���øߣ�����

}








