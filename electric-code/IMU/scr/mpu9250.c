#include "main.h"

unsigned char BUF[14];       //���ݻ���

double Acc_temp[3][5];
u8 Acc_cnt=0;

u8 MAG_x_axis,MAG_y_axis,MAG_z_axis;

u8 ST1_temp;
u8 MAG_cnt=0;

const u32 IIC_DELAY = 5;
u16 MS561101BA_Cal_C[7];
u32 D1_Pres,D2_Temp;
s32 dT;
float TEMP,T2;
double OFF,SENS,OFF2,SENS2;
float Pressure;
float Altitude;
u8 MS5611_cnt=0;

void WriteTo9250(u8 TxData)
{
//	u8 i = 0;
  u8 ValueToWrite = 0;
	
	ValueToWrite = TxData;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){;}
    SPI_I2S_SendData(SPI1, ValueToWrite);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){;}
	  SPI_I2S_ReceiveData(SPI1);
 
}

u8 ReadToMpu9250(u8 reg)
{
//	 u8 i = 0;
   u8 ReadData = 0;
	
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI1, reg);
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);					  
  ReadData = SPI_I2S_ReceiveData(SPI1);
  return(ReadData);
}

/***************************************************************/
//SPI����
//reg: addr
/***************************************************************/
void MPU9250_Write_Reg(u8 reg,u8 value)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
  Delay(100);
	WriteTo9250(reg); //����reg��ַ
	WriteTo9250(value);//��������
  Delay(100);
	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	Delay(100);
}

//SPI��ȡ
//reg: addr
u8 MPU9250_Read_Reg(u8 reg)
{
	u8 reg_val;
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
  Delay(100);
	WriteTo9250(reg|0x80); //reg��ַ+��ʹ��
	reg_val=ReadToMpu9250(0xff);//����
//	
	Delay(100);
	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	Delay(100);
	return reg_val;
}


u8 MPU9250_Read_RegArray(u8 reg,int len, unsigned char* dest )
{
	int readIndex;
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
  Delay(100);
	WriteTo9250(reg|0x80); //reg��ַ+��ʹ��
	
	for(readIndex=0; readIndex<len;readIndex++)
	{
	    dest[readIndex]=ReadToMpu9250(0xff);//����
			Delay(100);
	}

  GPIO_SetBits(GPIOC,GPIO_Pin_4);
  Delay(100);
	return 0;
}

/***************************************************************/
// MPU�ڲ�i2cд��
//I2C_SLVx_ADDR:  MPU9250_AK8963_ADDR
//I2C_SLVx_REG:   reg
//I2C_SLVx_Data out:  value
/***************************************************************/
static void i2c_Mag_write(u8 reg,u8 value)
{
	u16 j=100;
	MPU9250_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR);//���ô����Ƶ�ַ,mode: write
	MPU9250_Write_Reg(I2C_SLV0_REG ,reg);//set reg addr
	MPU9250_Write_Reg(I2C_SLV0_DO ,value);//send value	
	while(j--);//�ȴ��ڲ�д���
}

/***************************************************************/
// MPU�ڲ�i2c ��
//I2C_SLVx_ADDR:  MPU9250_AK8963_ADDR
//I2C_SLVx_REG:   reg
//return value:   EXT_SENS_DATA_00 register value
/***************************************************************/
static u8 i2c_Mag_read(u8 reg)
{
	u16 j=1000;

	MPU9250_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR|0x80); //���ô����Ƶ�ַ,mode:read
	MPU9250_Write_Reg(I2C_SLV0_REG ,reg);// set reg addr
	while(j--);
	MPU9250_Write_Reg(I2C_SLV0_DO ,0xff);//read
	while(j--);//�ȴ��ڲ������
	return MPU9250_Read_Reg(EXT_SENS_DATA_00);
}

