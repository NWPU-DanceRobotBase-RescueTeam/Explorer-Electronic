#include "IIC.h"
#include "delay.h"
#include "usart.h"

void delay_1us(u8 x)//������ʱ,iic_40K
{
	u8 i=20;
	x=i*x;
	while(x--);
}
////////IIC��ʼ����//////////
/*
IIC��ʼ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�ƽ��ɵ͵�ƽ����һ���½��أ�Ȼ��SCL����
*/
u8 I2C_Start(void)
{
	SDA_OUT()
	IIC_SDA=1; 
	delay_us(5);	//��ʱ��֤ʱ��Ƶ�ʵ���40K���Ա�ӻ�ʶ��
	IIC_SCL=1;
	delay_us(5);//��ʱ��֤ʱ��Ƶ�ʵ���40K���Ա�ӻ�ʶ��
	SDA_IN()
	if(!READ_SDA) 
		return 0;//SDA��Ϊ�͵�ƽ������æ,�˳�
	SDA_OUT()
	IIC_SDA=0;   //SCL���ڸߵ�ƽ��ʱ��SDA����
	delay_us(5);
	SDA_IN()
	if(READ_SDA) 
		return 0;//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
	IIC_SCL=0;
	delay_us(5);
	return 1;
}
//**************************************
//IICֹͣ�ź�
/*
IICֹͣ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ����һ��������
*/
//**************************************
void I2C_Stop(void)
{
	SDA_OUT()
    IIC_SDA=0;
	IIC_SCL=0;
	delay_us(5);
	IIC_SCL=1;
	delay_us(5);
	IIC_SDA=1;//��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ             //��ʱ
}
//**************************************
//IIC����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
/*
Ӧ�𣺵��ӻ����յ����ݺ�����������һ���͵�ƽ�ź�
��׼����SDA��ƽ״̬����SCL�ߵ�ƽʱ����������SDA
*/
//**************************************
void I2C_SendACK(u8 i)
{
	SDA_OUT()
    if(1==i)
		IIC_SDA=1;	             //׼����SDA��ƽ״̬����Ӧ��
    else 
		IIC_SDA=0;  						//׼����SDA��ƽ״̬��Ӧ�� 	
	IIC_SCL=1;                    //����ʱ����
    delay_us(5);                 //��ʱ
    IIC_SCL=0 ;                  //����ʱ����
    delay_us(5);    
} 
///////�ȴ��ӻ�Ӧ��////////
/*
������(����)������һ�����ݺ󣬵ȴ��ӻ�Ӧ��
���ͷ�SDA���ôӻ�ʹ�ã�Ȼ��ɼ�SDA״̬
*/
/////////////////
u8 I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	uint16_t i=0;
	SDA_OUT()
	IIC_SDA=1;	        //�ͷ�SDA
	IIC_SCL=1;         //SCL���߽��в���
	SDA_IN()
	while(READ_SDA)//�ȴ�SDA����
	{
		i++;      //�ȴ�����
		if(i == 500)//��ʱ����ѭ��
		break;
	}
	if(READ_SDA)//�ٴ��ж�SDA�Ƿ�����
	{
		IIC_SCL=0; 
		return RESET;//�ӻ�Ӧ��ʧ�ܣ�����0
	}
    delay_us(5);//��ʱ��֤ʱ��Ƶ�ʵ���40K��
	IIC_SCL=0;
	delay_us(5); //��ʱ��֤ʱ��Ƶ�ʵ���40K��
	return SET;//�ӻ�Ӧ��ɹ�������1
}
//**************************************
//��IIC���߷���һ���ֽ�����
/*
һ���ֽ�8bit,��SCL�͵�ƽʱ��׼����SDA��SCL�ߵ�ƽʱ���ӻ�����SDA
*/
//**************************************
void I2C_SendByte(u8 dat)
{
    u8 i;
	SDA_OUT() 	    
    IIC_SCL=0;;//SCL���ͣ���SDA׼��
    for (i=0; i<8; i++)         //8λ������
    {
		if(dat&0x80)//SDA׼��
			IIC_SDA=1;  
		else 
			IIC_SDA=0;
        IIC_SCL=1;                //����ʱ�ӣ����ӻ�����
		delay_us(5);        //��ʱ����IICʱ��Ƶ�ʣ�Ҳ�Ǹ��ӻ������г���ʱ��
		IIC_SCL=0;                //����ʱ�ӣ���SDA׼��
		delay_us(5); 		  //��ʱ����IICʱ��Ƶ��
		dat <<= 1;          //�Ƴ����ݵ����λ  
    }					 
}
//**************************************
//��IIC���߽���һ���ֽ�����
//**************************************
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
	SDA_OUT() 	
    IIC_SDA=1;//�ͷ�SDA�����ӻ�ʹ��
    delay_us(1);         //��ʱ���ӻ�׼��SDAʱ��            
    for (i=0; i<8; i++)         //8λ������
    { 
		dat <<= 1;
			
		IIC_SCL=1;                //����ʱ���ߣ������ӻ�SDA
     
		if(READ_SDA) //������    
			dat |= 0x01;
		
       delay_us(5);     //��ʱ����IICʱ��Ƶ��		
       IIC_SCL=0;           //����ʱ���ߣ�������յ�������
       delay_us(5);   //��ʱ���ӻ�׼��SDAʱ��
    } 
    return dat;
}
//**************************************
//��IIC�豸д��һ���ֽ�����
//**************************************
u8 Single_WriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 data)
{
	  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(data);       //�ڲ��Ĵ������ݣ�
	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		
		I2C_Stop();   //����ֹͣ�ź�
		
		return SET;
}

u8 Single_MWriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length)
{
	  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
	while(length)
	{
		I2C_SendByte(*data++);       //�ڲ��Ĵ������ݣ�
	   if(!I2C_WaitAck()){I2C_Stop(); return RESET;}           //Ӧ��
		length--;
	}
	//	I2C_SendByte(*data);       //�ڲ��Ĵ������ݣ�
 	//	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		I2C_Stop();   //����ֹͣ�ź�		
		return SET;
}

//**************************************
//��IIC�豸��ȡһ���ֽ�����
//**************************************
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
{
 if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}          
	 
	I2C_SendByte(Slave_Address);    //�����豸��ַ+д�ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;} 
	
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;} 
	
	if(I2C_Start()==0)  //��ʼ�ź�
			{I2C_Stop(); return RESET;}            

	I2C_SendByte(Slave_Address+1);  //�����豸��ַ+���ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
	
	while(length-1)
	{
		*REG_data++=I2C_RecvByte();       //�����Ĵ�������
		I2C_SendACK(0);               //Ӧ��
		length--;
	}
	*REG_data=I2C_RecvByte();  
	I2C_SendACK(1);     //����ֹͣ�����ź�
	I2C_Stop();                    //ֹͣ�ź�
	return SET;
}
