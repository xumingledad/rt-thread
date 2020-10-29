#include "DIALOG.h"
#include <maintask.h>
#include "DIALOG.h"
#include "GUI.h"
#include "GUI_weatherUpdata.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <sys/time.h>
#include <stdio.h>
#include "sensor_dallas_ds18b20.h"
#include <board.h>
#include <dfs_posix.h>
void _WriteByte2File(U8 Data, void * p)
{
    dfs_file_write(p,&Data,sizeof(Data));
}
//生成一个BMP图片，保存到SD卡中,实现屏幕截图功能！
//*filename:文件路径
//x0:创建BMP文件的X起始位置
//y0:创建BMP文件的Y起始位置
//Xsize:X的大小
//Ysize:Y的大小
void create_bmppicture(char *filename,int x0,int y0,int Xsize,int Ysize)
{
    struct dfs_fd hFile;
    unlink(filename);
    GUI_COLOR forecolor,bkcolor;
    forecolor=GUI_GetColor();       //获取当前前景色
    bkcolor=GUI_GetBkColor();       //获取当前背景色
    //创建一个文件，路径为filename,如果文件原本就存在的话会被新建的文件覆盖掉！
    dfs_file_open(&hFile, filename,O_WRONLY | O_CREAT);
    GUI_BMP_SerializeEx(_WriteByte2File,x0,y0,Xsize,Ysize,&hFile);
    dfs_file_close(&hFile);    //关闭文件
    GUI_SetColor(forecolor);        //恢复前景色
    GUI_SetBkColor(bkcolor);        //恢复背景色
}
void Shot_Test(void)
{
    rt_base_t leval;
    //rt_enter_critical();//调度器上锁后不能写入SDIO，但是可以写入flash,不过不上锁也可以
    create_bmppicture("sdcard/5.bmp", 0, 0,320,240);
    //rt_exit_critical();
}
MSH_CMD_EXPORT(Shot_Test,ee);

void Shot(int argc, char **argv)
{

    if (7>argc >= 1)
    {
        char filename1 = atol(argv[1]);
        create_bmppicture(&filename1, 0, 0,320,240);
			  rt_kprintf("%s",filename1);
    }
    else if (argc >= 7)
    {
        char *filename1 = (char*)atol(argv[1]);
        int x0= atol(argv[2]);
        int y0= atol(argv[3]);
        int Xsize= atol(argv[4]);
        int Ysize= atol(argv[5]);

        if (x0>320 || x0<0)
        {
            rt_kprintf("X0 is out of range [0-320]\n");
            return;
        }
        if (y0 <0 ||  y0> 240)
        {
            rt_kprintf("Y0 is out of range [0-240]\n");
            return;
        }
        if (Xsize>320-x0)
        {
            rt_kprintf("Xsize is out of range[%d]\n",320-x0);
            return;
        }
        if (Ysize>240-y0)
        {
            rt_kprintf("Ysize is out of range[%d]\n",240-y0);
            return;
        }
				 rt_kprintf("%s",filename1);
        create_bmppicture(filename1, x0, y0,Xsize,Ysize);
    }
}
MSH_CMD_EXPORT(Shot, e.g: Shot /sdcard/shotscreen.bmp 0 0 320 240);//此功能好像没有实现
/*
*********************************************************************************************************
*	函 数 名: _GetData
*	功能说明: 被函数GUI_PNG_DrawEx调用
*	形    参: p             FIL类型数据
*             NumBytesReq   请求读取的字节数
*             ppData        数据指针
*             Off           如果Off = 1，那么将重新从其实位置读取
*	返 回 值: 返回读取的字节数
*********************************************************************************************************
*/
#define BMPPERLINESIZE	2*1024
void Showpng(const char * sFilename)
{
    char *_acbuffer;
    GUI_HMEM hMem;
    int result,fd;

    /* 打开文件 */
    fd = open(sFilename,O_RDONLY);
    if (fd>0)
    {
        rt_kprintf("open done.\n");
    }

    /* 申请一块内存空间 并且将其清零 */
    hMem = GUI_ALLOC_AllocZero(1024*8);
//
//	/* 将申请到内存的句柄转换成指针类型 */
    _acbuffer = GUI_ALLOC_h2p(hMem);
    rt_malloc(*_acbuffer);
    /* 读取文件到动态内存 */
    result = read(fd, _acbuffer,sizeof(_acbuffer));
    if (result>0)
    {
        rt_kprintf("read done.\n");
    }

    /* 显示PNG图片 */
//	GUI_PNG_Draw(_acBuffer, sizeof(_acBuffer), 0, 0);
//rt_free(_acBuffer);
    /* 释放动态内存hMem */
    GUI_ALLOC_Free(hMem);

    /* 关闭文件 */
    close(fd);
}
int dispbmp(uint8_t *BMPFileName,uint32_t x,uint32_t y)
{
    struct dfs_fd BMPfile;
    char *bmpbuffer;
    char result;
    int XSize,YSize;
    float Xflag,Yflag;
    GUI_COLOR forecolor,bkcolor;
    forecolor=GUI_GetColor();			//获取当前前景色
    bkcolor=GUI_GetBkColor();			//获取当前背景色
    result =dfs_file_open(&BMPfile,(const char*)BMPFileName,O_RDONLY);	//打开文件
    //文件打开错误或者文件大于BMPMEMORYSIZE
    if(result==-1) 	return 1;

    bmpbuffer = rt_malloc(BMPfile.size);//申请内存
    if(bmpbuffer == NULL) return 2;
    result = dfs_file_read(&BMPfile,bmpbuffer,BMPfile.size); //读取数据

    GUI_BMP_Draw(bmpbuffer,x,y);	//在指定位置显示BMP图片
    dfs_file_close(&BMPfile);				//关闭BMPFile文件
    rt_free(bmpbuffer);		//释放内存
    GUI_SetColor(forecolor);		//恢复前景色
    GUI_SetBkColor(bkcolor);		//恢复背景色
    return 0;
}

