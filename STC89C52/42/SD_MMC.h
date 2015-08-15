/*****************************************************************************

                               SD卡驱动程序
                            (裁剪版，只读不写）

*****************************************************************************/

#include <reg51.h>

/************************ 定义引脚 *************************/
sbit MISO = P3^0;  //SD卡数据输出
sbit CLK  = P3^1;  //SD卡时钟输入
sbit MOSI = P3^2;  //SD卡数据输入
sbit CS   = P3^3;  //SD卡片选使能

/*********************** 全局变量 **************************/

unsigned char Buffer[76] _at_ 0x14; //通用数据缓冲区,
                                    //定位0x14是为了配合汇编程序
union
{
    unsigned long all;
    unsigned char byte[4];
}Addr;                              //SD卡数据物理地址

/************************ 外部函数 *************************/
extern void Delay(unsigned int i);

/********************** 写入一个字节 ***********************/
void SD_write_byte(unsigned char dat)
{   
                        //不采用循环结构是为了提高处理速度
    ACC = dat << 1;     //直接访问ACC和CY加快处理速度
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;                                  
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
    ACC += ACC; CLK = 1;
    MOSI =  CY; CLK = 0;
/*ACC += ACC*/; CLK = 1;
}

/********************* 慢速写入一个字节 *********************/
SD_write_byte_low_speed(unsigned char x)
{
    unsigned char i;
    for(i = 8; i != 0; i--)
    {
        MOSI = x & 0x80;
        x <<= 1;
        CLK = 0;
        Delay(1);
        CLK = 1;
        Delay(1);
    }
}

/******************** 利用硬件移入一字节 ********************/
unsigned char SD_spi_read(void)
{   
    RI = 0; //利用51串口的同步移位功能，以达到最高的速度 2MHz CLK
    while(RI == 0);  //等待读取完成
    return SBUF;     //返回值 LSB MSB 倒序
}

/********************** 慢速读入一字节 ***********************/
unsigned char SD_read_byte_low_speed(void)
{
    unsigned char i, temp;
    for(i = 8; i != 0; i--)
    {
        CLK = 0;
        temp <<= 1;
        if(MISO) temp++;
        Delay(1);
        CLK = 1;
        Delay(1);
    }
    return temp;
}

/************************ 读入一字节 *************************/
unsigned char SD_read_byte(void)
{
    unsigned char i;
    for(i = 8; i != 0; i--)
    {   
        ACC <<= 1;          //直接访问ACC加快执行速度
        CLK = 0;
        if (MISO) ACC++;
        CLK = 1;
    }
    return ACC;
}

/********************* SD卡访问出错处理 **********************/
void Error(unsigned char Err_code)
{
    unsigned char i = 0;
    CS = 1;
    TR0 = 0;                //停止播放
    do                      //空读512字节
    {
        SD_spi_read();
        SD_spi_read();
    }
    while(--i);
    LCD3310_clear_screen(); //清屏
    LCD3310_set_XY(37,2);
    LCD3310_print(0x0e);    //显示'E'
    LCD3310_print(Err_code);//显示错误代码
    PCON |= 0x02;           //MCU休眠
    while(1);
}
                             
/************************ 等待SD响应 *************************/
void Wait_start_token(void)
{
    unsigned int i;
    RI = 0;
    i  = 20000;
    while(--i)
    {             
        if (SBUF == 0x7f) return;//等待起始令牌0xfe
        RI = 0;
    }
    Error(0);                   //超时报错
}

/*********************** 跳过不需要的数据 ********************/
void Skip_nwords(unsigned char n)
{
    do
    {
        SD_spi_read();
        SD_spi_read();
    }           
    while(--n);
}

/******************* SD读入n个字节数据 ***********************/
void Read_nbytes(unsigned char n)
{
    unsigned char i = 0;
    do
    {
        Buffer[i] = SD_read_byte();
    }
    while(++i != n);
}

/********************** SD卡 复位，激活 **********************/
void SD_init(void)
{
    //unsigned char i, timeout, retry, temp, cmd, crc;
    #define cmd     Buffer[0]
    #define i       Buffer[1]
    #define timeout Buffer[2]
    #define retry   Buffer[3]
    #define temp    Buffer[4]
    #define crc     Buffer[5]

    CS = 1;
    for(i = 15; i != 0; i--) //复位前,先发送74个以上时钟脉冲
    {
        SD_write_byte_low_speed(0xff);
    }

    cmd = 0x40;              //CMD0
    crc = 0x95;              
    i = 2;                   
    do                       //分别发送 CMD0 和 CMD1
    {
        retry = 100;
        do
        {
            CS = 1;
            SD_write_byte_low_speed(0xff);
            CS = 0;
            SD_write_byte_low_speed(cmd);
            SD_write_byte_low_speed(0);
            SD_write_byte_low_speed(0);
            SD_write_byte_low_speed(0);
            SD_write_byte_low_speed(0);
            SD_write_byte_low_speed(crc);

            timeout = 100;
            do
            {
                temp = SD_read_byte_low_speed();
            }                       
            while(temp == 0xff && --timeout);//等待SD卡响应

            if(--retry == 0) Error(i);       //重试失败报错
        }
        while(++temp != i);                  //SD响应字节应为0x01,0x00
        SD_write_byte_low_speed(0xff);       //补充8个时钟脉冲
        CS = 1;             
        cmd = 0x41;                          //CMD17
        crc = 0xff;                          //伪CRC
    }
    while(--i);

    #undef cmd
    #undef i   
    #undef timeout
    #undef retry
    #undef temp
    #undef crc
}

/************************ 设置块大小 ************************/
void Set_block_size(unsigned char size)
{
    unsigned char timeout;
    CS = 0;
    SD_write_byte(0x50);     //发送CMD16
    //MOSI = 0;
    SD_spi_read();           //发送0x00
    SD_spi_read();           //发送0x00
    if (size == 1)
    {
        SD_write_byte(2);    //发送0x02
        //MOSI = 0;
        SD_spi_read();       //发送0x00
    }
    else
    {
       SD_spi_read();        //发送0x00
       SD_write_byte(4);     //发送0x04
    }
    MOSI = 1;
    SD_spi_read();           //发送0xff

    timeout = 100;
    do
    {
        if(--timeout == 0) Error(4);//超时报错
    }
    while(SD_spi_read() != 0);
    MOSI = 1;
    SD_spi_read();           //补充8个时钟脉冲
    CS = 1;
}

/*********************** 发送块读命令 ************************/
void SD_send_read_cmd(void)
{
    unsigned char timeout;
    CS = 0;
    SD_write_byte(0x51);     //发送CMD17
    SD_write_byte(Addr.byte[0]);
    SD_write_byte(Addr.byte[1]);
    SD_write_byte(Addr.byte[2]);
    SD_write_byte(Addr.byte[3]);
    MOSI = 1;
    SD_spi_read();           //发送0xff
    timeout = 100;
    do
    {
        if(--timeout == 0) Error(3);
    }
    while(SD_spi_read() != 0);
}

/********************* 结束块读操作 **************************/
void SD_read_block_complete(void)
{
    SD_spi_read();          //忽略两字节校验码
    SD_spi_read();          //
    MOSI = 1;
    SD_spi_read();          //补充8个时钟脉冲
    CS = 1;                 //结束一次块读操作
}
