#include "fsm.h"
#include "usart.h"
#include "Motor_Control.h"

//状态机发送格式：FF FF/FB 长度 数据包 FE

enum
{
    WAITING_FF1,
    RECEIVER1_ID,
	LENTH1,
    MESSAGE1,
    END,
	ERRORR
}receive_state;

/**
  * @brief  检查串口是否收到数据
  * @param  void
  * @retval 0:无数据,否则为数据长度
  */

u8 Usart_Receivecheck(void)
{
	u8 len;
	if (USART_RX_STA & 0X8000)
	{
		len = USART_RX_STA & 0x3fff;
		USART_RX_STA = 0;
		printf("Usart receive succesfully!\r\n");
		return len;
	}
	else
	{
		return 0;
	}
}

/**
  * @brief  串口数据状态机
  * @param  len:数据长度
  *         msg1:数据内容
  *         mode:数据ID
  * @retval 0:数据不符格式,否则为内容长度
  */

u8 FSM(u16 len, u8* msg1, u8* mode) //状态机函数
{
	u16 n = 0;
	u8 lenth, i;
	u32 rx_data_;
	receive_state = WAITING_FF1;
	while (1)
	{
		rx_data_ = USART1_RX_BUF[n++];
		switch (receive_state)
		{
		case WAITING_FF1:
			if (rx_data_ == 0xff)
				receive_state = RECEIVER1_ID;
			else
			{
				n--;
				receive_state = ERRORR;
			}
			break;

		case RECEIVER1_ID:
			if (rx_data_ == 0xFF || rx_data_ == 0xFB || rx_data_ == 0x25 || rx_data_ == 0x37 || rx_data_ == 0x01 || rx_data_ == 0x13 || rx_data_ == 0xFA || rx_data_ == 0xFC || rx_data_ == 0xF0)
			{
				receive_state = LENTH1;
				*mode = rx_data_;
			}
			else
			{
				n--;
				receive_state = ERRORR;
			}
			break;

		case LENTH1:
			if (rx_data_ > 0 && rx_data_ < 10)
			{
				lenth = rx_data_;
				receive_state = MESSAGE1;
				i = 0;
			}
			else
			{
				n--;
				receive_state = ERRORR;
			}
			break;

		case MESSAGE1:
			if (rx_data_ > 0x00 && rx_data_ < 0xff)
			{
				msg1[i] = rx_data_;
				i++;
				if (i >= lenth)
					receive_state = END; //RECEIVER2_ID;
			}
			else
			{
				n--;
				receive_state = ERRORR;
			}
			break;

		case END:
			if (rx_data_ == 0xfe)
			{
				receive_state = WAITING_FF1;
				return lenth;
			}
			else
			{
				n--;
				receive_state = ERRORR;
			}
			break;

		case ERRORR:
		{
			M3508[0].PID.Goal_Speed = 0;
			M3508[1].PID.Goal_Speed = 0;
			M3508[2].PID.Goal_Speed = 0;
			M3508[3].PID.Goal_Speed = 0;
			printf("wrong\r\n");
			return 0;
		}

		default:
			receive_state = ERRORR;
		}
		//printf("%f",&n);
		if (n == len)
			break;
	}
	return 0;
}

