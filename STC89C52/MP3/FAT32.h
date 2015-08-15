#include <sd.h>
#include <ctype.h>
/*
 分区记录结构如下
*/
struct PartRecord
{
 unsigned char Active;         //0x80表示此分区有效
 unsigned char StartHead;      //分区的开始头
 unsigned char StartCylSect[2];//开始柱面与扇区
 unsigned char PartType;       //分区类型
 unsigned char EndHead;        //分区的结束头
 unsigned char EndCylSect[2];  //结束柱面与扇区
 unsigned char StartLBA[4];    //分区的第一个扇区
 unsigned char Size[4];        //分区的大小 
};

/*
 分区扇区（绝对0扇区）定义如下
*/
struct PartSector
{
 unsigned char  PartCode[446]; //MBR的引导程序
 struct PartRecord Part[4];      //4个分区记录
 unsigned char BootSectSig0;
 unsigned char BootSectSig1;
};

struct FAT32_FAT_Item
{
 unsigned char Item[4];
};

struct FAT32_FAT
{
 struct FAT32_FAT_Item Items[128];
};

/*
 FAT32中对BPB的定义如下  一共占用90个字节
*/
struct FAT32_BPB
{
 unsigned char BS_jmpBoot[3];     //跳转指令            offset: 0
 unsigned char BS_OEMName[8];     //                    offset: 3
 unsigned char BPB_BytesPerSec[2];//每扇区字节数        offset:11
 unsigned char BPB_SecPerClus[1]; //每簇扇区数          offset:13
 unsigned char BPB_RsvdSecCnt[2]; //保留扇区数目        offset:14
 unsigned char BPB_NumFATs[1];    //此卷中FAT表数       offset:16
 unsigned char BPB_RootEntCnt[2]; //FAT32为0            offset:17
 unsigned char BPB_TotSec16[2];   //FAT32为0            offset:19
 unsigned char BPB_Media[1];      //存储介质            offset:21
 unsigned char BPB_FATSz16[2];    //FAT32为0            offset:22
 unsigned char BPB_SecPerTrk[2];  //磁道扇区数          offset:24
 unsigned char BPB_NumHeads[2];   //磁头数              offset:26
 unsigned char BPB_HiddSec[4];    //FAT区前隐扇区数     offset:28
 unsigned char BPB_TotSec32[4];   //该卷总扇区数        offset:32

 unsigned char BPB_FATSz32[4];    //一个FAT表扇区数     offset:36
 unsigned char BPB_ExtFlags[2];   //FAT32特有           offset:40
 unsigned char BPB_FSVer[2];      //FAT32特有           offset:42
 unsigned char BPB_RootClus[4];   //根目录簇号          offset:44
 unsigned char FSInfo[2];         //保留扇区FSINFO扇区数offset:48
 unsigned char BPB_BkBootSec[2];  //通常为6             offset:50
 unsigned char BPB_Reserved[12];  //扩展用              offset:52
 unsigned char BS_DrvNum[1];      //                    offset:64
 unsigned char BS_Reserved1[1];   //                    offset:65
 unsigned char BS_BootSig[1];     //                    offset:66
 unsigned char BS_VolID[4];       //                    offset:67
 unsigned char BS_FilSysType[11]; //	                offset:71
 unsigned char BS_FilSysType1[8]; //"FAT32    "         offset:82
};

// Structure of a dos directory entry.
struct direntry 
{
 unsigned char deName[8];       // filename, blank filled
 unsigned char deExtension[3]; 	// extension, blank filled
 unsigned char deAttributes;   	// file attributes
 unsigned char deLowerCase;    	// NT VFAT lower case flags  (set to zero)
 unsigned char deCHundredth;   	// hundredth of seconds in CTime
 unsigned char deCTime[2];     	// create time
 unsigned char deCDate[2];     	// create date
 unsigned char deADate[2];     	// access date
 unsigned char deHighClust[2];     // high unsigned chars of cluster number
 unsigned char deMTime[2];     	// last update time
 unsigned char deMDate[2];     	// last update date
 unsigned char deLowCluster[2]; 	// starting cluster of file
 unsigned char deFileSize[4];      // size of file in unsigned chars
};

