/*****************************************************************************

                         FAT32 文件系统51裁剪版
                      (仅支持根目录文件搜索与读取)

*****************************************************************************/

#include <reg51.h>

union
{
    unsigned long all;
    unsigned char byte[4];
} File_length, Next_cluster;

unsigned long FAT_base;
unsigned long Dat_base;

//unsigned char Cluster_size;
//unsigned char Shift_bits;
#define Cluster_size TH1
#define Shift_bits   TL1


/******************************** 获取磁盘参数 ******************************/
void Get_disk_info(void)
{
    unsigned int  Reserved;     //保留区
    unsigned char Signature;    //启动扇区标识符
    unsigned char i;

    Addr.all = 0;               //指向0号扇区
    SD_send_read_cmd();         //发送读取指令
    Wait_start_token();         //等待SD卡响应

    Signature = SD_spi_read();  //读取首字节
    SD_spi_read();              //跳过1节字节
    Skip_nwords(222);           //跳过444字节

    if (Signature == 0xd7)      //是否为跳指令"0xeb"
    {                           //如是，判断为DBR扇区
        Skip_nwords(0x20);      //空读64字节
    }
    else                        //否则判断为MBR引导扇区
    {
        i = 4;                  //最多为4个分区
        while(1)                //顺序搜索4个分区信息
        {
            Read_nbytes(0x10);  //读取16字节分区信息
            if (Buffer[8] | Buffer[9])         //如不为零，信息有效
            {
              //Addr.byte[0] = Buffer[11];     //获取分区起始扇区号
                Addr.byte[1] = Buffer[10];
                Addr.byte[2] = Buffer[9];
                Addr.byte[3] = Buffer[8];     
                Addr.all = Addr.all << 9;      //转换为地址
                while(--i) Read_nbytes(0x10);  //空读剩余字节
                break; 
            }
            else
            {
                if(--i == 0)  Error(6);        //未找到分区信息，报错
            }
        }
    }

    Signature = SD_spi_read();                 //读取最后两字节
    if (SD_spi_read() != 0x55 || Signature != 0xaa) Error(6);//最后两字节应为0x55aa
    SD_read_block_complete();                  //结束一次块读操作

    //---------------------- 读取DBR扇区进行处理 ---------------------------
    SD_send_read_cmd();                           //发送读取指令
    Wait_start_token();                           //等待SD卡响应
    Read_nbytes(64);                              //读取64字节

    //------------------ 分析扇区结构是否符合FAT32规范 ---------------------
    if (Buffer[0] != 0xeb) Error(7);              //跳转指令0xeb
    if (Buffer[0x0c] != 0x02 ||
        Buffer[0x0b] != 0x00 ||                   //扇区大小为应为512字节
        Buffer[0x2c] != 0x02 ||                   //根目录簇号应为2
       (Buffer[0x2d]|Buffer[0x2e]|Buffer[0x2f]) != 0
        || Buffer[0x10] != 0x02 ||                //FAT表应为两个
        Buffer[0x0d] == 0x00) Error(8);           //每簇扇区数不能为0

    //-------------------------- 保存有用的信息 ----------------------------
    Cluster_size = Buffer[0x0d];
    Reserved = Buffer[0x0e] + (Buffer[0x0f]<<8);  //保留区扇区数
    FAT_base = Addr.all + (Reserved<<9);          //FAT基址 = DBR基址 + 保留区大小
    for(Shift_bits = 9; (Buffer[0x0d] & 1) == 0; Shift_bits++) Buffer[0x0d] >>= 1;
                                                  //计算簇号转为地址时需要移位次数
  //File_length.byte[0] = Buffer[0x27];           //获取每个FAT占用的扇区数(最高字节为0)
    File_length.byte[1] = Buffer[0x26];
    File_length.byte[2] = Buffer[0x25];
    File_length.byte[3] = Buffer[0x24];
    Dat_base = FAT_base + (File_length.all << 10);//根目录地址 = FAT基址 + 两个FAT大小
    Dat_base -= 2ul << Shift_bits;                //由于簇号从2算起，把数据区起始地址向前移2簇

    Skip_nwords(223);                             //略过446字节
    Signature = SD_spi_read();                    //检测最后两字节应为0x55aa
    if (SD_spi_read() != 0x55 || Signature != 0xaa) Error(7);
    SD_read_block_complete();                     //结束一次块读操作
}



