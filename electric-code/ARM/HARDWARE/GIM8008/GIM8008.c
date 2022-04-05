#include "GIM8008.h"
#include "delay.h"
#include "usart.h"
#include "can.h"

#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))

// 将float值转换为u16
static u16 float_to_uint(float x, float x_min, float x_max, uint8_t bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return (u16)((x - offset) * ((float)((1 << bits) - 1)) / span);
}

/**
  * @brief  电机开始控制，并设各参数为零
  * @param  void
  * @retval void
  */

void ZeroPosition(void)
{
    Set_8008_Mode(CMD_MOTOR_MODE);
    delay_ms(100);
    Send_8008_Control(0, 0, 0, 0, 0);
    delay_ms(100);
}

/**
  * @brief  电机基本设置
  * @param  mode:CMD_MOTOR_MODE开始接收信息，CMD_RESET_MODE停止接收信息，CMD_ZERO_POSITION设置此为零位置
  * @retval void
  */

void Set_8008_Mode(u8 mode)
{
	uint8_t buf[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 };
	switch (mode)
	{
	case CMD_MOTOR_MODE:
		buf[7] = 0xFC;
		break;

	case CMD_RESET_MODE:
		buf[7] = 0xFD;
		break;

	case CMD_ZERO_POSITION:
		buf[7] = 0xFE;
		break;

	default:
		return; /* 直接退出函数 */
	}
	CAN2_Send_Msg(buf, 8, 1);
}

/**
  * @brief  电机发送信息
  * @param  f_p:目标位置,f_v:目标速度,f_kp:位置增益,f_kd:速度增益,f_t:力矩
  * @retval void
  */

void Send_8008_Control(float f_p, float f_v, float f_kp, float f_kd, float f_t)
{
    uint16_t p, v, kp, kd, t;
    uint8_t buf[8];

    /* 限制输入的参数在定义的范围内 */
    LIMIT_MIN_MAX(f_p, P_MIN, P_MAX);
    LIMIT_MIN_MAX(f_v, V_MIN, V_MAX);
    LIMIT_MIN_MAX(f_kp, KP_MIN, KP_MAX);
    LIMIT_MIN_MAX(f_kd, KD_MIN, KD_MAX);
    LIMIT_MIN_MAX(f_t, T_MIN, T_MAX);

    /* 根据协议，对float参数进行转换 */
    p = float_to_uint(f_p, P_MIN, P_MAX, 16);
    v = float_to_uint(f_v, V_MIN, V_MAX, 12);
    kp = float_to_uint(f_kp, KP_MIN, KP_MAX, 12);
    kd = float_to_uint(f_kd, KD_MIN, KD_MAX, 12);
    t = float_to_uint(f_t, T_MIN, T_MAX, 12);

    /* 根据传输协议，把数据转换为CAN命令数据字段 */
    buf[0] = p >> 8;
    buf[1] = p & 0xFF;
    buf[2] = v >> 4;
    buf[3] = ((v & 0xF) << 4) | (kp >> 8);
    buf[4] = kp & 0xFF;
    buf[5] = kd >> 4;
    buf[6] = ((kd & 0xF) << 4) | (t >> 8);
    buf[7] = t & 0xff;

    /* 通过CAN接口把buf中的内容发送出去 */
    CAN2_Send_Msg(buf, 8, 1);
}

/**
  * @brief  电机初始化
  * @param  void
  * @retval void
  */

void MotorControl_Start(void)
{
    ZeroPosition();
    Set_8008_Mode(CMD_ZERO_POSITION);
    delay_ms(10);
    Send_8008_Control(0.25 * 0.8, 0, GIM_Kp, GIM_Kd, 0);
}

/**
  * @brief  电机失能
  * @param  void
  * @retval void
  */

void MotorControl_Stop(void)
{
    Set_8008_Mode(CMD_RESET_MODE);
}