//****************��ʼ��MPU9250************************
#define AKM_REG_WHOAMI      (0x00)
void Init_MPU9250(void)
{
	BUF[0]=MPU9250_Read_Reg(WHO_AM_I);
	BUF[1]=i2c_Mag_read(AKM_REG_WHOAMI);
	MPU9250_Write_Reg(PWR_MGMT_1, 0x80);	//�������״̬
	Delay(500000);
	//MPU9250_Write_Reg(PWR_MGMT_1, 0x00);//ԭ��
	MPU9250_Write_Reg(PWR_MGMT_1, 0x01);
	BUF[0]=MPU9250_Read_Reg(PWR_MGMT_1);

	/**********************Init SLV0 i2c**********************************/	
  //Use SPI-bus read slave0
	MPU9250_Write_Reg(INT_PIN_CFG ,0x02);//  Bypass Enable Configuration  
	MPU9250_Write_Reg(56 ,0x01);
	MPU9250_Write_Reg(I2C_MST_CTRL,0x4d);//I2C MAster mode and Speed 400 kHz
	MPU9250_Write_Reg(USER_CTRL ,0x20); // I2C_MST _EN 
	MPU9250_Write_Reg(I2C_MST_DELAY_CTRL ,0x01);//��ʱʹ��I2C_SLV0 _DLY_ enable 
//  MPU9250_Write_Reg(I2C_MST_DELAY_CTRL ,0x03);	
	MPU9250_Write_Reg(I2C_SLV0_CTRL ,0x81); //enable IIC	and EXT_SENS_DATA==1 Byte
//	MPU9250_Write_Reg(I2C_SLV1_CTRL ,0x82); 
	
	
		/**********************Init MAG **********************************/
	 BUF[0]=i2c_Mag_read(AK8963_WHOAMI_REG);
   i2c_Mag_write(AK8963_CNTL2_REG,AK8963_CNTL2_SRST); // Reset AK8963
	 // BUF[0]=i2c_Mag_read(AK8963_CNTL2_REG);
	 Delay(500000);
	 i2c_Mag_write(AK8963_CNTL1_REG,0x16); // use i2c to set AK8963 working on Continuous measurement mode1 & 16-bit output	
	
	 MAG_x_axis=i2c_Mag_read(AK8963_ASAX);
	 MAG_y_axis=i2c_Mag_read(AK8963_ASAY);
	 MAG_z_axis=i2c_Mag_read(AK8963_ASAZ);
	 
	 
	 ST1_temp=i2c_Mag_read(AK8963_ST1_REG);
		
		
/////////////////////////////////////////////////////////////////////////////	
	/*******************Init GYRO and ACCEL******************************/	
   //MPU9250_Write_Reg(SMPLRT_DIV, 0x07);//ԭ  //�����ǲ�����,����ֵ:0x07(1kHz) (SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV) )
	MPU9250_Write_Reg(SMPLRT_DIV, 0x0);  //�����ǲ�����,����ֵ:0x07(1kHz) (SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV) )
//	MPU9250_Write_Reg(CONFIG, 0x47); //ԭ     //��ͨ�˲�ʱ�����ֵ07��3600Hz��Internal_Sample_Rate==8K
	MPU9250_Write_Reg(CONFIG, 0x43);      //��ͨ�˲�ʱ�����ֵ07��3600Hz��Internal_Sample_Rate==8K
	BUF[1]=MPU9250_Read_Reg(CONFIG);
	MPU9250_Write_Reg(GYRO_CONFIG, 0x08); //�������Լ켰������Χ,����ֵ:0x18(���Լ�,2000deg/s)               0x08  500deg/s
	MPU9250_Write_Reg(ACCEL_CONFIG, 0x08);//���ٶȼ��Լ켰������Χ����ͨ�˲�Ƶ�ʣ�����ֵ:0x18(���Լ�,16G)    0x08  4G
	MPU9250_Write_Reg(ACCEL_CONFIG_2, 0x48);//���ٶȼ��˲�Ƶ�� 1K OUT   200ƽ��
	MPU9250_Write_Reg(FIFO_EN, 0xf8); 
  //MPU9250_Write_Reg(105, 0x40); 
	 
//	 /**********************Init MS5611 **********************************/
//	 i2c_MS5611_write(MS561101BA_SlaveAddress,MS561101BA_RST);
//	 Delay(100000);
//	 MS561101BA_Cal_C[0]=i2c_MS5611_read(MS561101BA_PROM_RD);
}

