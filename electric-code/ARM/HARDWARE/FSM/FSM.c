#include "fsm.h"
#include "usart.h"

//״̬�����͸�ʽ��FF 05/16/27/38 ���� ���ݰ� FE 0d 0a

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
  * @brief  ��鴮���Ƿ��յ�����
  * @param  void
  * @retval 0:������,����Ϊ���ݳ���
  */

u8 Usart_Receivecheck()
{
	u8 len;
	if (USART_RX_STA & 0X8000)
	{
		len = USART_RX_STA & 0x3fff;
		USART_RX_STA = 0;
		printf("Usart\r\n");
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

u8 FSM(u16 len, u8* msg1, u8* mode) //״̬������
{
	u16 n = 0;
	u8 lenth, i;
	u32 rx_data_;
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
//				printf("ERROR1\r\n ");
				n--;
				receive_state = ERRORR;
			}
			break;

		case RECEIVER1_ID:
			if (rx_data_ == 0x05 || rx_data_ == 0x16 || rx_data_ == 0x27 || rx_data_ == 0x38 || rx_data_ == 0x49 || rx_data_ == 0x5a || rx_data_ == 0x00)
			{
				receive_state = LENTH1;
				*mode = rx_data_;
			}
			else if (rx_data_ == 0xff)
			{
				receive_state = RECEIVER1_ID;
			}
			else
			{
//				printf("ERROR2\r\n ");
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
//				printf("FSM is OK!\r\n ");
				receive_state = WAITING_FF1;
				return lenth;
			}
			else
			{
//				printf("ERROR3\r\n ");
				n--;
				receive_state = ERRORR;
			}
			break;

		case ERRORR:
//			EXTI_GenerateSWInterrupt(EXTI_Line0);
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
