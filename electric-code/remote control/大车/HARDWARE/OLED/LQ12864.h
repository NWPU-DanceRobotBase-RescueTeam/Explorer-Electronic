/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�MK66FX1M0VLQ18���İ�
����    д��CHIUSIR
����    ע��
������汾��V1.0
�������¡�2016��08��20��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
���������䡿chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef __LQ12864_H__
#define __LQ12864_H__
#include "sys.h"
/*******************�ӿڶ���******************************/
					  
#define	LQTFT_A0	PDout(3)
#define	LQTFT_RST	PDout(2) 
#define	LQTFT_SDA	PDout(1) 
#define	LQTFT_SCK	PDout(0)
#define	LQTFT_CS	PDout(4) 


#define	RED	0xf800
#define	GREEN	0x07e0
#define	BLUE	0x001f
#define	PURPLE	0xf81f
#define	YELLOW	0xffe0
#define	CYAN	0x07ff 		//����ɫ
#define	ORANGE	0xfc08
#define	BLACK	0x0000
#define	WHITE	0xffff

#define	XMAX	132
#define	YMAX	64

/*****************˽�к�������*********************************/
void time_delay_ms(unsigned int ms)	 ;
void LCD_init(void);									//LCD��ʼ��

void LCD_write_command(unsigned char cmd);						//���Ϳ�����

void LCD_write_para8(unsigned char dat);						//�������ݲ���

void LCD_write_para16(unsigned int dat);						//����������ʾ����

void LCD_address_rst(void);							    //DDRAM��ַ����

void LCD_SetPos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye);	//��λ��ʾ����λ��
void LCD_display_full(unsigned int color);					//ȫ����ʾĳ����ɫ
void LCD_CLS(void);

void LCD_draw_part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat);
															//��������������ʼ����ֹ���꣬����ʼ����ֹ���꣬��ɫ
void LCD_draw_line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat);
															//���ߣ�����ʼ����ֹ���꣬����ʼ����ֹ���꣬��ɫ
void LCD_draw_rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat);
															//�����α߿�����ʼ����ֹ���꣬����ʼ����ֹ���꣬��ɫ
void LCD_draw_circle(unsigned char x,unsigned char y,unsigned char r,unsigned int color_dat);
															//��Բ�α߿�Բ�ĺ����ꡢ�����꣬�뾶����ɫ
void LCD_draw_dot(unsigned char x,unsigned char y,unsigned int color_dat);	//���㣬�����꣬�����꣬��ɫ

void display_pic(void);//��ʾͼƬ

void LCD_P8X16(unsigned char x, unsigned char y, unsigned char c_dat,unsigned int word_color,unsigned int back_color);

void LCD_P8X16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned int word_color,unsigned int back_color);

void LCD_P6X8(unsigned char x, unsigned char y, unsigned char c_dat,unsigned int word_color,unsigned int back_color);

void LCD_P6X8Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned int word_color,unsigned int back_color);

void LCD_P6X8NUM(unsigned char x, unsigned char y, unsigned int num, unsigned char num_bit,unsigned int word_color,unsigned int back_color);

void LCD_P8X8(unsigned char x, unsigned char y, unsigned char c_dat,unsigned int word_color,unsigned int back_color);

void LCD_P8X8Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned int word_color,unsigned int back_color);

void LCD_P8X8NUM(unsigned char x, unsigned char y, unsigned int num, unsigned char num_bit,unsigned int word_color,unsigned int back_color);

void LCD_P16X16Str(unsigned char x,unsigned char y,unsigned char *s_dat,unsigned int word_color,unsigned int back_color);
void LCD_P16X12Str(unsigned char x,unsigned char y,unsigned char *s_dat,unsigned int word_color,unsigned int back_color);
void show_pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned char pic[]);

#endif /*SGP18T_LQTFTB.h*/
