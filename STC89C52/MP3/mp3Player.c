#include <string.h>
#include <stdio.h>
#include <math.h>
#include <Serial.h>
#include <fat32.h>

void main()
{
 //delay(10000);
 SerialInit();  //串口初始化，用以向调试终端发送数据
 while(1)
 {
 SerialPutString("yahoo!!!"); //发送一个测试字符串
 }
/*
 MMC_Init(); //SD卡初始化
 delay(10000);
 MMC_get_volume_info();	  //获得SD卡相关信息，输出到终端
 FAT32_Init(&Init_Arg);	  //FAT32文件系统初始化，装入参数
 Printf("BPB_Sector_No"  ,Init_Arg.BPB_Sector_No);
 Printf("Total_Size"     ,Init_Arg.Total_Size   );
 Printf("FirstDirClust"  ,Init_Arg.FirstDirClust); 
 Printf("FirstDataSector",Init_Arg.FirstDataSector); 
 Printf("BytesPerSector" ,Init_Arg.BytesPerSector); 
 Printf("FATsectors"     ,Init_Arg.FATsectors); 
 Printf("SectorsPerClust",Init_Arg.SectorsPerClust);
 Printf("FirstFATSector" ,Init_Arg.FirstFATSector); 
 Printf("FirstDirSector" ,Init_Arg.FirstDirSector);   //以上几个语句用以输出参数值到终端
 Printf("FAT32_OpenFile" ,(FAT32_OpenFile("\\TEST.TXT"))->FileSize); //打开根目录下的TEST.TXT文件，并输出文件大小 
 FAT32_ReadFile(&FileInfo);  //读取文件数据，输出到终端
*/
// while(1);
}
