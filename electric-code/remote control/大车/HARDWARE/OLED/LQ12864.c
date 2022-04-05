#include "sys.h"
#include "picture.h"
#include "lqTFT12864.h"
#include "LQ12864.h"

void time_delay_ms(unsigned int ms)
{
  unsigned int i,j;
  while(ms--)
  {
    for(i=0;i<120;i++)
			for(j=0;j<100;j++)
		{
		}
		
  }
}
void OLED_GPIO()
{  	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
/**********************************************************
函数名称：LCD_init()
入口参数：无
出口参数：无
时间：2016/11/12
功能说明：器件初始化
其他说明：无
**********************************************************/
void LCD_init(void)
{ 		
    OLED_GPIO();
    LQTFT_RST=0;                                
    time_delay_ms(200);
    LQTFT_RST=1;                     
    time_delay_ms(50); 
    LCD_write_command(0x11);//start OSC
    time_delay_ms(100);  

    LCD_write_command(0xB1);//frame rate control
    LCD_write_para8(0x05);   
    LCD_write_para8(0x3A);
    LCD_write_command(0x36);//镜像控制?
    LCD_write_para8(0x08);  //	
    LCD_write_command(0xB2);
    LCD_write_para8(0x05);    
    LCD_write_para8(0x3A);	
    LCD_write_command(0xB3);
    LCD_write_para8(0x05);    
    LCD_write_para8(0x3A);
    LCD_write_para8(0x05);   
    LCD_write_para8(0x3A);	
    LCD_write_command(0xB4);//inversion setting
    LCD_write_para8(0x00);  //0x00=dot inversion	
    LCD_write_command(0xC0);//Power Control 1
    LCD_write_para8(0xAA);  //AVDD=5V;GVDD=4.2V
    LCD_write_para8(0x0A);  //GVCL=-4.2V
    LCD_write_para8(0x84);  //Mode=AUTO;VRHP=VRHN=0	
    LCD_write_command(0xC1);//Power Control 2
    LCD_write_para8(0xC0);  //V25=2.4V;VGH=2AVDD+VGH25-0.5=11.9V;VGL=-7.5V	                           
    LCD_write_command(0xC2);//Power Control 3
    LCD_write_para8(0x0D);  //
    LCD_write_para8(0x00);  //	
    LCD_write_command(0xC3);//Power Control 4
    LCD_write_para8(0x8D);  //
    LCD_write_para8(0x2A);  //	
    LCD_write_command(0xC4);//Power Control 5
    LCD_write_para8(0x8D);
    LCD_write_para8(0xEE);
    LCD_write_command(0xC5);//VCOM setting
    LCD_write_para8(0x06);  //
    LCD_write_command(0xE0);
    LCD_write_para8(0x06);
    LCD_write_para8(0x1C);
    LCD_write_para8(0x10);	
    LCD_write_para8(0x14);
    LCD_write_para8(0x30);	
    LCD_write_para8(0x2B);
    LCD_write_para8(0x24);
    LCD_write_para8(0x29);
    LCD_write_para8(0x28);
    LCD_write_para8(0x26);
    LCD_write_para8(0x2E);
    LCD_write_para8(0x39);
    LCD_write_para8(0x00);
    LCD_write_para8(0x02);
    LCD_write_para8(0x01);
    LCD_write_para8(0x10);	
    LCD_write_command(0xE1);
    LCD_write_para8(0x06);
    LCD_write_para8(0x1C);
    LCD_write_para8(0x10);	
    LCD_write_para8(0x14);
    LCD_write_para8(0x30);	
    LCD_write_para8(0x2B);
    LCD_write_para8(0x24);
    LCD_write_para8(0x29);
    LCD_write_para8(0x28);
    LCD_write_para8(0x26);
    LCD_write_para8(0x2E);
    LCD_write_para8(0x39);
    LCD_write_para8(0x00);
    LCD_write_para8(0x02);
    LCD_write_para8(0x01);
    LCD_write_para8(0x10);
    LCD_write_command(0x3A);//
    LCD_write_para8(0x05);  //RGB 16-bit	
    LCD_write_command(0x29);//display on	
    time_delay_ms(10);   
    LCD_display_full(RED);
}


/******************DRIVE IC寄存器寻址 8080 SERIES*******************************/
void LCD_write_command(unsigned char cmd)
{
	unsigned char i;

	LQTFT_A0=0;	 						//A0=0发送命令
	LQTFT_CS=0;
	for(i=0;i<8;i++)
	{
		LQTFT_SCK=0;
		if((cmd&0x80)==0x80)	  	//高位先发送
			LQTFT_SDA=1;
		else		
			LQTFT_SDA=0;
		//LQTFT_SDA =((0x80&cmd>0)?1:0);
		LQTFT_SCK = 1;
		cmd = (cmd<<1);

	}
    LQTFT_CS=1;
}

/*****************DRIVE IC寄存器写数据 8080 SERIES******************************/
void LCD_write_para8(unsigned char dat)
{  
	unsigned char i;

	LQTFT_A0=1;			   				//A0=1发送数据
	LQTFT_CS=0;				
	for(i=0;i<8;i++)
	{
		LQTFT_SCK=0;
		if((dat&0x80)==0x80)
			LQTFT_SDA=1;
		else
			LQTFT_SDA=0;  	
		//LQTFT_SDA =((0x80&dat>0)?1:0);
		LQTFT_SCK = 1;
		dat = (dat<<1);

	}
	LQTFT_CS=1;
}
 
/****************DRIVE IC GDRAM 写数据 16BITS 8080 SERIES*****************************/
void LCD_write_para16(unsigned int dat)
{  		 
	unsigned char i,buf;
	LQTFT_A0=1;
	LQTFT_CS=0;				

	buf = (unsigned char)(0xFF&(dat>>8));
	for(i=0;i<8;i++)
	{
		LQTFT_SCK=0;
		if((buf&0x80)==0x80)
			LQTFT_SDA=1;
		else
			LQTFT_SDA=0;  		
		//LQTFT_SDA =((0x80&buf>0)?1:0);
		LQTFT_SCK=1;
		buf=(buf<<1);

	}
	LQTFT_CS=1;

	buf = (unsigned char)(0xFF&dat);
	LQTFT_CS=0;
	for(i=0;i<8;i++)
	{
		LQTFT_SCK=0;
	  if((buf&0x80)==0x80)
			LQTFT_SDA=1;
		else
			LQTFT_SDA=0;
		//LQTFT_SDA =((0x80&buf>0)?1:0);
		LQTFT_SCK =1;
		buf=(buf<<1);
	}
	LQTFT_CS=1; 
}


/**********************************************************
函数名称：LCD_SetPos()
入口参数：起始终止坐标
出口参数：无
时间：2016/11/12
功能说明：重新定位输入信息位置
其他说明：无
**********************************************************/
void LCD_SetPos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye)
{
	LCD_write_command(0x2A);      	//Colulm addRSTs set
	LCD_write_para8(0x00);		//行起始坐标高8位，始终为零
	LCD_write_para8(xs);		//行起始坐标低8位
	LCD_write_para8(0x00);		//行终止坐标高8位，始终为零
	LCD_write_para8(xe);		//行终止坐标低8位
	LCD_write_command(0x2B);      	//Colulm addRSTs set
	LCD_write_para8(0x00);		//列起始坐标高8位，始终为零
	LCD_write_para8(ys);		//列起始坐标低8位
	LCD_write_para8(0x00);		//列终止坐标高8位，始终为零
	LCD_write_para8(ye);		//列终止坐标低8位
	LCD_write_command(0x2C);      	//GRAM接收MCU数据或命令
}