// Stuctures
struct FileInfoStruct
{
 unsigned char  FileName[12];
 unsigned long  FileStartCluster;			//< file starting cluster for last file accessed
 unsigned long  FileCurCluster;
 unsigned long  FileNextCluster;
 unsigned long  FileSize;					//< file size for last file accessed
 unsigned char  FileAttr;					//< file attr for last file accessed
 unsigned short FileCreateTime;			//< file creation time for last file accessed
 unsigned short FileCreateDate;			//< file creation date for last file accessed
 unsigned short FileMTime;
 unsigned short FileMDate;
 unsigned long  FileSector;				    //<file record place
 unsigned int   FileOffset;				    //<file record offset
};

/*
  FAT32初始化时初始参数装入如下结构体中
*/
struct FAT32_Init_Arg
{
 unsigned char BPB_Sector_No;   //BPB所在扇区号
 unsigned long Total_Size;            //磁盘的总容量
 unsigned long FirstDirClust;       //根目录的开始簇
 unsigned long FirstDataSector;	 //文件数据开始扇区号
 unsigned int  BytesPerSector;	 //每个扇区的字节数
 unsigned int  FATsectors;            //FAT表所占扇区数
 unsigned int  SectorsPerClust;	 //每簇的扇区数
 unsigned long FirstFATSector;	 //第一个FAT表所在扇区
 unsigned long FirstDirSector;	 //第一个目录所在扇区
 unsigned long RootDirSectors;	 //根目录所占扇区数
 unsigned long RootDirCount;	 //根目录下的目录与文件数
};

//#define FIND_BPB_UP_RANGE 2000  //BPB不一定在0扇区，对0～FINE_BPB_UP_RANGE扇区进行扫描


unsigned char xdata FAT32_Buffer[512]; //扇区数据读写缓冲区
struct FAT32_Init_Arg Init_Arg;	       //初始化参数结构体实体
struct FileInfoStruct FileInfo;        //文件信息结构体实体

unsigned char * FAT32_ReadSector(unsigned long LBA,unsigned char *buf) //FAT32中读取扇区的函数
{
 MMC_get_data_LBA(LBA,512,buf);
 return buf;
}

unsigned char FAT32_WriteSector(unsigned long LBA,unsigned char *buf)//FAT32中写扇区的函数
{
 return MMC_write_sector(LBA,buf);
}

unsigned long lb2bb(unsigned char *dat,unsigned char len) //小端转为大端
{
 unsigned long temp=0;
 unsigned long fact=1;
 unsigned char i=0;
 for(i=0;i<len;i++)
 {
  temp+=dat[i]*fact;
  fact*=256;
 }
 return temp;
}

unsigned long  FAT32_FindBPB()  //寻找BPB所在的扇区号
{
 /*
 unsigned long Range=0;
 while(Range<FIND_BPB_UP_RANGE)
 { 
  FAT32_ReadSector(Range,FAT32_Buffer);
  if(FAT32_Buffer[0]==0xeb && FAT32_Buffer[2]==0x90)
   break;
  Range++;
 }
 return Range;*/
 FAT32_ReadSector(0,FAT32_Buffer);
 return lb2bb(((((struct PartSector *)(FAT32_Buffer))->Part[0]).StartLBA),4);
}

unsigned long FAT32_Get_Total_Size() //存储器的总容量，单位为M
{
 FAT32_ReadSector(Init_Arg.BPB_Sector_No,FAT32_Buffer);
 return ((float)(lb2bb((((struct FAT32_BPB *)(FAT32_Buffer))->BPB_TotSec32),4)))*0.0004883;
}

