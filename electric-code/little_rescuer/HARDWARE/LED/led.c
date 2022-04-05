#include "led.h"

/**
  * @brief  LED�Ƴ�ʼ��
  * @param  void
  * @retval void
  */
 
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��

	//GPIOC13,��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//LED0��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO

	GPIO_ResetBits(GPIOC, GPIO_Pin_13);//GPIOC13,���øߣ�����
}

/**
  * @brief  ���ߴ���ģʽ���ó�ʼ��,��M0,M1��Ϊ0,������ģʽ
  * @param  void
  * @retval void
  */

void Wireless_serial_port_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

	//GPIOC13,��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	// M0,M1=0
	GPIO_ResetBits(GPIOB, GPIO_Pin_4 | GPIO_Pin_5);
}

