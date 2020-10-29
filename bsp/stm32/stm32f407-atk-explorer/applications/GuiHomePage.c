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


#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)//updata
#define ID_IMAGE_0    (GUI_ID_USER + 0x02)
#define ID_TEXT_0    (GUI_ID_USER + 0x03)//ʱ����
#define ID_TEXT_1    (GUI_ID_USER + 0x04)//������
#define ID_TEXT_2    (GUI_ID_USER + 0x05)//����
#define ID_IMAGE_1    (GUI_ID_USER + 0x06)
#define ID_IMAGE_2    (GUI_ID_USER + 0x07)
#define ID_TEXT_3    (GUI_ID_USER + 0x08)//xity
#define ID_TEXT_4    (GUI_ID_USER + 0x09)//sunny
#define ID_TEXT_5    (GUI_ID_USER + 0x0A)//sunny2
#define ID_TEXT_6    (GUI_ID_USER + 0x0B)//1/1
#define ID_TEXT_7    (GUI_ID_USER + 0x0C)//sunny3
#define ID_TEXT_8    (GUI_ID_USER + 0x0D)//12
#define ID_TEXT_9    (GUI_ID_USER + 0x0E)//13
#define ID_TEXT_10    (GUI_ID_USER + 0x0F)//tmp
#define ID_TEXT_11    (GUI_ID_USER + 0x10)//fengsu
#define ID_TEXT_12    (GUI_ID_USER + 0x11)//shidu
#define ID_BUTTON_1   (GUI_ID_USER + 0x12)
//#define ID_TEXT_13 (GUI_ID_USER + 0x14)//updating

#define DS18B20_DATA_PIN    GET_PIN(A, 15)
//#define USING_PNG
static struct tm tm;
char buf[30];
extern char weatherUpdatFlag;

/*********************************************************************
*
*       _acImage_2, "C:\Users\xml46\OneDrive\����\png\99.png", ID_IMAGE_2_IMAGE_0
*/

