#include "GIM8008.h"
#include "delay.h"
#include "usart.h"
#include "can.h"

#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))

// ��floatֵת��Ϊu16
static u16 float_to_uint(float x, float x_min, float x_max, uint8_t bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return (u16)((x - offset) * ((float)((1 << bits) - 1)) / span);
}

/**
  * @brief  �����ʼ���ƣ����������Ϊ��
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
  * @brief  �����������
  * @param  mode:CMD_MOTOR_MODE��ʼ������Ϣ��CMD_RESET_MODEֹͣ������Ϣ��CMD_ZERO_POSITION���ô�Ϊ��λ��
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
		return; /* ֱ���˳����� */
	}
	CAN2_Send_Msg(buf, 8, 1);
}

/**
  * @brief  ���������Ϣ
  * @param  f_p:Ŀ��λ��,f_v:Ŀ���ٶ�,f_kp:λ������,f_kd:�ٶ�����,f_t:����
  * @retval void
  */

void Send_8008_Control(float f_p, float f_v, float f_kp, float f_kd, float f_t)
{
    uint16_t p, v, kp, kd, t;
    uint8_t buf[8];

    /* ��������Ĳ����ڶ���ķ�Χ�� */
    LIMIT_MIN_MAX(f_p, P_MIN, P_MAX);
    LIMIT_MIN_MAX(f_v, V_MIN, V_MAX);
    LIMIT_MIN_MAX(f_kp, KP_MIN, KP_MAX);
    LIMIT_MIN_MAX(f_kd, KD_MIN, KD_MAX);
    LIMIT_MIN_MAX(f_t, T_MIN, T_MAX);

    /* ����Э�飬��float��������ת�� */
    p = float_to_uint(f_p, P_MIN, P_MAX, 16);
    v = float_to_uint(f_v, V_MIN, V_MAX, 12);
    kp = float_to_uint(f_kp, KP_MIN, KP_MAX, 12);
    kd = float_to_uint(f_kd, KD_MIN, KD_MAX, 12);
    t = float_to_uint(f_t, T_MIN, T_MAX, 12);

    /* ���ݴ���Э�飬������ת��ΪCAN���������ֶ� */
    buf[0] = p >> 8;
    buf[1] = p & 0xFF;
    buf[2] = v >> 4;
    buf[3] = ((v & 0xF) << 4) | (kp >> 8);
    buf[4] = kp & 0xFF;
    buf[5] = kd >> 4;
    buf[6] = ((kd & 0xF) << 4) | (t >> 8);
    buf[7] = t & 0xff;

    /* ͨ��CAN�ӿڰ�buf�е����ݷ��ͳ�ȥ */
    CAN2_Send_Msg(buf, 8, 1);
}

/**
  * @brief  �����ʼ��
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
  * @brief  ���ʧ��
  * @param  void
  * @retval void
  */

void MotorControl_Stop(void)
{
    Set_8008_Mode(CMD_RESET_MODE);
}