double max_min_chioce(double value_tmp[],u8 number)
{
	 double max_value, min_value; 
   double sample_value = 0;
   double sum = 0;
   u8 i;
	
   max_value = 	value_tmp[0];
   min_value = 	value_tmp[0];
   for( i =0; i< number; i++)
    {
	 if(value_tmp[i] >  max_value)
	   {
		 max_value =  value_tmp[i];
	   }
	 if(value_tmp[i] <  min_value)
	   {
		 min_value =  value_tmp[i];
	   }
	}
   for(i=0;i< number;i++)
    {
	 sum += value_tmp[i];
	}
   sample_value = (sum - max_value - min_value)/(number - 2);
   return (sample_value);
	
	
}


//************************��ȡ���ٶȼ�**************************/
void READ_MPU9250_A_T_G(void)
{ 

  while(! GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)){;}		//�ж�����INT��Ϊ1ʱ���ݸ������

   BUF[0]=MPU9250_Read_Reg(ACCEL_XOUT_L); 
   BUF[1]=MPU9250_Read_Reg(ACCEL_XOUT_H);
   mpu_AD_value.Accel[0]=	(BUF[1]<<8)|BUF[0];
   mpu_Data_value.Accel[0] =(double)mpu_AD_value.Accel[0]/(double)8192; 
//	 Acc_temp[0][Acc_cnt]=(double)mpu_AD_value.Accel[0]/(double)8192;
//	 mpu_Data_value.Accel[0]=max_min_chioce(&Acc_temp[0][0],5);

   BUF[2]=MPU9250_Read_Reg(ACCEL_YOUT_L);
   BUF[3]=MPU9250_Read_Reg(ACCEL_YOUT_H);
   mpu_AD_value.Accel[1]=	(BUF[3]<<8)|BUF[2];
   mpu_Data_value.Accel[1] =(double)mpu_AD_value.Accel[1]/(double)8192;
//   Acc_temp[1][Acc_cnt]=(double)mpu_AD_value.Accel[1]/(double)8192;
//	 mpu_Data_value.Accel[1]=max_min_chioce(&Acc_temp[1][0],5);	

   BUF[4]=MPU9250_Read_Reg(ACCEL_ZOUT_L); 
   BUF[5]=MPU9250_Read_Reg(ACCEL_ZOUT_H);
   mpu_AD_value.Accel[2]=  (BUF[5]<<8)|BUF[4];
   mpu_Data_value.Accel[2] =(double)mpu_AD_value.Accel[2]/(double)8192;
//	 Acc_temp[2][Acc_cnt]=(double)mpu_AD_value.Accel[2]/(double)8192;
//	 mpu_Data_value.Accel[2]=max_min_chioce(&Acc_temp[2][0],5);
	 
	 Acc_cnt++;
	 if(Acc_cnt==5)
	 {
		 Acc_cnt=0;
	 }

	 BUF[0]=MPU9250_Read_Reg(TEMP_OUT_L); 
   BUF[1]=MPU9250_Read_Reg(TEMP_OUT_H);
   mpu_AD_value.Temp=	(BUF[1]<<8)|BUF[0];
	 mpu_Data_value.Temp =((double)mpu_AD_value.Temp/(double)333.87)+(double)21;
	
	 BUF[0]=MPU9250_Read_Reg(GYRO_XOUT_L); 
   BUF[1]=MPU9250_Read_Reg(GYRO_XOUT_H);
   mpu_AD_value.Gyro[0]=	(BUF[1]<<8)|BUF[0];
   mpu_Data_value.Gyro[0] =((double)mpu_AD_value.Gyro[0]/(double)65.5);//+3.5; 						   
   BUF[2]=MPU9250_Read_Reg(GYRO_YOUT_L);
   BUF[3]=MPU9250_Read_Reg(GYRO_YOUT_H);
   mpu_AD_value.Gyro[1]=	(BUF[3]<<8)|BUF[2];
   mpu_Data_value.Gyro[1] =((double)mpu_AD_value.Gyro[1]/(double)65.5);//-0.9;  						   
   BUF[4]=MPU9250_Read_Reg(GYRO_ZOUT_L); 
   BUF[5]=MPU9250_Read_Reg(GYRO_ZOUT_H);
   mpu_AD_value.Gyro[2]=  (BUF[5]<<8)|BUF[4];
   mpu_Data_value.Gyro[2] =(double)mpu_AD_value.Gyro[2]/(double)65.5;  
}

