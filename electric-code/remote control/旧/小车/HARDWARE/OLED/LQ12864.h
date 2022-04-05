/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技MK66FX1M0VLQ18核心板
【编    写】CHIUSIR
【备    注】
【软件版本】V1.0
【最后更新】2016年08月20日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef __LQ12864_H__
#define __LQ12864_H__
#include "sys.h"
/*******************接口定义******************************/
					  
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
#define	CYAN	0x07ff 		//蓝绿色
#define	ORANGE	0xfc08
#define	BLACK	0x0000
#define	WHITE	0xffff

#define	XMAX	132
#define	YMAX	64

/*****************私有函数声名*********************************/
void time_delay_ms(unsigned int ms)	 ;
void LCD_init(void);									//LCD初始化

void LCD_write_command(unsigned char cmd);						//发送控制字

void LCD_write_para8(unsigned char dat);						//发送数据参数

void LCD_write_para16(unsigned int dat);						//发送像素显示参数

void LCD_address_rst(void);							    //DDRAM地址重置

void LCD_SetPos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye);	//定位显示像素位置
void LCD_display_full(unsigned int color);					//全屏显示某种颜色
void LCD_CLS(void);

void LCD_draw_part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat);
															//填充矩形区域，行起始、终止坐标，列起始、终止坐标，颜色
void LCD_draw_line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat);
															//画线，行起始、终止坐标，列起始、终止坐标，颜色
void LCD_draw_rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat);
															//画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
void LCD_draw_circle(unsigned char x,unsigned char y,unsigned char r,unsigned int color_dat);
															//画圆形边框，圆心横坐标、纵坐标，半径，颜色
void LCD_draw_dot(unsigned char x,unsigned char y,unsigned int color_dat);	//画点，横坐标，纵坐标，颜色

void display_pic(void);//显示图片

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