// USER START (Optionally insert additional static data)
// USER END
/*********************************************************************************************************
*	              ���ڿؼ��б�
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 320, 240, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "update", ID_BUTTON_0, 237, 219, 80, 20, 0, 0x0, 0 },
    { IMAGE_CreateIndirect, "1png", ID_IMAGE_0, 10, 110, 60, 60, 0, 0, 0 },
    { IMAGE_CreateIndirect, "2png", ID_IMAGE_1, 90, 110, 60, 60, 0, 0, 0 },
    { IMAGE_CreateIndirect, "3png", ID_IMAGE_2, 170, 110, 60, 60, 0, 0, 0 },
    { TEXT_CreateIndirect, "Time", ID_TEXT_0, -13, 2, 253, 72, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Date", ID_TEXT_1, 215, 38, 105, 36, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Week", ID_TEXT_2, 218, 10, 110, 23, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "city", ID_TEXT_3, 76, 84, 95, 25, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "wea1", ID_TEXT_4, 0, 210, 80, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "wea2", ID_TEXT_5, 80, 210, 80, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "wea3", ID_TEXT_7, 160, 210, 80, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "tmp1", ID_TEXT_6, 10, 180, 60, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "tmp2", ID_TEXT_8, 90, 180, 60, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "tmp3", ID_TEXT_9, 170, 180, 60, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "temp", ID_TEXT_10, 242, 123, 79, 27, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "wind", ID_TEXT_11, 242, 157, 80, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "hum", ID_TEXT_12, 242, 190, 80, 20, 0, 0x64, 0 },
    { BUTTON_CreateIndirect, "time set", ID_BUTTON_1, 235, 84, 80, 20, 0, 0x0, 0 },
    //{ TEXT_CreateIndirect, "Text", ID_TEXT_13, 268, 19, 45, 16, 0, 0x64, 0 },
    // USER START (Optionally insert additional widgets)
    // USER END
};
/*
*********************************************************************************************************
*	�� �� ��: InitDialogRTC
*	����˵��: RTC�Ի���ص������ĳ�ʼ����Ϣ
*	��    ��: pMsg   ��Ϣָ�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitDialogRTC(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
	  WM_HWIN hWin = pMsg->hWin;
    // USER START (Optionally insert additional variables)
    // USER END
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00808000));
    // Initialization of '1png'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText(hItem, "Updata");
    BUTTON_SetFont(hItem, GUI_FONT_13HB_1);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_D32);
    TEXT_SetText(hItem, "11:29:09");
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x000080FF));
    //
    // Initialization of 'Date'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "2017 02 09");
    TEXT_SetFont(hItem, GUI_FONT_20B_1);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x004080FF));
    //
    // Initialization of 'Week'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_20B_1);
    TEXT_SetText(hItem, "Sunday");
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x000080FF));
    // TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0002CBFD));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "City");
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    //TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x008080FF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "sunny");
    //TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "sunny2");
    //TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "1/1");
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    //TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x000000FF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_7);
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "sunny3");
    // TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_8);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "1/2");
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    //TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x000000FF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_9);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "1/3");
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    //TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x000000FF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_10);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "Temp");
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    //TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x000000FF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_11);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetText(hItem, "Wind");
    //TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_12);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetText(hItem, "Rh");

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetText(hItem, "Time Set");
    BUTTON_SetFont(hItem, GUI_FONT_13HB_1);
    #ifdef USING_PNG
    Show_IMAGE(WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1),"/sdcard/png/99.png");
    Show_IMAGE(WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1),"/sdcard/png/99.png");
    Show_IMAGE(WM_GetDialogItem(pMsg->hWin, ID_IMAGE_2),"/sdcard/png/99.png");
		#endif
	}
/*
*********************************************************************************************************
*	�� �� ��: Clock_Update
*	����˵��: RTC�Ի���ص���������ʱ����Ϣ����
*	��    ��: pMsg   ��Ϣָ�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Clock_Update(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    time_t now=time(NULL);
    gmtime_r(&now, &tm);
    /* ����ʱ�� */
    sprintf(buf, "%0.2d:%0.2d:%0.2d ",tm.tm_hour,tm.tm_min, tm.tm_sec );
    TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_0), buf);
    /* �������� */
    sprintf(buf, "%0.2d:%0.2d:%0.2d ",tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday );
    TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_1), buf);
    /*��������*/
    switch(tm.tm_wday)//ʱ������ת������
    {
    case (01):
        sprintf(buf, "%s ","Monday");
        TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_2), buf);
        break;
    case (02):
        sprintf(buf, "%s", "Tuesday");
        TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_2), buf);
        break;
    case (03):
        sprintf(buf, "%s", "Wednesday");
        TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_2), buf);
        break;
    case (04):
        sprintf(buf, "%s","Thursday");
        TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_2), buf);
        break;
    case (05):
        sprintf(buf, "%s","Friday");
        TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_2), buf);
        break;
    case (06):
        sprintf(buf, "%s", "Saturday");
        TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_2), buf);
        break;
    case (07):
        sprintf(buf, "%s", "Sunday");
        TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_2), buf);
        break;
    }
    int  temperature=ds18b20_get_temperature(DS18B20_DATA_PIN);
    sprintf(buf, "%3d.%dC",temperature / 10,temperature % 10);
    TEXT_SetText(WM_GetDialogItem(hWin,ID_TEXT_10), buf);//�¶ȸ��»ص�����
		
		
		
}
/*
*********************************************************************************************************
*	�� �� ��: _cbDialog
*	����˵��: �Ի���ص�����
*	��    ��: pMsg   ��Ϣָ�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg)
{
    int Id,NCode;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_PRE_PAINT:
			GUI_MULTIBUF_Begin();
        break;//��һ��WM_PAINT��Ϣ����ǰ��WM������Ϣ���͵�����
    case WM_PAINT:/*���ڱ�Ϊ��Ч �����ֻ�ȫ��������Ҫ����ʱ��WM������Ϣ���͵����ڡ�
			�����յ�WM_PAINT��Ϣʱ��Ӧ�ػ�����������Ϣ���͵�����ǰ��WMȷ�����ѱ�ѡ��
			����:����Ϣ��Data.pָ��ָ������Ļ�����а������ڵ���Ч���ε�GUI_RECT�ṹ��
		������Ϣ�������Ż���ɫ���ܡ�*/
        break;
    case WM_POST_PAINT:
			//�����һ��WM_PAINT��Ϣ�����WM����������Ϣ���͵�����
        GUI_MULTIBUF_End();break;
    case WM_INIT_DIALOG:
			/*����Ϣ�ڶԻ��򴴽�����ʾǰ���͵����ڡ��Ի������ͨ��ʹ�ô���Ϣ��ʼ��С���ߣ�
		��ִ��Ӱ��Ի�����۵������κγ�ʼ������*/
        InitDialogRTC(pMsg);	//��ʼ��ʱ�Ӵ���
        break;
    case WM_NOTIFY_PARENT://��֪�������Ӵ��ڷ����ı���Ϣ��Data.vֵ������Ϣ��֪ͨ���롣
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id)
        {case ID_BUTTON_1: // Notifications sent by 'Button'/* ��ȡ�ؼ���� */
            WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);//���ضԻ�����Ŀ ��С���ߣ��Ĵ��ھ��
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED://���
            App_SetDate();
						break;
            case WM_NOTIFICATION_RELEASED://����
        //WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1));
       /*��ָ����������Ϊ����״̬��WM���Ὣָ�������豸(PID)��Ϣ
						����������ꡢ�����ֱ�...�����͵������õĴ���*/
						break;
            }
            break;
        case ID_BUTTON_0:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
							 WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0));//���ô���
                GetWeather();
						    GUI_Delay(2000);
                WM_EnableWindow(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0));	//ʹ�ܴ���
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            }
            break;
        }
        break;
    case WM_TIMER://������ʱ����
        Clock_Update(pMsg);// ��ȡ����
        WM_RestartTimer(pMsg->Data.v, 500);//0.5�����һ��
        break;
    case WM_KEY:
        switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key)
        {
      case GUI_KEY_ESCAPE:
       GUI_EndDialog(hWin, 1);
            break;
        case GUI_KEY_ENTER:
            GUI_EndDialog(hWin, 0);
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);//Ĭ�ϵ���Ϣ���  ������������û�д������Ϣ
        break;
    }
}
/*
*********************************************************************************************************
*	�� �� ��: MainTask
*	����˵��: GUI������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
extern char flag;
char image_path_buff[3][20]={"sdcard/png/99.png","sdcard/png/99.png","sdcard/png/99.png"};
void  GuiMainTask(void)
{
    WM_HWIN hWin;
    char buffer[50];
    WM_SetCreateFlags(WM_CF_MEMDEV);
    hWin= GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, 0, 0, 0);
    WM_CreateTimer(WM_GetClientWindow(hWin),/* ������Ϣ�Ĵ��ڵľ�� */
               0, 	                     /* �û������Id���������ͬһ����ʹ�ö����ʱ������ֵ��������Ϊ��*/
               500,                    /* ���ڣ������ڹ���ָ������Ӧ�յ���Ϣ*/
               0);	                     /*û�������� ���Ž�����*/
    while(1)
    {
        if(weatherUpdatFlag)
        {
            TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3), weather_data.name); //����
            TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_4), weather_data.text_day[0]); //����
            sprintf(buf,"%s-%s",weather_data.high[0],weather_data.low[0]);
            TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_6), buf); //�¶�
					sprintf(buf,"%s %s","Wind:",weather_data.wind_scale[0]);
            TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_11), weather_data.wind_scale[0]);//����
					sprintf(buf,"%s %s","RH  :",weather_data.humidity[0]);
            TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_12), weather_data.humidity[0]);//ʪ��
            

            TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_5), weather_data.text_day[1]); //����
            sprintf(buf,"%s-%s",weather_data.high[1],weather_data.low[1]);
            TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_8), buf); //�¶�
           
            

            TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_7), weather_data.text_day[2]); //����
            sprintf(buf,"%s-%s",weather_data.high[2],weather_data.low[2]);
            TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_9), buf); //�¶�
					  #ifdef 0
					  sprintf(image_path_buff[0],"sdcard/png/%s.png",weather_data.code_day[0]);
            sprintf(image_path_buff[1],"sdcard/png/%s.png",weather_data.code_day[1]);
            sprintf(image_path_buff[2],"sdcard/png/%s.png",weather_data.code_day[2]);
					  Show_IMAGE(WM_GetDialogItem(hWin, ID_IMAGE_0),image_path_buff[0]);
					  Show_IMAGE(WM_GetDialogItem(hWin, ID_IMAGE_1),image_path_buff[1]);
            Show_IMAGE(WM_GetDialogItem(hWin, ID_IMAGE_2),image_path_buff[2]);
						#endif
            weatherUpdatFlag = 0;
        }
        GUI_Delay(100);
    }
}