/**********************************************************
函数名称：LCD_address_rst()
入口参数：无
出口参数：无
时间：2016/11/12
功能说明：重置地址
其他说明：无
**********************************************************/
void LCD_address_rst(void)
{
    LCD_write_command(0x2a);            //配置MCU可操作的LCD内部RAM横坐标起始、结束参数
    LCD_write_para8(0x00);		//横坐标起始地址0x0000
    LCD_write_para8(0x00);		
    LCD_write_para8(0x00);		//横坐标结束地址0x007f(127)
    LCD_write_para8(0x7f);   
    LCD_write_command(0x2b);            //配置MCU可操作的LCD内部RAM纵坐标起始结束参数
    LCD_write_para8(0x00);		//纵坐标起始地址0x0000
    LCD_write_para8(0x30);
    LCD_write_para8(0x00);		//纵坐标结束地址0x009f(159)
    LCD_write_para8(0x70);
    LCD_write_command(0x2C);      	//GRAM接收MCU数据或命令
}


/*********************************************************/
//延时函数
void time_delay(unsigned int t)
{
	unsigned int i,j;

	for(j=0;j<t;j++)
		for(i=0;i<1000;i++);
}

/**********************************************************
函数名称：LCD_display_full()
入口参数：color为常用的需要显示的颜色(已经提前定义)，color为
		  NULL时，则可将不常用颜色放入dat中加以显示
出口参数：无
时间：2012-09-09
功能说明：全屏显示单色画面
其他说明：无
**********************************************************/
void LCD_display_full(unsigned int color)
{
  	unsigned int i,j;
        
        LCD_SetPos(0,0,XMAX,YMAX);
	for(i=0;i<XMAX;i++)
	{ 
	   	for(j=0;j<YMAX;j++)
		{			
		  LCD_write_para16(color);
		}
	}    
}
void LCD_CLS(void)
{
    LCD_display_full(BLACK);
}

/**********************************************************
函数名称：LCD_draw_part()
入口参数：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色word
出口参数：无
时间：2012-09-09
功能说明：填充矩形区域
其他说明：0<=xs<xe<=127
		  0<=ys<ye<=159
**********************************************************/
void LCD_draw_part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat)
{
	unsigned char i,j;
        int k;

	LCD_SetPos(xs,ys,xe,ye);	
	for(j=0;j<(ye-ys+1);j++)
    {
    	for(i=0;i<(xe-xs+1);i++)
		{
			LCD_write_para16(color_dat);
                        //慢动作设置，时间越长，打点越慢！  
                        for(k=0;k<300;k++);
		}
	}   
}

/**********************************************************
函数名称：LCD_draw_rectangle()
入口参数：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色word
出口参数：无
时间：2012-09-09
功能说明：画矩形边框
其他说明：0<=xs<xe<=127
		  0<=ys<ye<=159
**********************************************************/
void LCD_draw_rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat)
{
	LCD_draw_line(xs,ys,xs,ye,color_dat);	  //画矩形左边
	LCD_draw_line(xe,ys,xe,ye,color_dat);	  //画矩形右边
	LCD_draw_line(xs,ys,xe,ys,color_dat);	  //画矩形上边
	LCD_draw_line(xs,ye,xe,ye,color_dat);	  //画矩形下边
        time_delay_ms(1); //慢动作设置，时间越长，打点越慢！  
}


/**********************************************************
函数名称：LCD_draw_circle()
入口参数：圆心横坐标(0-127)，纵坐标(0-159),半径(0-128),显示颜色word
出口参数：无
时间：2012-09-09
功能说明：画圆形边框(仅支持屏幕内画圆)
其他说明：0<=x<=127
		  0<=y<=159
**********************************************************/
void LCD_draw_circle(unsigned char x,unsigned char y,unsigned char r,unsigned int color_dat)
{
	unsigned char dx, dy = r; 

	if((x>=r) &&((XMAX-x)>=r) && (y>=r) && ((YMAX-y)>=r))		//确定所画圆在屏幕范围内，没有超出最外边，(暂不支持与屏幕边相交)
	{
		for(dx = 0; dx <= r; dx++)
		{
			while((r * r + 1 - dx * dx) < (dy * dy)) dy--;
			LCD_draw_dot(x + dx, y - dy, color_dat);
			LCD_draw_dot(x - dx, y - dy, color_dat);
			LCD_draw_dot(x - dx, y + dy, color_dat);
			LCD_draw_dot(x + dx, y + dy, color_dat);

			LCD_draw_dot(x + dy, y - dx, color_dat);
			LCD_draw_dot(x - dy, y - dx, color_dat);
			LCD_draw_dot(x - dy, y + dx, color_dat);
			LCD_draw_dot(x + dy, y + dx, color_dat);
                        time_delay_ms(1); //慢动作设置，时间越长，打点越慢！  
	        }
	}
}


/**********************************************************
函数名称：LCD_draw_line()
入口参数：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色word
出口参数：无
时间：2012-09-09
功能说明：画直线
其他说明：0<=xs<xe<=127
		  0<=ys<ye<=159
**********************************************************/
void LCD_draw_line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned int color_dat)
{
	unsigned char i,ds;
	int dx,dy,inc_x, inc_y;
	int xerr = 0, yerr = 0;				//初始化变量

	if(xs==xe)	  						//如果是画垂直线则只需对竖直坐标计数
	{
    	LCD_SetPos(xs,ys,xe,ye);
		for(i=0;i<(ye-ys+1);i++)
		{
			LCD_write_para16(color_dat);
                        time_delay_ms(1); //慢动作设置，时间越长，打点越慢！  
		}
	}
	else if(ys==ye)						//如果是水平线则只需要对水平坐标计数
        {
 		LCD_SetPos(xs,ys,xe,ye);
 	  	for(i=0;i<(xe-xs+1);i++)
		{
			LCD_write_para16(color_dat);
                        time_delay_ms(1); //慢动作设置，时间越长，打点越慢！  
		}
	} 
	else											//如果是斜线，则重新计算，使用画点函数画出直线
	{
    	dx = xe - xs;								//计算坐标增量
    	dy = ye - ys;

		if(dx > 0) inc_x = 1;						//设置单步方向
		else 
		{
			inc_x = -1; dx = -dx;
		}
		if(dy > 0) inc_y = 1;						//设置单步方向
		else
	        {
			inc_y = -1; dy = -dy;
		}

		if(dx > dy) ds = dx;						//选取基本增量坐标轴
		else		ds = dy;

		for(i = 0; i <= ds+1; i++)					//画线输出
		{
			LCD_draw_dot(xs, ys,color_dat);	//画点
                        time_delay_ms(1); //慢动作设置，时间越长，打点越慢！  
			xerr += dx;
			yerr += dy;
			if(xerr > ds)
			{
				xerr -= ds;
				xs   += inc_x;
			}
			if(yerr > ds)
			{
				yerr -= ds;
				ys   += inc_y;
			}
		}
	}  
}

/**********************************************************
函数名称：LCD_draw_dot()
入口参数：起始横坐标(0-127)，纵坐标(0-63),显示颜色word
出口参数：无
时间：2012-09-09
功能说明：画点
其他说明：0<=x<=127
	  0<=y<=63
**********************************************************/
void LCD_draw_dot(unsigned char x,unsigned char y,unsigned int color_dat)
{
	LCD_SetPos(x,y,x,y);
	LCD_write_para16(color_dat);
}

/****************显示图片（从单片机的DATA区）********************************/	
void display_pic()
{
	unsigned int i,j,k=0;

	LCD_SetPos(2,0,XMAX-3,YMAX-1);
	for(i=0;i<XMAX;i++)
	{ 
	   	for(j=0;j<YMAX;j++)
		{			
		  LCD_write_para16(((unsigned int)PIC[k])<<8|PIC[k+1]);			
		  k+=2; 
		}
	}    
 }

