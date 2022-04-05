#include "fsm.h"

//状态机发送格式：FF FF/FE/FD 长度 数据包 FE

enum
{
	WAITING_FF1,
	RECEIVER1_ID,
	LENTH1,
	MESSAGE1,
	END,
	ERRORR
} receive_state;

/**
 * @brief  检查串口是否收到数据
 * @param  void
 * @retval 0:无数据,否则为数据长度
 */

uint8_t Usart_Receivecheck(void)
{
	uint8_t len;
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

uint8_t FSM(uint8_t len, uint8_t *msg1, uint8_t *mode) //状态机函数
{
	uint8_t n = 0;
	uint8_t lenth, i;
	uint8_t rx_data_;
	receive_state = WAITING_FF1;
	while (1)
	{
		rx_data_ = USART_RX_BUF[n++];
		switch (receive_state)
		{
			case WAITING_FF1:
				if (rx_data_ == 0xff)
				{
					receive_state = RECEIVER1_ID;
				}
				else
				{
					printf("ERROR1\r\n ");
					n--;
					receive_state = ERRORR;
				}
				break;

			case RECEIVER1_ID:
				if (rx_data_ == 0xFF || rx_data_ == 0xFE || rx_data_ == 0xFD || rx_data_ == 0xFC || rx_data_ == 0xFB || rx_data_ == 0xFA || rx_data_ == 0xF9)
				{
					receive_state = LENTH1;
					*mode = rx_data_;
				}
				else
				{
					printf("ERROR2\r\n ");
					n--;
					receive_state = ERRORR;
				}
				break;

			case LENTH1:
				lenth = rx_data_;
				receive_state = MESSAGE1;
				i = 0;
				break;

			case MESSAGE1:
				msg1[i] = rx_data_;
				i++;
				if (i == lenth)
					receive_state = END; //RECEIVER2_ID;
				break;

			case END:
				if (rx_data_ == 0xfe)
				{
					printf("FSM is OK!\r\n ");
					receive_state = WAITING_FF1;
					return lenth;
				}
				else
				{
					printf("ERROR3\r\n ");
					n--;
					receive_state = ERRORR;
				}
				break;

			case ERRORR:
				for (i = 0; i < 2; i++)
					MAXON_MOTOR[i] = 0;
				for (i = 0; i < 4; i++)
				{
					M3508[i].Stop_Flag = 10;
					M3508[i].PID.Goal_Speed = 0;
				}
				receive_state = WAITING_FF1;
				return 0;

			default:
				receive_state = WAITING_FF1;
				return 0;
		}
		if (n == len)
			break;
	}
	return 0;
}