void FAT32_Init(struct FAT32_Init_Arg *arg)
{
 struct FAT32_BPB *bpb=(struct FAT32_BPB *)(FAT32_Buffer);             //将数据缓冲区指针转为struct FAT32_BPB 型指针
 arg->BPB_Sector_No   =FAT32_FindBPB();                                             //FAT32_FindBPB()可以返回BPB所在的扇区号
 arg->Total_Size      =FAT32_Get_Total_Size();                                        //FAT32_Get_Total_Size()可以返回磁盘的总容量，单位是兆
 arg->FATsectors      =lb2bb((bpb->BPB_FATSz32)    ,4);                       //装入FAT表占用的扇区数到FATsectors中
 arg->FirstDirClust   =lb2bb((bpb->BPB_RootClus)   ,4);                        //装入根目录簇号到FirstDirClust中
 arg->BytesPerSector  =lb2bb((bpb->BPB_BytesPerSec),2);                   //装入每扇区字节数到BytesPerSector中
 arg->SectorsPerClust =lb2bb((bpb->BPB_SecPerClus) ,1);                   //装入每簇扇区数到SectorsPerClust 中
 arg->FirstFATSector  =lb2bb((bpb->BPB_RsvdSecCnt) ,2)+arg->BPB_Sector_No;//装入第一个FAT表扇区号到FirstFATSector 中
 arg->RootDirCount    =lb2bb((bpb->BPB_RootEntCnt) ,2);                   //装入根目录项数到RootDirCount中
 arg->RootDirSectors  =(arg->RootDirCount)*32>>9;                           //装入根目录占用的扇区数到RootDirSectors中
 arg->FirstDirSector  =(arg->FirstFATSector)+(bpb->BPB_NumFATs[0])*(arg->FATsectors); //装入第一个目录扇区到FirstDirSector中
 arg->FirstDataSector =(arg->FirstDirSector)+(arg->RootDirSectors); //装入第一个数据扇区到FirstDataSector中
}

void FAT32_EnterRootDir()
{
 unsigned long iRootDirSector;
 unsigned long iDir;
 struct direntry *pDir;
 for(iRootDirSector=(Init_Arg.FirstDirSector);iRootDirSector<(Init_Arg.FirstDirSector)+(Init_Arg.SectorsPerClust);iRootDirSector++)
 {
  FAT32_ReadSector(iRootDirSector,FAT32_Buffer);
  for(iDir=0;iDir<Init_Arg.BytesPerSector;iDir+=sizeof(struct direntry))
  {
   pDir=((struct direntry *)(FAT32_Buffer+iDir));
   if((pDir->deName)[0]!=0x00 /*无效目录项*/ && (pDir->deName)[0]!=0xe5 /*无效目录项*/ && (pDir->deName)[0]!=0x0f /*无效属性*/)
   {
    SerialPutString(pDir->deName);
   }
  }
 }
}

void FAT32_CopyName(unsigned char *Dname,unsigned char *filename)
{
 unsigned char i=0;
 for(;i<11;i++)
 {
  Dname[i]=filename[i];
 }
 Dname[i]=0;
}

unsigned long FAT32_EnterDir(char *path)
{
 unsigned long iDirSector;
 unsigned long iCurSector=Init_Arg.FirstDirSector;
 unsigned long iDir;
 struct direntry *pDir;
// unsigned char DirName[12];
 unsigned char depth=0,i=0;
 while(path[i]!=0)
 {
  if(path[i]=='\\')
  { 
   depth++;
  }
  i++;
 }
 if(depth==1)
 {
  return iCurSector;    //如果是根目录，直接返回当前扇区号
 }
 for(iDirSector=iCurSector;iDirSector<(Init_Arg.FirstDirSector)+(Init_Arg.SectorsPerClust);iDirSector++)
 {
  FAT32_ReadSector(iDirSector,FAT32_Buffer);
  for(iDir=0;iDir<Init_Arg.BytesPerSector;iDir+=sizeof(struct direntry))
  {
   pDir=((struct direntry *)(FAT32_Buffer+iDir));
   if((pDir->deName)[0]!=0x00 /*无效目录项*/ && (pDir->deName)[0]!=0xe5 /*无效目录项*/ && (pDir->deName)[0]!=0x0f /*无效属性*/)
   {
    SerialPutString(pDir->deName);
   }
  }
 }
}

unsigned char FAT32_CompareName(unsigned char *sname,unsigned char *dname)
{
 unsigned char i,j=8;
 unsigned char name_temp[12];
 for(i=0;i<11;i++) name_temp[i]=0x20;
 name_temp[11]=0;
 i=0;
 while(sname[i]!='.')
 {
  name_temp[i]=sname[i];
  i++;
 }
 i++;
 while(sname[i]!=0)
 {
  name_temp[j++]=sname[i];
  i++;
 }
 //Printf(name_temp,0);
 for(i=0;i<11;i++)
 {
  if(name_temp[i]!=dname[i]) return 0;
 }
  //Printf(name_temp,0);
 return 1;
}