void show_pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned char *ppic)
{
	unsigned int i,j,k=0;

	LCD_SetPos(xs,ys,xe,ye);
	for(i=0;i<ye-ys;i++)
	{ 
	   	for(j=0;j<xe-xs;j++)
		{			
		  LCD_write_para16(((unsigned int)ppic[k])<<8|ppic[k+1]);			
		  k+=2;
		}
	}    
 }

/*--------------------------------------------------------------*/
//	  字体:	8*6字体
//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
//查表方法: 要显示的ASCII码-32就可以得到字库码的指针
unsigned char Font_code8[][6] = {
{0x00,0x00,0x00,0x00,0x00,0x00},// (0)
{0x00,0x00,0x00,0x4F,0x00,0x00},//!(1)
{0x00,0x00,0x07,0x00,0x07,0x00},//"(2)
{0x00,0x14,0x7F,0x14,0x7F,0x14},//#(3)
{0x00,0x24,0x2A,0x7F,0x2A,0x12},//$(4)
{0x00,0x23,0x13,0x08,0x64,0x62},//%(5)
{0x00,0x36,0x49,0x55,0x22,0x50},//&(6)
{0x00,0x00,0x05,0x03,0x00,0x00},//'(7)
{0x00,0x00,0x1C,0x22,0x41,0x00},//((8)
{0x00,0x00,0x41,0x22,0x1C,0x00},//)(9)
{0x00,0x14,0x08,0x3E,0x08,0x14},//*(10)
{0x00,0x08,0x08,0x3E,0x08,0x08},//+(11)
{0x00,0x00,0x50,0x30,0x00,0x00},//,(12)
{0x00,0x08,0x08,0x08,0x08,0x08},//-(13)
{0x00,0x00,0x60,0x60,0x00,0x00},//.(14)
{0x00,0x20,0x10,0x08,0x04,0x02},///(15)
{0x00,0x3E,0x51,0x49,0x45,0x3E},//0(16)
{0x00,0x00,0x42,0x7F,0x40,0x00},//1(17)
{0x00,0x42,0x61,0x51,0x49,0x46},//2(18)
{0x00,0x21,0x41,0x45,0x4B,0x31},//3(19)
{0x00,0x18,0x14,0x12,0x7F,0x10},//4(20)
{0x00,0x27,0x45,0x45,0x45,0x39},//5(21)
{0x00,0x3C,0x4A,0x49,0x49,0x30},//6(22)
{0x00,0x01,0x71,0x09,0x05,0x03},//7(23)
{0x00,0x36,0x49,0x49,0x49,0x36},//8(24)
{0x00,0x06,0x49,0x49,0x29,0x1E},//9(25)
{0x00,0x00,0x36,0x36,0x00,0x00},//:(26)
{0x00,0x00,0x56,0x36,0x00,0x00},//;(27)
{0x00,0x08,0x14,0x22,0x41,0x00},//<(28)
{0x00,0x14,0x14,0x14,0x14,0x14},//=(29)
{0x00,0x00,0x41,0x22,0x14,0x08},//>(30)
{0x00,0x02,0x01,0x51,0x09,0x06},//?(31)
{0x00,0x32,0x49,0x79,0x41,0x3E},//@(32)
{0x00,0x7E,0x11,0x11,0x11,0x7E},//A(33)
{0x00,0x7F,0x49,0x49,0x49,0x3E},//B(34)
{0x00,0x3E,0x41,0x41,0x41,0x22},//C(35)
{0x00,0x7F,0x41,0x41,0x22,0x1C},//D(36)
{0x00,0x7F,0x49,0x49,0x49,0x41},//E(37)
{0x00,0x7F,0x09,0x09,0x09,0x01},//F(38)
{0x00,0x3E,0x41,0x49,0x49,0x7A},//G(39)
{0x00,0x7F,0x08,0x08,0x08,0x7F},//H(40)
{0x00,0x00,0x41,0x7F,0x41,0x00},//I(41)
{0x00,0x20,0x40,0x41,0x3F,0x01},//J(42)
{0x00,0x7F,0x08,0x14,0x22,0x41},//K(43)
{0x00,0x7F,0x40,0x40,0x40,0x40},//L(44)
{0x00,0x7F,0x02,0x04,0x02,0x7F},//M(45)
{0x00,0x7F,0x04,0x08,0x10,0x7F},//N(46)
{0x00,0x3E,0x41,0x41,0x41,0x3E},//O(47)
{0x00,0x7F,0x09,0x09,0x09,0x06},//P(48)
{0x00,0x3E,0x41,0x51,0x21,0x5E},//Q(49)
{0x00,0x7F,0x09,0x19,0x29,0x46},//R(50)
{0x00,0x46,0x49,0x49,0x49,0x31},//S(51)
{0x00,0x01,0x01,0x7F,0x01,0x01},//T(52)
{0x00,0x3F,0x40,0x40,0x40,0x3F},//U(53)
{0x00,0x1F,0x20,0x40,0x20,0x1F},//V(54)
{0x00,0x3F,0x40,0x38,0x40,0x3F},//W(55)
{0x00,0x63,0x14,0x08,0x14,0x63},//X(56)
{0x00,0x03,0x04,0x78,0x04,0x03},//Y(57)
{0x00,0x61,0x51,0x49,0x45,0x43},//Z(58)
{0x00,0x00,0x7F,0x41,0x41,0x00},//[(59)
{0x00,0x15,0x16,0x7C,0x16,0x15},//\(60)
{0x00,0x00,0x41,0x41,0x7F,0x00},//](61)
{0x00,0x04,0x02,0x01,0x02,0x04},//^(62)
{0x00,0x40,0x40,0x40,0x40,0x40},//_(63)
{0x00,0x00,0x01,0x02,0x04,0x00},//`(64)
{0x00,0x20,0x54,0x54,0x54,0x78},//a(65)
{0x00,0x7F,0x48,0x44,0x44,0x38},//b(66)
{0x00,0x38,0x44,0x44,0x44,0x20},//c(67)
{0x00,0x38,0x44,0x44,0x48,0x7F},//d(68)
{0x00,0x38,0x54,0x54,0x54,0x18},//e(69)
{0x00,0x08,0x7E,0x09,0x01,0x02},//f(70)
{0x00,0x0C,0x52,0x52,0x52,0x3E},//g(71)
{0x00,0x7F,0x08,0x04,0x04,0x78},//h(72)
{0x00,0x00,0x44,0x7D,0x40,0x00},//i(73)
{0x00,0x20,0x40,0x44,0x3D,0x00},//j(74)
{0x00,0x7F,0x10,0x28,0x44,0x00},//k(75)
{0x00,0x00,0x41,0x7F,0x40,0x00},//l(76)
{0x00,0x7E,0x02,0x0C,0x02,0x7C},//m(77)
{0x00,0x7E,0x04,0x02,0x02,0x7C},//n(78)
{0x00,0x38,0x44,0x44,0x44,0x38},//o(79)
{0x00,0x7C,0x14,0x14,0x14,0x08},//p(80)
{0x00,0x08,0x14,0x14,0x18,0x7C},//q(81)
{0x00,0x7C,0x08,0x04,0x04,0x08},//r(82)
{0x00,0x48,0x54,0x54,0x54,0x20},//s(83)
{0x00,0x04,0x3F,0x44,0x40,0x20},//t(84)
{0x00,0x3C,0x40,0x40,0x20,0x7C},//u(85)
{0x00,0x1C,0x20,0x40,0x20,0x1C},//v(86)
{0x00,0x3C,0x40,0x30,0x40,0x3C},//w(87)
{0x00,0x44,0x28,0x10,0x28,0x44},//x(88)
{0x00,0x0C,0x50,0x50,0x50,0x3C},//y(89)
{0x00,0x44,0x64,0x54,0x4C,0x44},//z(90)
{0x00,0x00,0x08,0x36,0x41,0x00},//{(91)
{0x00,0x00,0x00,0x7F,0x00,0x00},//|(92)
{0x00,0x00,0x41,0x36,0x08,0x00},//}(93)
{0x00,0x08,0x04,0x08,0x10,0x08},//~(94)
{0x00,0x08,0x08,0x2A,0x1C,0x08},//→(127)
{0x00,0x08,0x1C,0x2A,0x08,0x08},//←(128)
{0x00,0x04,0x02,0x7F,0x02,0x04},//↑(129)
{0x00,0x10,0x20,0x7F,0x20,0x10},//↓(130)
{0x00,0x1C,0x2A,0x32,0x2A,0x1C},//笑面(131)
{0x00,0x1C,0x22,0x44,0x22,0x1C}};//爱心(132)   


