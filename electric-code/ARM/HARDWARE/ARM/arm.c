#include "arm.h"
#include "delay.h"
#include "pwm.h"
#include "iwdg.h"

const float SERVO_RAD = 636.61977;	
const float SERVO_RAD1 = 424.42;
const float MOTOR_RAD = 57.325;

// �������PWMֵ
u16 pwm[8] = {0, 0, 0, 1460, 1500, 1200, 1500, 1500};	

/**
  * @brief  ������Ƕ��趨
  * @param  id�����ID
  *         pwm_re������趨��PWMֵ
  * @retval void
  */

void servo_set(u8 id, u16 pwm_re)//����趨
{
	u16 pwm_set;
	u16 i;
	switch (id)
	{
	case 7://צ�ӿ���
		if (pwm_re <= 1475)
			pwm_re = 1475;
		else if (pwm_re >= 1900)
			pwm_re = 1900;

		if (pwm[id] < pwm_re)
		{
			for (i = pwm[id]; i <= pwm_re; i += 5)
			{
				pwm_set = i;
				TIM_SetCompare2(TIM4, pwm_set);
				delay_ms(5);
				IWDG_Feed();
				pwm[id] = i;
			}
		}
		else if (pwm[id] > pwm_re)
		{
			for (i = pwm[id]; i >= pwm_re; i -= 5)
			{
				pwm_set = i;
				TIM_SetCompare2(TIM4, pwm_set);
				delay_ms(5);
				IWDG_Feed();
				pwm[id] = i;
			}
		}
		else
		{
			pwm_set = pwm_re;
			TIM_SetCompare2(TIM4, pwm_set);
		}
		break;

	case 6://צ��������ת
		if (pwm_re <= 500)
			pwm_re = 500;
		else if (pwm_re >= 2500)
			pwm_re = 2500;

		if (pwm[id] < pwm_re)
		{
			for (i = pwm[id]; i <= pwm_re; i += 10)
			{
				pwm_set = i;
				TIM_SetCompare1(TIM4, pwm_set);
				delay_ms(5);
				IWDG_Feed();
				pwm[id] = i;
			}
		}
		else if (pwm[id] > pwm_re)
		{
			for (i = pwm[id]; i >= pwm_re; i -= 10)
			{
				pwm_set = i;
				TIM_SetCompare1(TIM4, pwm_set);
				delay_ms(5);
				IWDG_Feed();
				pwm[id] = i;
			}
		}
		else
		{
			pwm_set = pwm_re;
			TIM_SetCompare1(TIM4, pwm_set);
		}
		break;

	case 5://����ͷ//צ�������ƶ�
		if (pwm_re <= 1000)
			pwm_re = 1000;
		else if (pwm_re >= 2350)
			pwm_re = 2350;

		if (pwm[id] < pwm_re)
		{
			for (i = pwm[id]; i <= pwm_re; i += 2)
			{
				pwm_set = i;
				TIM_SetCompare2(TIM2, pwm_set);
				delay_ms(10);
				IWDG_Feed();
				pwm[id] = i;
			}
		}
		else if (pwm[id] > pwm_re)
		{
			for (i = pwm[id]; i >= pwm_re; i -= 2)
			{
				pwm_set = i;
				TIM_SetCompare2(TIM2, pwm_set);
				delay_ms(10);
				IWDG_Feed();
				pwm[id] = i;
			}
		}
		else
		{
			pwm_set = pwm_re;
			TIM_SetCompare2(TIM2, pwm_set);
		}
		break;

	case 4://��������ת//צ��������ת
		if (pwm_re <= 500)
			pwm_re = 500;
		else if (pwm_re >= 2500)
			pwm_re = 2500;

		if (pwm[id] < pwm_re)
		{
			for (i = pwm[id]; i <= pwm_re; i += 2)
			{
				pwm_set = i;
				TIM_SetCompare4(TIM3, pwm_set);
				delay_ms(5);
				IWDG_Feed();
				pwm[id] = i;
			}
		}
		else if (pwm[id] > pwm_re)
		{
			for (i = pwm[id]; i >= pwm_re; i -= 2)
			{
				pwm_set = i;
				TIM_SetCompare4(TIM3, pwm_set);
				delay_ms(5);
				IWDG_Feed();
				pwm[id] = i;
			}
		}
		else
		{
			pwm_set = pwm_re;
			TIM_SetCompare4(TIM3, pwm_set);
		}
		break;

	case 3:    // ����
		if (pwm_re <= 500)
			pwm_re = 500;
		else if (pwm_re >= 2500)
			pwm_re = 2500;

		if (pwm[id] < pwm_re)
		{
			for (i = pwm[id]; i <= pwm_re; i += 5)
			{
				pwm_set = i;
				TIM_SetCompare3(TIM3, pwm_set);
				delay_ms(5);
				IWDG_Feed();
				pwm[id] = i;
			}
		}
		else if (pwm[id] > pwm_re)
		{
			for (i = pwm[id]; i >= pwm_re; i -= 5)
			{
				pwm_set = i;
				TIM_SetCompare3(TIM3, pwm_set);
				delay_ms(5);
				IWDG_Feed();
				pwm[id] = i;
			}
		}
		else
		{
			pwm_set = pwm_re;
			TIM_SetCompare3(TIM3, pwm_set);
		}
		break;
	}
}

/**
  * @brief  �������ʼ��
  * @param  void
  * @retval void
  */

void servo_init(void)
{
	// 84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ20000������PWMƵ��Ϊ 1M/20000=50hz.  
	TIM4_PWM_Init(20000-1, 84-1);
	TIM3_PWM_Init(20000-1, 84-1);
	TIM2_PWM_Init(20000-1, 84-1);
	delay_ms(50);
	TIM_SetCompare3(TIM3, pwm[3]);
	delay_ms(50);
	TIM_SetCompare4(TIM3, pwm[4]);
	delay_ms(50);
	IWDG_Feed();
	TIM_SetCompare2(TIM2, pwm[5]);
	delay_ms(50);
	TIM_SetCompare1(TIM4, pwm[6]);
	delay_ms(50);
	TIM_SetCompare2(TIM4, pwm[7]);
	delay_ms(50);
}