unsigned long FAT32_GetNextCluster(unsigned long LastCluster)
{
 unsigned long temp;
 struct FAT32_FAT *pFAT;
 struct FAT32_FAT_Item *pFAT_Item;
 temp=((LastCluster/128)+Init_Arg.FirstFATSector);
 FAT32_ReadSector(temp,FAT32_Buffer);
 pFAT=(struct FAT32_FAT *)FAT32_Buffer;
 pFAT_Item=&((pFAT->Items)[LastCluster%128]);
 return lb2bb(pFAT_Item,4);
}

struct FileInfoStruct * FAT32_OpenFile(char *filepath)
{
 unsigned char depth=0;
 unsigned char i,index;
 unsigned long iFileSec,iCurFileSec,iFile;
 struct direntry *pFile;
 unsigned char len=strlen(filepath);
 for(i=0;i<len;i++)
 {
  if(filepath[i]=='\\')
  { 
   depth++;
   index=i+1;
  }
 }
 iCurFileSec=FAT32_EnterDir(filepath)/*Init_Arg.FirstDirSector*/;
 Printf("iCurFileSec",iCurFileSec);
 for(iFileSec=iCurFileSec;iFileSec<iCurFileSec+(Init_Arg.SectorsPerClust);iFileSec++)
 {
  FAT32_ReadSector(iFileSec,FAT32_Buffer);
  for(iFile=0;iFile<Init_Arg.BytesPerSector;iFile+=sizeof(struct direntry))
  {
   pFile=((struct direntry *)(FAT32_Buffer+iFile));
   if(FAT32_CompareName(filepath+index,pFile->deName))
   {
    //PutHex(pFile->deFileSize[0]);
	//PutHex(pFile->deFileSize[1]);
	//PutHex(pFile->deFileSize[2]);
	//PutHex(pFile->deFileSize[3]);
    FileInfo.FileSize=lb2bb(pFile->deFileSize,4);
	strcpy(FileInfo.FileName,filepath+index);
	FileInfo.FileStartCluster=lb2bb(pFile->deLowCluster,2)+lb2bb(pFile->deHighClust,2)*65536;
	FileInfo.FileCurCluster=FileInfo.FileStartCluster;
	FileInfo.FileNextCluster=FAT32_GetNextCluster(FileInfo.FileCurCluster);
	FileInfo.FileOffset=0;
	Printf("FileStartCluster",FileInfo.FileStartCluster);
	return &FileInfo;
   }
  }
 }
}



void FAT32_ReadFile(struct FileInfoStruct *pstru)
{
 unsigned long Sub=pstru->FileSize-pstru->FileOffset;
 unsigned long iSectorInCluster=0;
 unsigned long i=0;
 while(pstru->FileNextCluster!=0x0fffffff)  //如果FAT中的簇项为0x0fffffff，说明无后继簇
 {
  for(iSectorInCluster=0;iSectorInCluster<Init_Arg.SectorsPerClust;iSectorInCluster++)   //读出整簇数据
  {
   FAT32_ReadSector((((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster),FAT32_Buffer);
   pstru->FileOffset+=Init_Arg.BytesPerSector;
   Sub=pstru->FileSize-pstru->FileOffset;
   for(i=0;i<Init_Arg.BytesPerSector;i++)
   {
    send(FAT32_Buffer[i]);   //将数据发送到终端上显示
   }
  }
  pstru->FileCurCluster=pstru->FileNextCluster;  
  pstru->FileNextCluster=FAT32_GetNextCluster(pstru->FileCurCluster);   //这里是FAT簇链的传递
 }
 iSectorInCluster=0;
 while(Sub>=Init_Arg.BytesPerSector)   //处理不足一簇，而足扇区的数据
 {
  FAT32_ReadSector((((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster++),FAT32_Buffer);
  pstru->FileOffset+=Init_Arg.BytesPerSector;
  Sub=pstru->FileSize-pstru->FileOffset;
  for(i=0;i<Init_Arg.BytesPerSector;i++)
  {
   send(FAT32_Buffer[i]);
  }
 }
 FAT32_ReadSector((((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster),FAT32_Buffer); //读取最后一个扇区
 for(i=0;i<Sub;i++)    //Sub为最后剩余的字节数
 {
  send(FAT32_Buffer[i]);
 }	
}











