#include<reg52.h>   
#define DELAY_TIME 80 //SD卡的复位与初始化时SPI的延时参数，根据实际速率修改其值，否则会造成SD卡复位或初始化失败
#define TRY_TIME 800    //向SD卡写入命令之后，读取SD卡的回应次数，即读TRY_TIME次，如果在TRY_TIME次中读不到回应，产生超时错误，命令写入失败
sbit SD_SCL=P1^3; //SD卡同步时钟  输入
sbit SD_SI =P1^1; //SD卡同步数据  输入
sbit SD_CS =P1^2; //SD卡片选   输入
sbit SD_SO =P1^0; //SD卡同步数据  输出
 
unsigned long laddr;
unsigned char SD_Reset();
unsigned char SD_Init();
unsigned char SD_Read_Sector(unsigned long addr);
unsigned char xdata  pbuf[512]; //数据缓冲区
unsigned char  pbu[10];
unsigned char is_init; //在初始化的时候设置此变量为1,                    
unsigned char bdata _dat;
sbit _dat7=_dat^7;
sbit _dat6=_dat^6;
sbit _dat5=_dat^5;
sbit _dat4=_dat^4;
sbit _dat3=_dat^3;
sbit _dat2=_dat^2;
sbit _dat1=_dat^1;
sbit _dat0=_dat^0;
void delay(unsigned int time)
{
 while(time--);
}
 
 
 
void SD_spi_write(unsigned char x)
{
 _dat=x;
 SD_SI=_dat7;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat6;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat5;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat4;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat3;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat2;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat1;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SI=_dat0;
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME); 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
}
 
 
unsigned char SD_spi_read() //SPI读一个字节
{ 
 SD_SO=1;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat7=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat6=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat5=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat4=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat3=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat2=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat1=SD_SO;
 
 SD_SCL=1;
 if(is_init) delay(DELAY_TIME);
 SD_SCL=0;
 if(is_init) delay(DELAY_TIME);
 _dat0=SD_SO;
 
 return (_dat);
}
 
 
unsigned char SD_Write_Cmd(unsigned char *pcmd) //向SD卡写命令，pcmd是命令字节序列的首地址
{
 unsigned char temp,tim;
 tim=0;
 SD_CS=1;
 SD_spi_write(0xff); //提高兼容性，如果没有这里，有些SD卡可能不支持  
 SD_CS=0;
 SD_spi_write(pcmd[0]);
 SD_spi_write(pcmd[1]);
 SD_spi_write(pcmd[2]);
 SD_spi_write(pcmd[3]);
 SD_spi_write(pcmd[4]);
 SD_spi_write(pcmd[5]); 
 do
 { 
  temp = SD_spi_read();//一直读，直到读到的不是0xff或超时
  tim++;
  
 }while((temp==0xff)&&(tim<TRY_TIME));
 
 return(temp);
}
 
 
 
 
 
unsigned char SD_Init() //初始化，使用CMD1（命令1）
{ 
 unsigned char temp;
 unsigned char pcmd[] = {0x41,0x00,0x00,0x00,0x00,0xff}; //命令1的字节序列
 
 SD_CS=0; //打开片选 
 temp=SD_Write_Cmd(pcmd);                 
 is_init=0; //初始化完毕，将is_init设置为0,为了提高以后的数据传输速度   
 SD_CS=1;  //关闭片选
 SD_spi_write(0xff); //按照SD卡的操作时序在这里补8个时钟
 return(temp); //返回0,说明初始化操作成功
}
 
unsigned char SD_Read_Sector(unsigned long addr)//从SD卡的指定扇区中读出512个字节，使用CMD17（17号命令）
{unsigned char temp;
 unsigned char pcmd[]={0x51,0x00,0x00,0x00,0x00,0xff}; //CMD17的字节序列  
 addr<<=9; //addr=addr*512    将块地址（扇区地址）转为字节地址
 pcmd[1]=((addr&0xff000000)>>24);//将字节地址写入到CMD17字节序列中
 pcmd[2]=((addr&0x00FF0000)>>16);
 pcmd[3]=((addr&0x0000FF00)>>8);
 SD_CS=0;//打开片选
 //time=0;  
 temp=SD_Write_Cmd(pcmd); //写入CMD17
 return (temp);
}
unsigned char SD_Reset()//SD卡复位，进入SPI模式，使用CMD0（命令0）
{
 unsigned char temp,i;
 unsigned char pcmd[] = {0x40,0x00,0x00,0x00,0x00,0x95}; //命令0的字节序列
 is_init=1; //将is_init置为1
 SD_CS=1;  //关闭片选
 for(i=0;i<0x0f;i++) //复位时，首先要发送最少74个时钟信号，这是必须的！！！
 {
  SD_spi_write(0xff);//120个时钟
 }
 
 SD_CS=0; //打开片选      
 temp=SD_Write_Cmd(pcmd);//写入CMD0   
 SD_CS=1; //关闭片选
 //按照SD卡的操作时序在这里补8个时钟
 return (temp);//返回0,说明复位操作成功
 
}
void main()
{unsigned char t,s;
 unsigned int j;
 laddr=1892;
 //P2=0x00;
 t=0x00;
 while(t!=0x01)
 {t=SD_Reset();}
 SD_spi_write(0xff);
 //P2=0x0f;
 while(t!=0x00)
 {t=SD_Init();}    
 //SD_Write_Sector(ADDR,pbuf);//将数据缓冲区中的512个字节的数据写入SD卡的第ADDR扇区中
 
 t=0xff;
 while(t!=0x00)
 {t=SD_Read_Sector(laddr);}//从SD卡的第ADDR扇区中读取512个字节的数据到数据缓冲区
 //P2=0xca;
 while (t!=0xfe){t=SD_spi_read(); //一直读，当读到0xfe时，说明后面的是512字节的数据了                            
            
                    }
 
  //P2=0x0f;
 for(j=0;j<512;j++)       //将数据写入到数据缓冲区中
 {   
  pbuf[j]=SD_spi_read();
 }   
 for(j=0;j<10;j++)  //将数据写入到数据缓冲区中
 {   
  pbu[j]=pbuf[j];
 }                                                                                                        
 SD_spi_read();
 SD_spi_read();//读取两个字节的CRC校验码，不用关心它们
 SD_CS=1;  //SD卡关闭片选
 SD_spi_write(0xff);//按照SD卡的操作时序在这里补8个时钟
 for(j=0;j<10;j++)  //将数据写入到数据缓冲区中
 {   
   P2=pbu[j];
   for(s=0;s<200;s++){delay(1000);}
 
 }          
 
 //P2=0x0c;
 while(1);
}