////************************��ȡ���ٶȼ�**************************/
//void READ_MPU9250_A_T_G(void)
//{ 

//  while(! GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)){;}

//  MPU9250_Read_RegArray(ACCEL_XOUT_L,6,BUF);

//   mpu_AD_value.Accel[0]=	(BUF[0]<<8)|BUF[1];
//   mpu_Data_value.Accel[0] =(double)mpu_AD_value.Accel[0]/(double)16384; 

//   mpu_AD_value.Accel[1]=	(BUF[2]<<8)|BUF[3];
//   mpu_Data_value.Accel[1] =(double)mpu_AD_value.Accel[1]/(double)16384;


//   mpu_AD_value.Accel[2]=  (BUF[4]<<8)|BUF[5];
//   mpu_Data_value.Accel[2] =(double)mpu_AD_value.Accel[2]/(double)16384;



//   mpu_AD_value.Temp=	(BUF[6]<<8)|BUF[7];
//	 mpu_Data_value.Temp =((double)mpu_AD_value.Temp/(double)333.87)+(double)21;
//	
//   mpu_AD_value.Gyro[0]=	(BUF[8]<<8)|BUF[9];
//   mpu_Data_value.Gyro[0] =(double)mpu_AD_value.Gyro[0]/(double)65.5; 						   

//   mpu_AD_value.Gyro[1]=	(BUF[10]<<8)|BUF[11];
//   mpu_Data_value.Gyro[1] =(double)mpu_AD_value.Gyro[1]/(double)65.5;  						   

//   mpu_AD_value.Gyro[2]=  (BUF[12]<<8)|BUF[13];
//   mpu_Data_value.Gyro[2] =(double)mpu_AD_value.Gyro[2]/(double)65.5;  
//}



//************************read MAG**************************/
void READ_MPU9250_MAG(void)
{ 
	ST1_temp=i2c_Mag_read(AK8963_ST1_REG);
	if((ST1_temp & AK8963_ST1_DRDY)==AK8963_ST1_DRDY)
	{
		if(MAG_cnt==0)
		{
	  BUF[0]=i2c_Mag_read(MAG_XOUT_L);
	  BUF[1]=i2c_Mag_read(MAG_XOUT_H);
	  mpu_AD_value.Mag[0]=(BUF[1]<<8)|BUF[0];
		mpu_Data_value.Mag[0]=mpu_AD_value.Mag[0]*(double)0.25*((double)1+((double)MAG_x_axis-(double)128)/(double)256);
		MAG_cnt=1;			
		}
		else if(MAG_cnt==1)
		{
		BUF[2]=i2c_Mag_read(MAG_YOUT_L);
	  BUF[3]=i2c_Mag_read(MAG_YOUT_H);
	  mpu_AD_value.Mag[1]=(BUF[3]<<8)|BUF[2];
		mpu_Data_value.Mag[1]=mpu_AD_value.Mag[1]*(double)0.25*((double)1+((double)MAG_y_axis-(double)128)/(double)256);
		MAG_cnt=2;	
		}
		else if(MAG_cnt==2)
		{
		BUF[4]=i2c_Mag_read(MAG_ZOUT_L);
	  BUF[5]=i2c_Mag_read(MAG_ZOUT_H);
	  mpu_AD_value.Mag[2]=(BUF[5]<<8)|BUF[4];
		mpu_Data_value.Mag[2]=mpu_AD_value.Mag[2]*(double)0.25*((double)1+((double)MAG_z_axis-(double)128)/(double)256);
		MAG_cnt=0;
		}
	  i2c_Mag_write(AK8963_CNTL1_REG,0x11);
	}
}
////////////////////////////////////////////////////////////////////////////////