static char _acBuffer[4096];
int APP_GetData(void * p, const U8 ** ppData, unsigned NumBytes, U32 Off) {
        
        static U32 read_addr=0; int file;
        int NumBytesRead;
        //
        // Check buffer size
        //
        if (NumBytes > sizeof(_acBuffer)) {
        NumBytes = sizeof(_acBuffer);
        }
        //
        // Set file pointer to the required position
        //
        if(Off == 1) read_addr = 0;
        else read_addr = Off;
        
				if(lseek(file,read_addr,SEEK_SET)<0)
				{
					rt_kprintf("off set eror！");
				}
        //
        // Read data into buffer
        //
				//rt_enter_critical();
				NumBytesRead=read(file,_acBuffer,NumBytes);
				if (NumBytesRead<0)
				rt_kprintf("read error");
				//rt_exit_critical();	
        //
        // Set data pointer to the beginning of the buffer
        //
        *ppData = (const U8*)_acBuffer;
        //
        // Return number of available bytes
        //
        return NumBytesRead;
}
void Show_IMAGE(IMAGE_Handle hWin,const char *sFilename)//, char num)
{
	/* 进入临界段 */
	int res;
	struct dfs_fd png;//[num];
	rt_enter_critical();
  res= dfs_file_open(&png, sFilename,O_RDONLY);
	if (res<0)
	{
		rt_kprintf("open failed\n");
		_acBuffer[0]='\0';
	}
	//IMAGE_SetPNGEx(hWin,APP_GetData,&png);
	//f_close(&fnew);
	/* 退出临界段 */
	rt_exit_critical();	
}
void emwin_test(void)
{
    uint8_t x,y;
    __HAL_RCC_CRC_CLK_ENABLE();
    CRC_HandleTypeDef   CrcHandle;
    CrcHandle.Instance = CRC;
    HAL_CRC_Init(&CrcHandle);
    GUI_Init();       
    GUI_SetBkColor(GUI_BLUE);
        GUI_Clear();
      //rt_enter_critical();//调度器上锁后不能写入SDIO
    int fd = open("/sdcard/screenshot.bmp", O_RDONLY);
    if (fd >= 0)
    {
     rt_kprintf("file open OK\n");
     GUI_BMP_DrawEx(APP_GetData,&fd,0,0);
               // rt_exit_critical();
    }
        else
        {
                rt_kprintf("file open file\n");
        }
				//close(fd);
                rt_kprintf("close file\n");
				
}
//INIT_APP_EXPORT(emwin_test);