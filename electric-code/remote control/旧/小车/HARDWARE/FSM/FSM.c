#include "usart.h"
#include "fsm.h"

//状态机发送格式：FF FF/FE/FD 长度 数据包 FE

enum
{
    WAITING_FF1,
    RECEIVER1_ID,
	  LENTH1,
    MESSAGE1,
    END
}receive_state;


int Usart_Receivecheck()  //检查串口是否收到数据，收到则返回长度，否则返回0
{
	u8 len;
	if(USART_RX_STA&0X8000)
		{
			len=USART_RX_STA&0x3fff;
			USART_RX_STA=0;
			printf("Usart receive succesfully!\r\n");
			return len;
		}
	else
	{
		return 0;
	}
}

int FSM(u16 len,u8* msg1,u8* mode) //状态机函数
{
	u16 n=0;
	u8 lenth,i;
	u32 rx_data_;
	receive_state= WAITING_FF1;
	while(1)
	{
		rx_data_=USART_RX_BUF[n];
		n++;
		switch(receive_state)
		{
			case WAITING_FF1:
				if(rx_data_==0xff)
				{
					receive_state=RECEIVER1_ID;
				}	
				break;

			case RECEIVER1_ID:
				if(rx_data_==0xFF||rx_data_==0xFE||rx_data_==0xFD||rx_data_==0xFC||rx_data_==0xFB||rx_data_==0xFA||rx_data_==0xF9)
				{
					receive_state=LENTH1;
					* mode=rx_data_;
				}
				break;

			case LENTH1:
				lenth = rx_data_;
				receive_state= MESSAGE1;
				i = 0;
				break;

			case MESSAGE1:
				msg1[i] = rx_data_;
				i++;
				if(i==lenth) receive_state= END; //RECEIVER2_ID;
				break;

			case END:
				if(rx_data_==0xfe)
				{
					printf("FSM is OK!\r\n ");
					receive_state=WAITING_FF1;
					return lenth;
				}

			default:
			{
				receive_state=WAITING_FF1;
				return 0;
			}
		}
		//printf("%f",&n);
		if(n==len)  break;
	}
	return 0;
}
