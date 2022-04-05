#include "iwdg.h"

/**
  * @brief  初始化独立看门狗
  * @param  prer:分频数:0~7
  *         rlr:自动重装载值,0~0XFFF
  * @retval void
  * @note   时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms).
  */
  
void IWDG_Init(u8 prer, u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对IWDG->PR IWDG->RLR的写
	
	IWDG_SetPrescaler(prer); //设置IWDG分频系数

	IWDG_SetReload(rlr);   //设置IWDG装载值

	IWDG_ReloadCounter(); //reload
	
	IWDG_Enable();       //使能看门狗
}

//喂独立看门狗
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();//reload
}