/*--------------------------------------------------------------*/
//	  字体:	GulimChe12
//取码规则:	高位在前, 行列扫描, 阴码(1-亮, 0-灭)
//查表方法: 要显示的ASCII码-32就可以得到字库码的指针

unsigned char Font_code16[][16] = {
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},// (0)
{0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x10,0x10,0x00,0x00},//!(1)
{0x00,0x00,0x24,0x24,0x24,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//"(3)
{0x00,0x00,0x48,0x48,0x48,0xFE,0x24,0x24,0x24,0x24,0x24,0x7F,0x12,0x12,0x12,0x00},//#(4)
{0x00,0x10,0x7C,0x92,0x92,0x12,0x14,0x18,0x30,0x50,0x90,0x92,0x92,0x7C,0x10,0x00},//$(5)
{0x00,0x00,0x8C,0x92,0x52,0x32,0x2C,0x10,0x08,0x68,0x94,0x92,0x92,0x60,0x00,0x00},//%(6)
{0x00,0x00,0x18,0x24,0x24,0x24,0x18,0x98,0xA4,0xA2,0x42,0x42,0xA2,0x9C,0x00,0x00},//&(7)
{0x00,0x00,0x10,0x10,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//'(8)
{0x00,0x20,0x10,0x08,0x08,0x04,0x04,0x04,0x04,0x04,0x04,0x08,0x08,0x10,0x20,0x00},//((9)
{0x00,0x04,0x08,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x08,0x04,0x00},//)(10)
{0x00,0x00,0x00,0x00,0x00,0x08,0x2A,0x1C,0x14,0x22,0x00,0x00,0x00,0x00,0x00,0x00},//*(11)
{0x00,0x00,0x00,0x00,0x10,0x10,0x10,0xFE,0x10,0x10,0x10,0x00,0x00,0x00,0x00,0x00},//+(12)
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x08,0x00},//,(13)
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//-(14)
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x00,0x00},//.(15)
{0x00,0x00,0x40,0x40,0x20,0x20,0x10,0x10,0x08,0x08,0x04,0x04,0x02,0x02,0x00,0x00},///(16)
{0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00},//0(17)
{0x00,0x00,0x10,0x1C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x3C,0x42,0x42,0x40,0x40,0x20,0x10,0x08,0x04,0x02,0x02,0x7E,0x00,0x00},
{0x00,0x00,0x3C,0x42,0x42,0x40,0x40,0x3C,0x40,0x40,0x40,0x42,0x42,0x3C,0x00,0x00},
{0x00,0x00,0x60,0x60,0x50,0x50,0x48,0x48,0x44,0x44,0xFE,0x40,0x40,0x40,0x00,0x00},
{0x00,0x00,0x7E,0x02,0x02,0x02,0x3E,0x42,0x40,0x40,0x40,0x42,0x42,0x3C,0x00,0x00},
{0x00,0x00,0x3C,0x42,0x42,0x02,0x02,0x3E,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},
{0x00,0x00,0x7E,0x40,0x20,0x20,0x10,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
{0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},
{0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x7C,0x40,0x40,0x42,0x42,0x3C,0x00,0x00},//9(25)
{0x00,0x00,0x00,0x00,0x08,0x08,0x00,0x00,0x00,0x00,0x08,0x08,0x00,0x00,0x00,0x00},//:(26)
{0x00,0x00,0x00,0x00,0x10,0x10,0x00,0x00,0x00,0x00,0x10,0x10,0x10,0x08,0x00,0x00},//;(27)
{0x00,0x00,0x00,0x40,0x30,0x08,0x06,0x01,0x06,0x08,0x30,0x40,0x00,0x00,0x00,0x00},//<(28)
{0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00},//=(29)
{0x00,0x00,0x00,0x02,0x0C,0x10,0x60,0x80,0x60,0x10,0x0C,0x02,0x00,0x00,0x00,0x00},//>(30)
{0x00,0x00,0x38,0x44,0x82,0x82,0x40,0x20,0x10,0x10,0x10,0x00,0x10,0x10,0x00,0x00},//?(31)
{0x00,0x00,0x38,0x44,0x82,0xB2,0xAA,0xAA,0xAA,0xAA,0xD2,0x02,0x84,0x78,0x00,0x00},//@(32)
{0x00,0x00,0x10,0x10,0x10,0x28,0x28,0x28,0x44,0x44,0x7C,0x82,0x82,0x82,0x00,0x00},//A(33)
{0x00,0x00,0x3E,0x42,0x82,0x82,0x42,0x3E,0x42,0x82,0x82,0x82,0x42,0x3E,0x00,0x00},
{0x00,0x00,0x38,0x44,0x82,0x82,0x02,0x02,0x02,0x02,0x82,0x82,0x44,0x38,0x00,0x00},
{0x00,0x00,0x3E,0x42,0x42,0x82,0x82,0x82,0x82,0x82,0x82,0x42,0x42,0x3E,0x00,0x00},
{0x00,0x00,0xFE,0x02,0x02,0x02,0x02,0x7E,0x02,0x02,0x02,0x02,0x02,0xFE,0x00,0x00},
{0x00,0x00,0xFE,0x02,0x02,0x02,0x02,0x7E,0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x00},
{0x00,0x00,0x38,0x44,0x82,0x02,0x02,0x02,0xE2,0x82,0x82,0x82,0xC4,0xB8,0x00,0x00},
{0x00,0x00,0x82,0x82,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x82,0x82,0x82,0x00,0x00},
{0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x22,0x22,0x1C,0x00,0x00},
{0x00,0x00,0x82,0x42,0x22,0x12,0x0A,0x06,0x06,0x0A,0x12,0x22,0x42,0x82,0x00,0x00},
{0x00,0x00,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x7E,0x00,0x00},
{0x00,0x00,0x82,0x82,0xC6,0xC6,0xAA,0xAA,0x92,0x92,0x92,0x82,0x82,0x82,0x00,0x00},
{0x00,0x00,0x82,0x86,0x86,0x8A,0x8A,0x92,0x92,0xA2,0xA2,0xC2,0xC2,0x82,0x00,0x00},
{0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00},
{0x00,0x00,0x3E,0x42,0x82,0x82,0x82,0x42,0x3E,0x02,0x02,0x02,0x02,0x02,0x00,0x00},
{0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xA2,0x44,0xB8,0x00,0x00},
{0x00,0x00,0x3E,0x42,0x82,0x82,0x82,0x42,0x3E,0x42,0x82,0x82,0x82,0x82,0x00,0x00},
{0x00,0x00,0x7C,0x82,0x82,0x02,0x04,0x18,0x20,0x40,0x80,0x82,0x82,0x7C,0x00,0x00},
{0x00,0x00,0xFE,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x7C,0x00,0x00},
{0x00,0x00,0x82,0x82,0x82,0x44,0x44,0x44,0x28,0x28,0x28,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x92,0x92,0x92,0x92,0x92,0xAA,0xAA,0xAA,0x44,0x44,0x44,0x44,0x00,0x00},
{0x00,0x00,0x82,0x82,0x44,0x44,0x28,0x10,0x28,0x28,0x44,0x44,0x82,0x82,0x00,0x00},
{0x00,0x00,0x82,0x82,0x44,0x44,0x28,0x28,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x7E,0x40,0x20,0x20,0x10,0x10,0x08,0x08,0x04,0x04,0x02,0x7E,0x00,0x00},
{0x00,0x78,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x78,0x00},
{0x00,0x00,0x92,0x92,0x92,0xFF,0x92,0xAA,0xAA,0xAA,0x44,0x44,0x44,0x44,0x00,0x00},
{0x00,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x00},
{0x00,0x10,0x28,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00},
{0x00,0x04,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x40,0x7C,0x42,0x42,0x42,0xBC,0x00,0x00},
{0x00,0x00,0x02,0x02,0x02,0x02,0x3A,0x46,0x82,0x82,0x82,0x82,0x46,0x3A,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x84,0x02,0x02,0x02,0x02,0x84,0x78,0x00,0x00},
{0x00,0x00,0x80,0x80,0x80,0x80,0xB8,0xC4,0x82,0x82,0x82,0x82,0xC4,0xB8,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x82,0xFE,0x02,0x02,0x84,0x78,0x00,0x00},
{0x00,0x00,0x70,0x08,0x08,0x08,0x7E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x64,0x42,0x42,0x42,0x64,0x58,0x42,0x3C,0x00},
{0x00,0x00,0x02,0x02,0x02,0x02,0x3A,0x46,0x42,0x42,0x42,0x42,0x42,0x42,0x00,0x00},
{0x00,0x00,0x10,0x10,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x10,0x10,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x0C,0x00},
{0x00,0x00,0x02,0x02,0x02,0x42,0x22,0x12,0x0A,0x06,0x0A,0x12,0x22,0x42,0x00,0x00},
{0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x6E,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x3A,0x46,0x42,0x42,0x42,0x42,0x42,0x42,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x3A,0x46,0x82,0x82,0x82,0x46,0x3A,0x02,0x02,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xB8,0xC4,0x82,0x82,0x82,0xC4,0xB8,0x80,0x80,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x74,0x0C,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x02,0x0C,0x30,0x40,0x42,0x3C,0x00,0x00},
{0x00,0x00,0x08,0x08,0x08,0x08,0x7E,0x08,0x08,0x08,0x08,0x08,0x08,0x70,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x7C,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x82,0x44,0x44,0x28,0x28,0x10,0x10,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x92,0x92,0x92,0xAA,0x6C,0x44,0x44,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x44,0x28,0x10,0x10,0x28,0x44,0x82,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x82,0x44,0x44,0x28,0x28,0x10,0x10,0x0E,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x40,0x20,0x10,0x08,0x04,0x02,0x7E,0x00,0x00},
{0x00,0x60,0x10,0x10,0x10,0x10,0x10,0x0C,0x10,0x10,0x10,0x10,0x10,0x10,0x60,0x00},
{0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00},
{0x00,0x06,0x08,0x08,0x08,0x08,0x08,0x30,0x08,0x08,0x08,0x08,0x08,0x08,0x06,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x8C,0x72,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};	
unsigned char hanzi_Idx[] = 
{
	"北京龙邱丘智能科技淘宝店液晶单片机智能车温度高海拔测试校准取消按键绝对相：°"
};
unsigned char hanzi[] = {	
//取码规则:	高位在前, 行扫描, 阴码(1-亮, 0-灭)
//查表方法: 要显示的ASCII码-32就可以得到字库码的指针 微软雅黑16*12
0x00,0x00,0x00,0x00,0x08,0x80,0x08,0x80,0x08,0x8C,0xF8,0xF0,
0x08,0x80,0x08,0x80,0x38,0x84,0xC8,0x84,0x08,0xF8,0x00,0x00,//北0
0x00,0x00,0x00,0x00,0x03,0x00,0xFF,0xFC,0x3F,0xF0,0x20,0x10,
0x20,0x10,0x3F,0xF0,0x19,0x60,0x31,0x18,0xC7,0x0C,0x00,0x00,//京1
0x00,0x00,0x00,0x00,0x04,0x60,0x04,0x30,0x7F,0xFC,0x04,0x88,
0x04,0x90,0x08,0xE0,0x09,0x84,0x36,0x84,0x60,0xF8,0x00,0x00,//龙2
0x00,0x00,0x0E,0x00,0x78,0xF8,0x40,0x88,0x40,0x90,0x7E,0xA0,
0x48,0x90,0x48,0x88,0x48,0x88,0x4A,0xF0,0xF4,0x80,0x00,0x80,//邱3
0x00,0x00,0x00,0x00,0x00,0x78,0x1F,0x80,0x10,0x00,0x10,0x00,
0x1F,0xF8,0x10,0x40,0x10,0x40,0x10,0x40,0x7F,0xFC,0x00,0x00,//丘4
0x00,0x00,0x20,0x00,0x7E,0xF8,0x90,0x88,0xFE,0x88,0x38,0xF8,
0xC6,0x00,0x3F,0xF0,0x3F,0xF0,0x20,0x10,0x3F,0xF0,0x00,0x00,//智5
0x00,0x00,0x00,0x00,0x30,0x8C,0x46,0xF4,0xFF,0x84,0x7E,0xF8,
0x42,0x84,0x7E,0x98,0x7E,0xE4,0x42,0x84,0x4E,0xFC,0x00,0x00,//能6
0x00,0x00,0x00,0x00,0x00,0x10,0x7D,0x90,0x10,0xD0,0x7F,0x10,
0x19,0x90,0x37,0xFC,0x50,0x10,0x10,0x10,0x10,0x10,0x00,0x00,//科7
0x00,0x00,0x00,0x00,0x20,0x40,0xFF,0xFC,0x20,0x40,0x27,0xF8,
0x32,0x08,0xE1,0x10,0x20,0xE0,0x21,0xE0,0xEE,0x1C,0x00,0x00,//技8
0x00,0x00,0x00,0x00,0x62,0x00,0x37,0xF8,0x6A,0x08,0x37,0xE8,
0x0F,0xF8,0x24,0xA8,0x24,0xA8,0x47,0xE8,0x40,0x78,0x00,0x00,//淘9
0x00,0x00,0x00,0x00,0x03,0x00,0x7F,0xF8,0x40,0x08,0x7F,0xF8,
0x02,0x00,0x3F,0xF0,0x02,0x60,0x02,0x10,0x7F,0xF8,0x00,0x00,//宝10
0x00,0x00,0x00,0x00,0x01,0x80,0x3F,0xF8,0x21,0x00,0x21,0xF8,
0x21,0x00,0x2F,0xF0,0x28,0x10,0x68,0x10,0x4F,0xF0,0x00,0x00,//店11
0x00,0x00,0x00,0x00,0x60,0xC0,0x3F,0xFC,0x62,0x40,0x34,0xF8,
0x0D,0x48,0x36,0xB0,0x24,0x60,0x44,0xE0,0x47,0x1C,0x00,0x00,//液12
0x00,0x00,0x00,0x00,0x3F,0xF0,0x20,0x10,0x3F,0xF0,0x3F,0xF0,
0x7C,0xF8,0x44,0x88,0x7C,0xF8,0x44,0x88,0x7C,0xF8,0x44,0x88,//晶13
0x00,0x00,0x10,0x20,0x08,0xC0,0x3F,0xF0,0x22,0x10,0x3F,0xF0,
0x22,0x10,0x3F,0xF0,0x02,0x00,0x7F,0xF8,0x02,0x00,0x02,0x00,//单14
0x00,0x00,0x00,0x00,0x10,0x40,0x10,0x40,0x10,0x40,0x1F,0xFC,
0x10,0x00,0x1F,0xE0,0x10,0x20,0x20,0x20,0x60,0x20,0x00,0x00,//片15
0x00,0x00,0x20,0x00,0x23,0xE0,0xFA,0x20,0x22,0x20,0x72,0x20,
0x6A,0x20,0xA2,0x20,0xA2,0x24,0x24,0x24,0x28,0x38,0x00,0x00,//机16
0x00,0x00,0x20,0x00,0x7E,0xF8,0x90,0x88,0xFE,0x88,0x38,0xF8,
0xC6,0x00,0x3F,0xF0,0x3F,0xF0,0x20,0x10,0x3F,0xF0,0x00,0x00,//智17
0x00,0x00,0x00,0x00,0x30,0x8C,0x46,0xF4,0xFF,0x84,0x7E,0xF8,
0x42,0x84,0x7E,0x98,0x7E,0xE4,0x42,0x84,0x4E,0xFC,0x00,0x00,//能18
0x00,0x00,0x04,0x00,0x08,0x00,0x7F,0xF8,0x11,0x00,0x31,0x00,
0x3F,0xF0,0x01,0x00,0x01,0x00,0xFF,0xFC,0x01,0x00,0x01,0x00,//车19	   	
};
unsigned char hanzi16x16[] = {	
//取码规则:	高位在前, 行扫描, 阴码(1-亮, 0-灭)
//查表方法: 要显示的ASCII码-32就可以得到字库码的指针 宋体16*16加粗


0x06,0x60,0x06,0x60,0x06,0x60,0x06,0x66,0x06,0x6F,0x7E,0x7C,0x06,0x70,0x06,0x60,
0x06,0x60,0x06,0x60,0x0E,0x60,0x3E,0x63,0xF6,0x63,0x66,0x63,0x06,0x3F,0x00,0x00,//北0
0x03,0x00,0x01,0x8C,0x7F,0xFE,0x00,0x00,0x00,0x00,0x1F,0xF8,0x18,0x18,0x18,0x18,
0x1F,0xF8,0x01,0x80,0x0D,0xB8,0x1D,0x9C,0x39,0x8E,0x61,0x8C,0x07,0x80,0x03,0x00,//京1
0x03,0x00,0x03,0x60,0x03,0x30,0x03,0x30,0x7F,0xFF,0x03,0xC0,0x03,0xD8,0x03,0xD8,
0x06,0xF0,0x06,0xE0,0x0C,0xC0,0x0D,0xC0,0x1B,0xC3,0x30,0xC3,0x60,0x7F,0x00,0x00,//龙2
0x00,0x00,0x07,0xFF,0x3C,0x66,0x30,0x66,0x30,0x6C,0x3F,0xF8,0x36,0x6C,0x36,0x66,
0x36,0x63,0x36,0x63,0x36,0x63,0x3F,0xFF,0xF8,0x66,0x00,0x60,0x00,0x60,0x00,0x60,//邱3
0x00,0x00,0x18,0xF8,0x1F,0x80,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x0C,0x1F,0xFE,
0x18,0x60,0x18,0x60,0x18,0x60,0x18,0x60,0x18,0x66,0xFF,0xFF,0x00,0x00,0x00,0x00,//丘4
0x30,0x00,0x3F,0x80,0x6C,0x7E,0x0C,0x66,0xFF,0xE6,0x0E,0x66,0x0F,0x7E,0x19,0x80,
0x7F,0xF8,0x0C,0x18,0x0C,0x18,0x0F,0xF8,0x0C,0x18,0x0C,0x18,0x0F,0xF8,0x0C,0x18,//智5
0x30,0x60,0x38,0x60,0x36,0x6E,0x63,0x78,0xFF,0xE6,0x63,0x67,0x00,0x7E,0x7F,0x00,
0x63,0x60,0x7F,0x6E,0x63,0x78,0x7F,0x60,0x63,0x66,0x63,0x67,0x6F,0x3E,0x66,0x00,//能6
0x07,0x0C,0x7C,0xCC,0x0C,0x6C,0x0C,0x6C,0xFF,0x0C,0x1C,0xCC,0x1E,0x6C,0x3F,0x6C,
0x3C,0x0F,0x6C,0x7C,0xCF,0xCC,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,//科7
0x18,0x30,0x18,0x30,0x18,0x30,0xFF,0xFF,0x18,0x30,0x1E,0x30,0x1D,0xFE,0x39,0x8C,
0xF8,0xCC,0x18,0xD8,0x18,0x70,0x18,0x70,0x18,0xD8,0x19,0x8F,0x7F,0x06,0x30,0x00,//技8
0x31,0x80,0x19,0x80,0x1B,0xFE,0x03,0x06,0xC7,0x86,0x7B,0xFE,0x7E,0xC6,0x18,0xC6,
0x37,0xFE,0x30,0xC6,0xF6,0xDE,0x36,0xDE,0x37,0xFE,0x30,0x06,0x30,0x1E,0x30,0x0C,//淘9
0x03,0x00,0x01,0x80,0x7F,0xFF,0x60,0x06,0xC0,0x0C,0x3F,0xFC,0x01,0x80,0x01,0x80,
0x01,0x80,0x1F,0xFC,0x01,0xE0,0x01,0xB0,0x01,0xB8,0x01,0xB0,0x7F,0xFF,0x00,0x00,//宝10
0x01,0x80,0x00,0xC0,0x3F,0xFF,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xFE,0x30,0xC0,
0x30,0xC0,0x37,0xFC,0x36,0x0C,0x36,0x0C,0x66,0x0C,0x67,0xFC,0xC6,0x0C,0x00,0x00,//店11
0x60,0x60,0x30,0x30,0x37,0xFF,0x0D,0xB0,0xCD,0xB0,0x7B,0x7E,0x7B,0x66,0x1F,0xFC,
0x3F,0xDC,0x33,0x78,0xF3,0x30,0x33,0x38,0x33,0x78,0x33,0xCC,0x33,0x8F,0x33,0x06,//液12
0x00,0x00,0x0F,0xF8,0x0C,0x18,0x0F,0xF8,0x0C,0x18,0x0F,0xF8,0x0C,0x18,0x00,0x00,
0x7F,0x7F,0x63,0x63,0x7F,0x7F,0x63,0x63,0x63,0x63,0x7F,0x7F,0x63,0x63,0x00,0x00,//晶13
0x0C,0x30,0x07,0x38,0x06,0x60,0x3F,0xFC,0x31,0x8C,0x3F,0xFC,0x31,0x8C,0x31,0x8C,
0x3F,0xFC,0x31,0x8C,0x01,0x80,0xFF,0xFF,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,//单14
0x00,0x60,0x18,0x60,0x18,0x60,0x18,0x66,0x1F,0xFF,0x18,0x00,0x18,0x00,0x18,0x00,
0x1F,0xF8,0x18,0x18,0x18,0x18,0x18,0x18,0x30,0x18,0x30,0x18,0x60,0x18,0xC0,0x18,//片15
0x18,0x00,0x18,0xFC,0x18,0xCC,0xFF,0xCC,0x18,0xCC,0x18,0xCC,0x3C,0xCC,0x3E,0xCC,
0x7E,0xCC,0x78,0xCC,0xD9,0x8C,0x19,0x8F,0x1B,0x0F,0x1B,0x0F,0x1E,0x07,0x18,0x00,//机16
0x30,0x00,0x3F,0x80,0x6C,0x7E,0x0C,0x66,0xFF,0xE6,0x0E,0x66,0x0F,0x7E,0x19,0x80,
0x7F,0xF8,0x0C,0x18,0x0C,0x18,0x0F,0xF8,0x0C,0x18,0x0C,0x18,0x0F,0xF8,0x0C,0x18,//智17
0x30,0x60,0x38,0x60,0x36,0x6E,0x63,0x78,0xFF,0xE6,0x63,0x67,0x00,0x7E,0x7F,0x00,
0x63,0x60,0x7F,0x6E,0x63,0x78,0x7F,0x60,0x63,0x66,0x63,0x67,0x6F,0x3E,0x66,0x00,//能18
0x03,0x00,0x03,0x00,0x7F,0xFE,0x06,0x00,0x06,0xC0,0x0C,0xC0,0x18,0xC0,0x3F,0xFE,
0x00,0xC0,0x00,0xC0,0xFF,0xFF,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,//车19
0x60,0x00,0x37,0xF8,0x36,0x18,0x07,0xF8,0xDE,0x18,0x7E,0x18,0x1F,0xF8,0x30,0x00,
0x3F,0xFE,0xED,0xB6,0x6D,0xB6,0x6D,0xB6,0x6D,0xB6,0x6D,0xB6,0x7F,0xFF,0x60,0x00,//温20
0x01,0x80,0x00,0xC0,0x3F,0xFF,0x33,0x30,0x33,0x30,0x3F,0xFE,0x33,0x30,0x33,0xF0,
0x30,0x00,0x37,0xFC,0x33,0x18,0x31,0xB0,0x30,0xE0,0x61,0xB8,0x67,0x0F,0xDC,0x06,//度21
0x03,0x00,0x01,0x80,0xFF,0xFF,0x00,0x00,0x0F,0xF0,0x0C,0x30,0x0F,0xF0,0x00,0x00,
0x7F,0xFE,0x60,0x06,0x6F,0xF6,0x6C,0x36,0x6C,0x36,0x6F,0xF6,0x60,0x1E,0x60,0x0C,//高22
0x31,0x80,0x19,0x80,0x19,0xFF,0x03,0x00,0xDF,0xFC,0x7B,0xCC,0x7B,0x6C,0x1B,0x0C,
0x3F,0xFF,0x33,0xCC,0xF3,0x6C,0x33,0x0C,0x33,0xFF,0x30,0x0C,0x30,0x3C,0x30,0x18,//海23
0x18,0xC0,0x18,0xD8,0x18,0xCC,0xFE,0xC0,0x1B,0xFF,0x18,0xC0,0x1E,0xFE,0x1C,0xCC,
0x39,0xEC,0xF9,0xF8,0x19,0xB8,0x1B,0x30,0x1B,0x78,0x1E,0xCF,0x7F,0x86,0x30,0x00,//拔24
0x60,0x03,0x37,0xE3,0x36,0x63,0xC6,0x7B,0x67,0xFB,0x7F,0xFB,0x1F,0xFB,0x37,0xFB,
0x37,0xFB,0x37,0xFB,0xE7,0xFB,0x61,0x83,0x63,0xC3,0x63,0x63,0x66,0x6F,0x6C,0x06,//测25
0x00,0x30,0x60,0x3C,0x30,0x36,0x38,0x36,0x37,0xFF,0x00,0x30,0xF0,0x30,0x37,0xF0,
0x31,0xB0,0x31,0x98,0x31,0x98,0x31,0x8F,0x3D,0xEF,0x3F,0x07,0x30,0x03,0x00,0x00,//试26
0x18,0x60,0x18,0x30,0x18,0x30,0xFF,0xFF,0x18,0x00,0x3C,0xCC,0x3E,0xC6,0x79,0x8F,
0x79,0x8C,0xDB,0xD8,0x18,0xD8,0x18,0x70,0x18,0x70,0x19,0xDC,0x1F,0x0F,0x18,0x06,//校27
0x03,0x60,0x63,0xB0,0x33,0x36,0x3B,0xFF,0x37,0x30,0x0F,0x3C,0x1B,0xFE,0x1B,0x30,
0x1B,0x3C,0xF3,0xFE,0x33,0x30,0x33,0x30,0x33,0x36,0x33,0xFF,0x33,0x00,0x00,0x00,//准28
0x00,0x00,0xFF,0xC0,0x33,0x7F,0x33,0x66,0x3F,0x66,0x33,0x66,0x33,0x66,0x3F,0x3C,
0x33,0x3C,0x33,0xD8,0x37,0x98,0x3F,0x3C,0xE3,0x67,0x03,0xC6,0x03,0x00,0x03,0x00,//取29
0x30,0x60,0x18,0x60,0x1B,0x66,0xC1,0xEC,0x6D,0xF8,0x6F,0xFE,0x1B,0x06,0x1B,0x06,
0x33,0xFE,0x33,0x06,0xF3,0x06,0x33,0xFE,0x33,0x06,0x33,0x06,0x33,0x1E,0x33,0x0C,//消30
0x18,0x60,0x18,0x30,0x18,0x30,0x1B,0xFF,0xFF,0x06,0x18,0x60,0x1E,0x60,0x1F,0xFF,
0x38,0xCC,0xF9,0x98,0x19,0xD8,0x18,0x70,0x18,0x7C,0x19,0xCF,0x7F,0x06,0x30,0x00,//按31
0x30,0x30,0x30,0x30,0x37,0xFE,0x3D,0xB6,0x61,0xFF,0x7F,0x36,0xF7,0xFE,0x31,0xB0,
0xFD,0xFE,0x37,0xB0,0x33,0xFF,0x31,0xB0,0x3F,0xF0,0x3E,0x70,0x3C,0x1F,0x00,0x00,//键32
0x30,0xC0,0x38,0xC0,0x31,0xFE,0x61,0x8C,0x6F,0x18,0xFF,0xFE,0x1F,0xB6,0x31,0xB6,
0x61,0xB6,0xFD,0xFE,0x01,0x80,0x01,0x80,0x1F,0x83,0xF1,0x83,0x00,0xFF,0x00,0x00,//绝33
0x00,0x18,0x00,0x18,0x7E,0x18,0x06,0x18,0x07,0xFF,0x66,0x18,0x36,0x18,0x1D,0x98,
0x0C,0xD8,0x1E,0xF8,0x36,0xD8,0x33,0x18,0x63,0x18,0xC0,0x18,0x00,0x78,0x00,0x30,//对34
0x18,0x00,0x18,0xFE,0x18,0xC6,0x18,0xC6,0xFF,0xC6,0x1C,0xFE,0x3E,0xC6,0x3B,0xC6,
0x7B,0xC6,0x78,0xFE,0xD8,0xC6,0x18,0xC6,0x18,0xC6,0x18,0xC6,0x18,0xFE,0x18,0xC6,//相35
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x38,0x00,0x38,0x00,0x00,0x00,0x38,0x00,0x38,0x00,0x00,0x00,0x00,0x00,//：36
0x00,0x00,0x00,0x00,0x38,0x00,0x6C,0x00,0x6C,0x00,0x38,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//°37
  	
};


/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
//液晶字符输出(6*8字体)
//x: 0 - 20	(行)
//y: 0 -19	(列)
void LCD_P6X8(unsigned char x, unsigned char y, unsigned char c_dat,unsigned int word_color,unsigned int back_color)
{
	unsigned char i,j;

	LCD_SetPos(x*6,y*8,(x+1)*6-1,(y+1)*8-1);   //定位字符显示区域
	for(j=0; j<8; j++)
	{
		for(i=0; i<6; i++)
		{
			if((Font_code8[c_dat-32][i]) & (0x01<<j))
				LCD_write_para16(word_color);
			else
				LCD_write_para16(back_color);
		}
	}
}

/*--------------------------------------------------------------*/
//液晶字符输出(8*16字体)
//x: 0 -15   (行)
//y: 0 -9  	 (列)
void LCD_P8X16(unsigned char x, unsigned char y, unsigned char c_dat,unsigned int word_color,unsigned int back_color)
{
	unsigned char i,j;

	LCD_SetPos(x*8,y*16,(x+1)*8-1,(y+1)*16-1);			//定位字符显示区域
	for(j=0; j <16; j++)
	{
		for(i=0; i<8; i++)
		{
			if((Font_code16[c_dat-32][j]) & (0x01<<i))
				LCD_write_para16(word_color);
			else
				LCD_write_para16(back_color);
		}
	}
}

/*--------------------------------------------------------------*/
//液晶字符串输出(8*16字体)
//x: 0 - 15 (行)
//y: 0 - 7  (列)
void LCD_P8X16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned int word_color,unsigned int back_color)
{
	while(*s_dat)
        { 
          LCD_P8X16(x++, y, *s_dat++,word_color,back_color);
          time_delay_ms(201);//汉字显示速度，演示用，平时可以注释掉，不影响显示速度
        }
}

