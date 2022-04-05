#include "queue.h"
/**
  * @brief  队列操作函数
  * @param  None
  * @retval None
  */
float Top_Queue(Queue *pQueue)
{
	float data;
	data = pQueue->Q_data[pQueue->Q_Head];
	pQueue->Q_Head = (pQueue->Q_Head + 1) % QUEUE_SIZE;
	pQueue->Q_Mum --;
	return data;
}

void Push_Queue(Queue *pQueue, float ch)
{
	pQueue->Q_data[pQueue->Q_Tail] = ch;
	pQueue->Q_Tail = (pQueue->Q_Tail + 1) % QUEUE_SIZE;
	pQueue->Q_Mum ++;
}

void Init_Queue(Queue *pQueue)
{
	pQueue->Q_Head = 0;
	pQueue->Q_Tail = 0;
	pQueue->Q_Mum = 0;
}
