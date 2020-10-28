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
//����һ��BMPͼƬ�����浽SD����,ʵ����Ļ��ͼ���ܣ�
//*filename:�ļ�·��
//x0:����BMP�ļ���X��ʼλ��
//y0:����BMP�ļ���Y��ʼλ��
//Xsize:X�Ĵ�С
//Ysize:Y�Ĵ�С
void create_bmppicture(char *filename,int x0,int y0,int Xsize,int Ysize)
{
    struct dfs_fd hFile;
    unlink(filename);
    GUI_COLOR forecolor,bkcolor;
    forecolor=GUI_GetColor();       //��ȡ��ǰǰ��ɫ
    bkcolor=GUI_GetBkColor();       //��ȡ��ǰ����ɫ
    //����һ���ļ���·��Ϊfilename,����ļ�ԭ���ʹ��ڵĻ��ᱻ�½����ļ����ǵ���
    dfs_file_open(&hFile, filename,O_WRONLY | O_CREAT);
    GUI_BMP_SerializeEx(_WriteByte2File,x0,y0,Xsize,Ysize,&hFile);
    dfs_file_close(&hFile);    //�ر��ļ�
    GUI_SetColor(forecolor);        //�ָ�ǰ��ɫ
    GUI_SetBkColor(bkcolor);        //�ָ�����ɫ
}
void Shot_Test(void)
{
    rt_base_t leval;
    //rt_enter_critical();//��������������д��SDIO�����ǿ���д��flash,����������Ҳ����
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
MSH_CMD_EXPORT(Shot, e.g: Shot /sdcard/shotscreen.bmp 0 0 320 240);//�˹��ܺ���û��ʵ��
/*
*********************************************************************************************************
*	�� �� ��: _GetData
*	����˵��: ������GUI_PNG_DrawEx����
*	��    ��: p             FIL��������
*             NumBytesReq   �����ȡ���ֽ���
*             ppData        ����ָ��
*             Off           ���Off = 1����ô�����´���ʵλ�ö�ȡ
*	�� �� ֵ: ���ض�ȡ���ֽ���
*********************************************************************************************************
*/
#define BMPPERLINESIZE	2*1024
void Showpng(const char * sFilename)
{
    char *_acbuffer;
    GUI_HMEM hMem;
    int result,fd;

    /* ���ļ� */
    fd = open(sFilename,O_RDONLY);
    if (fd>0)
    {
        rt_kprintf("open done.\n");
    }

    /* ����һ���ڴ�ռ� ���ҽ������� */
    hMem = GUI_ALLOC_AllocZero(1024*8);
//
//	/* �����뵽�ڴ�ľ��ת����ָ������ */
    _acbuffer = GUI_ALLOC_h2p(hMem);
    rt_malloc(*_acbuffer);
    /* ��ȡ�ļ�����̬�ڴ� */
    result = read(fd, _acbuffer,sizeof(_acbuffer));
    if (result>0)
    {
        rt_kprintf("read done.\n");
    }

    /* ��ʾPNGͼƬ */
//	GUI_PNG_Draw(_acBuffer, sizeof(_acBuffer), 0, 0);
//rt_free(_acBuffer);
    /* �ͷŶ�̬�ڴ�hMem */
    GUI_ALLOC_Free(hMem);

    /* �ر��ļ� */
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
    forecolor=GUI_GetColor();			//��ȡ��ǰǰ��ɫ
    bkcolor=GUI_GetBkColor();			//��ȡ��ǰ����ɫ
    result =dfs_file_open(&BMPfile,(const char*)BMPFileName,O_RDONLY);	//���ļ�
    //�ļ��򿪴�������ļ�����BMPMEMORYSIZE
    if(result==-1) 	return 1;

    bmpbuffer = rt_malloc(BMPfile.size);//�����ڴ�
    if(bmpbuffer == NULL) return 2;
    result = dfs_file_read(&BMPfile,bmpbuffer,BMPfile.size); //��ȡ����

    GUI_BMP_Draw(bmpbuffer,x,y);	//��ָ��λ����ʾBMPͼƬ
    dfs_file_close(&BMPfile);				//�ر�BMPFile�ļ�
    rt_free(bmpbuffer);		//�ͷ��ڴ�
    GUI_SetColor(forecolor);		//�ָ�ǰ��ɫ
    GUI_SetBkColor(bkcolor);		//�ָ�����ɫ
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
					rt_kprintf("off set eror��");
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
	/* �����ٽ�� */
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
	/* �˳��ٽ�� */
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
      //rt_enter_critical();//��������������д��SDIO
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