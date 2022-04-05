#ifndef __QUEUE_H
#define __QUEUE_H

#include "stm32f4xx.h"

#define QUEUE_SIZE  64    				//缓冲大小


#define Empty_Queue(Queue_Check)	((Queue_Check).Q_Mum == 0)
#define Full_Queue(Queue_Check)		((Queue_Check).Q_Mum == QUEUE_SIZE)

typedef struct{
	float Q_data[QUEUE_SIZE];
	uint8_t Q_Head, Q_Tail, Q_Mum;
}Queue;															//接受队列结构体

float Top_Queue(Queue * pQueue);
void Push_Queue(Queue *pQueue, float ch);
void Init_Queue(Queue *pQueue);

#endif /*__QUEUE_H*/