/***************************************************************
// SDA_Out2Input SDA���ŵĴ�����任�����룬��MCU�����йأ�������
// ST��STM32 MCUΪ��.
****************************************************************/
void IIC_SDA_Out2Input( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/***************************************************************
// SDA_Input2Out SDA���ŵĴ�����任���������MCU�����йأ�������
// ST��STM32 MCUΪ��.
****************************************************************/
void IIC_SDA_Input2Out( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void IIC_Configuration( void )
{   
	GPIO_InitTypeDef      GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/***************************************************************
// i2c_Delay ��ʱ��������ʱʱ����MCU�ͺ��й�.
****************************************************************/
void IIC_Delay( u32 u32Delay )
{
	vu32 i;
	
	for( i = 0; i < u32Delay; i ++ )
	{
		;
	}	 
}

/***************************************************************
// ��SCLΪ�ߵ�ƽʱ��SDA�ɸߵ�ƽ��͵�ƽ���䣬������ʼ�ź�.
// i2c_Start ģ��i2c����һ��start condition.
****************************************************************/
void IIC_Start(void) 
{

	IIC_SDA_HIGH;        // SDA --|___
	IIC_SCL_HIGH;        // SCL ----__
	IIC_Delay( IIC_DELAY );

	IIC_SDA_LOW;

	IIC_Delay( IIC_DELAY );
}

/****************************************************************
// ��SCLΪ�ߵ�ƽʱ��SDA�����ɵ͵�ƽ��ߵ�ƽ�����䣬����ֹͣ�ź�.
// i2c_Stop ģ��i2c����һ��stop condition.
****************************************************************/
void IIC_Stop(void)
{
	IIC_SDA_LOW;
	IIC_SCL_HIGH;
	IIC_Delay( IIC_DELAY );
	
	IIC_SDA_HIGH;
}
/****************************************************************
// ��һ��CLK�����ڣ� ͨ����SDA��ƽ������Ӧ���ź�.
// i2c_Ack ģ��i2c����һ��Ӧ��.
****************************************************************/
void IIC_Ack(void)
{
	IIC_SDA_LOW;

	IIC_SCL_HIGH;
	IIC_Delay( IIC_DELAY );
	
	IIC_SCL_LOW;
}

/****************************************************************
// ��һ��CLK�����ڣ� ����SDAΪ�ߵ�ƽ��������Ӧ���ź�.
// i2c_NoAck ģ��i2c����һ����Ӧ���ź�.
****************************************************************/
void IIC_NoAck(void)
{
	IIC_SDA_HIGH;
   
	IIC_SCL_HIGH;
	IIC_Delay( IIC_DELAY );
	
	IIC_SCL_LOW;
}

/****************************************************************
// i2c_SalveAck ģ��i2c�ж϶Է��豸�Ƿ񷢳�һ��Ӧ��. �͵�ƽΪӦ��.
****************************************************************/
u8 IIC_isSalveAck( void )
{
	vu8 lu8Tmp = 0, i;       // ��������volatileʱ���Ż�ʱ��������⡣
		
	IIC_SDA_HIGH;
  IIC_SDA_Out2Input(); // SDAת��Ϊ����ģʽ.
	
	IIC_SCL_HIGH;
   
   for( i = 0; i < 10; i ++ )
   {
	   if( IIC_SDA_VALUE == 0 )	// �Է���ӦACK����
	   {
		   lu8Tmp = 1;
         break;
	   }
	   IIC_Delay( IIC_DELAY );
	}
	IIC_SCL_LOW;
	IIC_SDA_Input2Out();
	
	return lu8Tmp;
}

/****************************************************************
// i2c_Send ģ��i2c����һ���ֽ�����.
****************************************************************/
void IIC_Send(u8 u8Snd)
{
	u8 i;
	
	IIC_SCL_LOW;
	// �Ӹ�λ������λ, MSB��ǰ.
	for( i = 0; i < 8; i ++ )
	{
		if( (u8Snd & 0x80) == 0x80 )		// �������λ.
		{
			IIC_SDA_HIGH;
		}
		else
		{
			IIC_SDA_LOW;
		}
		IIC_Delay( IIC_DELAY );	// ��֤�����ߵ��ȶ�.
		
		IIC_SCL_HIGH;				// �����ȶ���SCLΪ�ߵ�ƽ
		u8Snd = u8Snd << 1;		// ׼������һλ.
		IIC_Delay( IIC_DELAY ); // ��ʱ��ȷ���Է�������SDA��ƽ.
		IIC_SCL_LOW;				// SCL�õͣ���֤��һλ���ݱ仯ʱSCLΪ�͵�ƽ.
	}
}

/****************************************************************
// i2c_Read ģ��i2c��һ���ֽ�����.
****************************************************************/
u8 IIC_Read(void)
{
	u8 i;
	vu8 lu8Val = 0;
	
	IIC_SDA_Out2Input(); // SDAת��Ϊ����ģʽ.
   for( i = 0; i < 8; i ++ )
   {
      IIC_SCL_HIGH;
      IIC_Delay( IIC_DELAY );
      
      lu8Val = lu8Val << 1;
      if( IIC_SDA_VALUE != 0 )
      {
         lu8Val = lu8Val | 0x01;
      }
      else
      {
        lu8Val = lu8Val & 0xfe;
      }
      
      IIC_SCL_LOW;
      IIC_Delay( IIC_DELAY );
   }
	IIC_SDA_Input2Out();		// SDAת��Ϊ���ģʽ.

	return (u8)lu8Val;
}

u8  IIC_WriteByte( u8 DeviceAddr, u8 RegisterAddr, u8 vol )
{
    IIC_Start() ;
    IIC_Send( DeviceAddr ) ;
	  if( 0 == IIC_isSalveAck() )
		{
			return 0;                       // �豸����Ӧ.
		}
    IIC_Send( RegisterAddr ) ;
		if( 0 == IIC_isSalveAck() )
		{
			return 0;                       // �豸����Ӧ.
		}
    IIC_Send( vol ) ;
		if( 0 == IIC_isSalveAck() )
		{
			return 0;                       // �豸����Ӧ.
		}
   IIC_Stop() ;
	
	  return 1;
}

u8  IIC_ReadnByte( u8 DeviceAddr, u8 RegisterAddr, u8 n, u8* para )
{
    u8  i ;

    IIC_Start() ;
    IIC_Send( DeviceAddr ) ;   //devaddr_Wr
		if( 0 == IIC_isSalveAck() )
		{
			return 0;                       // �豸����Ӧ.
		}	  
    IIC_Send( RegisterAddr ) ;
		if( 0 == IIC_isSalveAck() )
		{
			return 0;                       // �豸����Ӧ.
		}			
		IIC_Stop() ;
		
		
    IIC_Start();
    IIC_Send( DeviceAddr+1 ) ;   //devaddr_RD
    if( 0 == IIC_isSalveAck() )
		{
			return 0;                       // �豸����Ӧ.
		}	
    para[0] = IIC_Read() ;
    for( i=1; i<n; i++ )
    {
        IIC_Ack() ;
        para[i] = IIC_Read() ;
    }
    IIC_NoAck() ;
    IIC_Stop();
		return 1;  
}

u8 MS561101BA_RESET(void)
{
	IIC_Start();
	IIC_Send(MS561101BA_SlaveAddress);
	if( 0 == IIC_isSalveAck() )
	{
		return 0;                       // �豸����Ӧ.
	}
	IIC_Send(MS561101BA_RST);
	if( 0 == IIC_isSalveAck() )
	{
		return 0;                       // �豸����Ӧ.
	}
	IIC_Stop();
	return 1; 
}

u8 MS561101BA_PROM_READ(void)
{
	u8 d1,d2,i;
	
	for(i=0;i<7;i++)
	{
		IIC_Start();
		IIC_Send(MS561101BA_SlaveAddress);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		IIC_Send(MS561101BA_PROM_RD+i*2);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		IIC_Stop();		
		
		IIC_Start();
		IIC_Send(MS561101BA_SlaveAddress+1);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		d1 = IIC_Read();           
	  IIC_Ack(); 
		d2 = IIC_Read();
		IIC_NoAck();
		IIC_Stop();
		
		MS561101BA_Cal_C[i]=(d1<<8)+d2;
		Delay(1000);
	}
	
  return 1; 	
}

u8 MS561101BA_start_Temperature(void)
{
		IIC_Start();
		IIC_Send(MS561101BA_SlaveAddress);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		IIC_Send(MS561101BA_D2_OSR_4096);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		IIC_Stop();	
		
		return 1; 
}

u8 MS561101BA_getTemperature(void)
{
	  u8 d1,d2,d3;
		
		IIC_Start();
		IIC_Send(MS561101BA_SlaveAddress);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		IIC_Send(MS561101BA_ADC_RD);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		IIC_Stop();	

	  IIC_Start();
		IIC_Send(MS561101BA_SlaveAddress+1);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		d1 = IIC_Read();           
	  IIC_Ack(); 
		d2 = IIC_Read();
		IIC_Ack(); 
		d3 = IIC_Read();
		IIC_NoAck();
		IIC_Stop();
		
		D2_Temp=(d1<<16)+(d2<<8)+d3;
	  dT=D2_Temp - (((u32)MS561101BA_Cal_C[5])<<8);
		TEMP=(s32)((float)2000+(float)dT*((float)MS561101BA_Cal_C[6])/(float)8388608);
		
		return 1; 
}

u8 MS561101BA_start_Pressure(void)
{
	  IIC_Start();
		IIC_Send(MS561101BA_SlaveAddress);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		IIC_Send(MS561101BA_D1_OSR_4096);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		IIC_Stop();
		
		return 1; 
}

u8 MS561101BA_getPressure(void)
{
	  u8 d1,d2,d3;	

    IIC_Start();
		IIC_Send(MS561101BA_SlaveAddress);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		IIC_Send(MS561101BA_ADC_RD);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		IIC_Stop();			

	  IIC_Start();
		IIC_Send(MS561101BA_SlaveAddress+1);
		if( 0 == IIC_isSalveAck() )
	  {
		return 0;                       // �豸����Ӧ.
	  }
		d1 = IIC_Read();           
	  IIC_Ack(); 
		d2 = IIC_Read();
		IIC_Ack(); 
		d3 = IIC_Read();
		IIC_NoAck();
		IIC_Stop();
		
	  D1_Pres=(d1<<16)+(d2<<8)+d3;
		OFF=(double)MS561101BA_Cal_C[2]*(double)65536+(((double)MS561101BA_Cal_C[4]*dT)/(double)128);
    SENS=(double)MS561101BA_Cal_C[1]*(double)32768+(((double)MS561101BA_Cal_C[3]*dT)/(double)256);
		
		if(TEMP>=2000)
		{
			T2=0;
			OFF2=0;
			SENS2=0;
		}
		else 
		{
			T2=(dT*dT)/(float)0x80000000;
			OFF2=(float)2.5*(TEMP-(float)2000)*(TEMP-(float)2000);
			SENS2=(float)1.25*(TEMP-(float)2000)*(TEMP-(float)2000);
			if(TEMP<-1500)
			{
				OFF2=OFF2+(float)7.0*(TEMP+(float)1500)*(TEMP+(float)1500);
				SENS2=SENS2+(float)5.5*(TEMP+(float)1500)*(TEMP+(float)1500);
			}
		}
		
		TEMP=TEMP-T2;
		OFF=OFF-OFF2;
		SENS=SENS-SENS2;	
		
		Pressure=(D1_Pres*SENS/(float)2097152.0-OFF)/(float)3276800;
	  Altitude=((float)1013.25-Pressure)*(float)9.0;
		
    mpu_Data_value.Pressure=Pressure;
		mpu_Data_value.Altitude=Altitude;
		
		return 1; 
}


