#include "iwdg.h"

/**
  * @brief  ��ʼ���������Ź�
  * @param  prer:��Ƶ��:0~7
  *         rlr:�Զ���װ��ֵ,0~0XFFF
  * @retval void
  * @note   ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).
  */
  
void IWDG_Init(u8 prer, u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶ�IWDG->PR IWDG->RLR��д
	
	IWDG_SetPrescaler(prer); //����IWDG��Ƶϵ��

	IWDG_SetReload(rlr);   //����IWDGװ��ֵ

	IWDG_ReloadCounter(); //reload
	
	IWDG_Enable();       //ʹ�ܿ��Ź�
}

/**
  * @brief  ι�������Ź�
  * @param  void
  * @retval void
  */

void IWDG_Feed(void)
{
	IWDG_ReloadCounter();//reload
}