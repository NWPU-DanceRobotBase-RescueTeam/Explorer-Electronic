#include "fsm.h"

//״̬�����͸�ʽ��FF FF/FB ���� ���ݰ� FE

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
 * @brief  ��鴮���Ƿ��յ�����
 * @param  void
 * @retval 0:������,����Ϊ���ݳ���
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
 * @brief  ��������״̬��
 * @param  len:���ݳ���
 *         msg1:��������
 *         mode:����ID
 * @retval 0:���ݲ�����ʽ,����Ϊ���ݳ���
 */

uint8_t FSM(uint16_t len, uint8_t *msg1, uint8_t *mode) //״̬������
{
	uint16_t n = 0;
	uint8_t lenth, i;
	uint32_t rx_data_;
	receive_state = WAITING_FF1;
	while (1)
	{
		rx_data_ = USART_RX_BUF[n++];
		switch (receive_state)
		{
			case WAITING_FF1:
				if (rx_data_ == 0xff)
					receive_state = RECEIVER1_ID;
				else
				{
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
					receive_state = ERRORR;
				}
				break;

			default:
				receive_state = ERRORR;
		}
		//printf("%f",&n);
		if (receive_state == ERRORR)
		{
			M3508[0].PID.Goal_Speed = 0;
			M3508[1].PID.Goal_Speed = 0;
			M3508[2].PID.Goal_Speed = 0;
			M3508[3].PID.Goal_Speed = 0;
			printf("wrong\r\n");
			return 0;
		}

		if (n == len)
			break;
	}
	return 0;
}

