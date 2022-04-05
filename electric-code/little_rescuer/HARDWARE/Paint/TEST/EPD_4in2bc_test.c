/*****************************************************************************
* | File      	:   EPD_4in2bc_test.c
* | Author      :   Waveshare team
* | Function    :   4.2inch B&C e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-13
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
//#include "EPD_Test.h"
#include <stdlib.h>
#include "EPD_4in2bc.h"
#include "usart.h"
#include "delay.h"
#include "GUI_Paint.h"
#include "led.h"
#include "sensor.h"
#include "test.h"

UBYTE *BlackImage, *RYImage; // Red or Yellow

int EPD_4in2bc_test(void)
{
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_4IN2BC_Init();
	delay_ms(500);
    EPD_4IN2BC_Clear();
    
//	Paint_Clear(RED);

    //Create a new image cache named IMAGE_BW and fill it with white
    
    UWORD Imagesize = ((EPD_4IN2BC_WIDTH % 8 == 0)? (EPD_4IN2BC_WIDTH / 8 ): (EPD_4IN2BC_WIDTH / 8 + 1)) * EPD_4IN2BC_HEIGHT;
	printf("%d\r\n",Imagesize);
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) 
	{
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) 
	{
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }
    printf("NewImage:BlackImage and RYImage\r\n");
    Paint_NewImage(BlackImage, EPD_4IN2BC_WIDTH, EPD_4IN2BC_HEIGHT, 270, WHITE);
    Paint_NewImage(RYImage, EPD_4IN2BC_WIDTH, EPD_4IN2BC_HEIGHT, 270, WHITE);

    //Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);

#if 0   // show image for array    
    printf("show image for array\r\n");
    EPD_4IN2BC_Display(gImage_4in2bc_b, gImage_4in2bc_ry);
	Paint_Clear(WHITE);
		
#endif
	
    /*Horizontal screen*/
    //1.Draw black image

    EPD_display();
		
#if 0   // Drawing on the image
    printf("Clear...\r\n");
    EPD_4IN2BC_Clear();
    
    printf("Goto Sleep...\r\n");
    EPD_4IN2BC_Sleep();
    free(BlackImage);
    free(RYImage);
    BlackImage = NULL;
    RYImage = NULL;
#endif

 //   printf("close 5V, Module enters 0 power consumption ...\r\n");
//    DEV_Module_Exit();
    
    return 0;
}


void EPD_display(void)
{
	printf("Draw black image\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    Paint_DrawString_CN(10, 10, "光线强度：", &Font12CN, BLACK, WHITE);
	Paint_DrawString_CN(10, 50, "烟雾强度：", &Font12CN, BLACK, WHITE);
	Paint_DrawString_CN(10, 90, "红外强度：", &Font12CN, BLACK, WHITE);
	Paint_DrawString_CN(10, 130, "雨水强度：", &Font12CN, BLACK, WHITE);
	Paint_DrawString_CN(10, 170, "火焰强度：", &Font12CN, BLACK, WHITE);
	Paint_DrawString_CN(10, 210, "距离：", &Font12CN, BLACK, WHITE);
	Paint_DrawString_CN(10, 250, "俯仰角：", &Font12CN, BLACK, WHITE);
	Paint_DrawString_CN(10, 290, "横滚角：", &Font12CN, BLACK, WHITE);
	Paint_DrawString_CN(10, 330, "航向角：", &Font12CN, BLACK, WHITE);
		
	Paint_DrawNum(100, 14, light, &Font24, BLACK, WHITE);
	Paint_DrawNum(100, 54, smoke, &Font24, BLACK, WHITE);
	Paint_DrawNum(100, 94, infrare, &Font24, BLACK, WHITE);
	Paint_DrawNum(100, 134, rain, &Font24, BLACK, WHITE);
	Paint_DrawNum(100, 174, flame, &Font24, BLACK, WHITE);
	Paint_DrawNum(100, 214, Distance, &Font24, BLACK, WHITE);
	Paint_DrawNum(100, 254, 0, &Font24, BLACK, WHITE);
	Paint_DrawNum(100, 294, 0, &Font24, BLACK, WHITE);
	Paint_DrawNum(100, 334, 20, &Font24, BLACK, WHITE);

    printf("EPD_Display\r\n");
    EPD_4IN2BC_Display(BlackImage, RYImage);
}