//液晶字符输出(16*12字体)
//x: 0 - 7	(行)
//y: 0 -12	(列)
void LCD_P16X12Str(unsigned char x,unsigned char y,unsigned char *s_dat,unsigned int word_color,unsigned int back_color)
{
	unsigned char wm=0,ii = 0,i,j;
	unsigned int adder=1; 
	
	while(s_dat[ii] != '\0')
	{
	  	wm = 0;
	  	adder = 1;
	  	while(hanzi_Idx[wm] > 127)
	  	{
	  		if(hanzi_Idx[wm] == s_dat[ii])
	  		{
	  			if(hanzi_Idx[wm + 1] == s_dat[ii + 1])
	  			{
	  				adder = wm * 12;
	  				break;
	  			}
	  		}
	  		wm += 2;			
	  	}		
	  	if(adder != 1)// 显示汉字					
	  	{
	  		LCD_SetPos(x*16,y*12,(x+1)*16-1,(y+1)*12-1);			//定位字符显示区域 			
	  		for(j=0; j <24; j++)
			{
				for(i=0; i<8; i++)
				{
					if((hanzi[adder]) &  (0x80>>i))
						LCD_write_para16(word_color);
					else
						LCD_write_para16(back_color);
				}
				adder += 1;		 	
			} 					
	  	}
	  	else			  //显示空白字符			
	  	{}
	    //y+=1;//左右方向
		x+=1;//上下方向  		  	
	  	ii += 2;
               time_delay_ms(201);//汉字显示速度，演示用，平时可以注释掉，不影响显示速度 
	}
}
//液晶字符输出(16*16字体)
//x: 0 - 7	(行)
//y: 0 -9	(列) 
void LCD_P16X16Str(unsigned char x,unsigned char y,unsigned char *s_dat,unsigned int word_color,unsigned int back_color)
{
	unsigned char wm=0,ii = 0,i,j;
	unsigned int adder=1; 
	
	while(s_dat[ii] != '\0')
	{
	  	wm = 0;
	  	adder = 1;
	  	while(hanzi_Idx[wm] > XMAX-1)
	  	{
	  		if(hanzi_Idx[wm] == s_dat[ii])
	  		{
	  			if(hanzi_Idx[wm + 1] == s_dat[ii + 1])
	  			{
	  				adder = wm * 16;
	  				break;
	  			}
	  		}
	  		wm += 2;			
	  	}		
	  	if(adder != 1)// 显示汉字					
	  	{
	  		LCD_SetPos(x*16,y*16,(x+1)*16-1,(y+1)*16-1);			//定位字符显示区域 			
	  		for(j=0; j <32; j++)
			{
				for(i=0; i<8; i++)
				{
					if((hanzi16x16[adder]) &  (0x80>>i))
						LCD_write_para16(word_color);
					else
						LCD_write_para16(back_color);
				}
				adder += 1;		 	
			} 					
	  	}
	  	else			  //显示空白字符			
	  	{}
	    //y+=1;//左右方向
		x+=1;//上下方向  		  	
	  	ii += 2;
                time_delay_ms(201);//汉字显示速度，演示用，平时可以注释掉，不影响显示速度
	}
}