/***************************** 搜查根目录中的文件 ****************************/
unsigned char Scan_file(unsigned char n, unsigned char File_type)
{
    unsigned char Cluster_count;
    unsigned char Sector_count;
    unsigned char i,j,k;
    unsigned char code Rev[16] =            //ASCII字符高低位倒序转换表
    {
        0x0c,0x8c,0x4c,0xcc,0x2c,0xac,0x6c,0xec,0x1c,0x9c,  //'0' ~ '9'
        0x42,0xb2,0x0a,0xea,0x82,0x6a,                      //"BMP" "WAV"
    };

    for (k = 3; k != 0; k--)                //预载文件名于Buffer，以便进行比较
    {
        Buffer[k + 7] = Rev[k + File_type]; //根据扩展名，载入"BMP"或"WAV"的倒序码
        Buffer[k - 1] = Rev[n%10];          //依次载入个十百位的倒序码
        n = n / 10;                         //十进制右移一位
    }  

    Cluster_count = 100;                    //限制根目录搜索深度，防止数据错误导致死读
    Next_cluster.all = 2;                   //根目录从簇号2开始
    do                                      //按簇链依次进行文件搜索
    {
        Sector_count = 0;
        Addr.all = Dat_base + (Next_cluster.all << Shift_bits); //计算簇基址
        do                                  //搜索簇内各个扇区
        {  
            SD_send_read_cmd();             //发送读取指令
            Addr.all += 512;                //计算下一扇区地址
            Wait_start_token();             //等待SD卡响应
            for (j = 16; j != 0; j--)       //搜索扇区内各个文件记录
            {   
                for (i = 0; i != 12; i++)   //比较文件名的11个字符
                {                           //只比较文件名前三位数字和扩展名，其它忽略
                    if (SD_spi_read() != Buffer[i] && (i & 0x07) < 3) break;
                }
                 
                if (i == 12)                                  //匹配成功
                {
                    Cluster_count = 0;                        //清零簇计数器，标记搜索完成
                    Read_nbytes(20);                          //获取文件记录信息
                    Next_cluster.byte[0] = Buffer[0x15-12];   //获取文件的起始簇号
                    Next_cluster.byte[1] = Buffer[0x14-12];
                    Next_cluster.byte[2] = Buffer[0x1b-12];
                    Next_cluster.byte[3] = Buffer[0x1a-12];
                    Addr.all = Dat_base + (Next_cluster.all << Shift_bits);//计算得文件的起始地址

                    File_length.byte[0] = Buffer[0x1f-12];    //获取文件的长度
                    File_length.byte[1] = Buffer[0x1e-12];
                    File_length.byte[2] = Buffer[0x1d-12];
                    File_length.byte[3] = Buffer[0x1c-12];
                    File_length.all = File_length.all >> 9;   //计算文件占用的扇区数量

                    while(--j) Skip_nwords(16);               //读完一扇区的剩余字节
                    break;
                }
                else                                          //匹配不成功，则略过剩余字节
                {
                    i = 31 - i;
                    do
                    {
                         SD_spi_read();
                    }
                    while(--i);
                }
            }
            SD_read_block_complete();                         //结束一次块读操作

            if (Cluster_count == 0) return 1;                 //搜索成功，返回
        }
        while(++Sector_count != Cluster_size);                //继续搜索簇内下一扇区

        //------------------------- 根据簇链取得下一簇号 ------------------------------
        Set_block_size(0);                                    //设定SD卡块大小为4字节
        Addr.all = FAT_base + (Next_cluster.all << 2);        //根据簇链计算链表地址
        SD_send_read_cmd();                                   //发送读取指令
        Wait_start_token();                                   //等待SD卡响应
        Next_cluster.byte[3] = SD_read_byte();//获取下一簇簇号
        Next_cluster.byte[2] = SD_read_byte();
        Next_cluster.byte[1] = SD_read_byte();
        Next_cluster.byte[0] = SD_read_byte();
        SD_read_block_complete();             //结束一次块读操作

        Set_block_size(1);                    //重新设定SD卡块大小为512字节
        if (( Next_cluster.byte[3]            //判断是否为簇链结束标志 0x0fffffff
            & Next_cluster.byte[2]
            & Next_cluster.byte[1]) == 0xff
           && Next_cluster.byte[0]  == 0x0f)
        break;
    }
    while(--Cluster_count != 0);              //循环搜索下一簇，直至达到最大数目
    return 0;
}