/*--------------------------------------------------------------*/
//液晶字符串输出(6*8字体)
//x: 0 - 20 (行)
//y: 0 -19 (列)
void LCD_P6X8Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned int word_color,unsigned int back_color)
{
	while(*s_dat)
        {
          LCD_P6X8(x++, y, *s_dat++,word_color,back_color);
          time_delay_ms(201);//汉字显示速度，演示用，平时可以注释掉，不影响显示速度
        }
}

/*--------------------------------------------------------------*/ 
//定位输出数字
//x: 0 - 20  (行)
//y: 0 - 19 (列)
//num: 0 - 65535	要显示的数字
//num_bit: 0 - 5	数字的位数
void LCD_P6X8NUM(unsigned char x, unsigned char y, unsigned int num, unsigned char num_bit,unsigned int word_color,unsigned int back_color)
{
	signed char i;
	unsigned char ii;
	unsigned char dat[6];
	for(i = 0; i < 6; i++) dat[i] = 0; i = 0;	//初始化数据
	while(num / 10)								//拆位
	{
		dat[i] = num % 10;						//最低位
		num /= 10; i++;		
	}
	dat[i] = num;								//最高位
	ii = i;										//保存dat的位数
	for(; i>= 0; i--)	dat[i] += 48;			//转化成ASCII
	for(i = 0; i < num_bit; i++)
	LCD_P6X8(x, y + i, ' ',word_color,back_color);		//清显示区域
	for(i = ii; i>= 0; i--)
	LCD_P6X8(x++, y, dat[i],word_color,back_color);		//输出数值
}
/*--------------------------------------------------------------*/

/*--------------------------------------------------------------*/
//液晶字符输出(8*8字体)
//x: 0 - 15	(行)
//y: 0 -19	(列)
void LCD_P8X8(unsigned char x, unsigned char y, unsigned char c_dat,unsigned int word_color,unsigned int back_color)
{
	unsigned char i,j;

	LCD_SetPos(x*8,y*8,(x+1)*8-1,(y+1)*8-1);			//定位字符显示区域

	for(j=0; j<8; j++)
	{
		LCD_write_para16(back_color);
		for(i=0; i<6; i++)
		{
			if((Font_code8[c_dat-32][i]) & (0x01<<j))
				LCD_write_para16(word_color);
			else
				LCD_write_para16(back_color);
		}
		LCD_write_para16(back_color);
	}
}

/*--------------------------------------------------------------*/
//液晶字符串输出(8*8字体)
//x: 0 - 15 (行)
//y: 0 -19 (列)
void LCD_P8X8Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned int word_color,unsigned int back_color)
{
	while(*s_dat) 
        {
          LCD_P8X8(x++, y, *s_dat++,word_color,back_color);
        }
}

/*--------------------------------------------------------------*/ 
//定位输出数字
//x: 0 - 15  (行)
//y: 0 - 19 (列)
//num: 0 - 65535	要显示的数字
//num_bit: 0 - 5	数字的位数
void LCD_P8X8NUM(unsigned char x, unsigned char y, unsigned int num, unsigned char num_bit,unsigned int word_color,unsigned int back_color)
{
	signed  char i;
	unsigned char ii;
	unsigned char dat[6];
	for(i = 0; i < 6; i++) dat[i] = 0; i = 0;	//初始化数据
	while(num / 10)								//拆位
	{
		dat[i] = num % 10;						//最低位
		num /= 10; i++;		
	}
	dat[i] = num;								//最高位
	ii = i;										//保存dat的位数
	for(; i>= 0; i--)	dat[i] += 48;			//转化成ASCII
	for(i = 0; i < num_bit; i++)
	LCD_P8X8(x, y + i, ' ',word_color,back_color);					//清显示区域
	for(i = ii; i>= 0; i--)
	LCD_P8X8(x, y++, dat[i],word_color,back_color);					//输出数值
}

                 
