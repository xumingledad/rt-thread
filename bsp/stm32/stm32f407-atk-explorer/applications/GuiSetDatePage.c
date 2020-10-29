#include "DIALOG.h"
#include "GUI.h"
#include "GUI_weatherUpdata.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include "sensor_dallas_ds18b20.h"
#include <board.h>
#include "ff.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <maintask.h>
/*********************************************************************
*       Defines
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_TEXT_0   (GUI_ID_USER + 0x02)
#define ID_TEXT_1   (GUI_ID_USER + 0x03)
#define ID_SPINBOX_0 (GUI_ID_USER + 0x04)
#define ID_TEXT_2   (GUI_ID_USER + 0x05)
#define ID_TEXT_3   (GUI_ID_USER + 0x06)
#define ID_TEXT_4 (GUI_ID_USER + 0x07)
#define ID_SPINBOX_1 (GUI_ID_USER + 0x08)
#define ID_SPINBOX_2 (GUI_ID_USER + 0x09)
#define ID_SPINBOX_3 (GUI_ID_USER + 0x0A)
#define ID_SPINBOX_4 (GUI_ID_USER + 0x0B)
#define ID_CHECKBOX_0 (GUI_ID_USER + 0x0C)
#define ID_CHECKBOX_1 (GUI_ID_USER + 0x0D)
#define ID_SPINBOX_5 (GUI_ID_USER + 0x0E)
#define ID_BUTTON_1 (GUI_ID_USER + 0x0F)
/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect, "SetDate", ID_WINDOW_0, 0, 0, 320, 240, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 45, 210, 60, 30, 0, 0x0, 0 },//apply
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 50, 0, 90, 30, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 200, 0, 90, 30, 0, 0x64, 0 },
    { SPINBOX_CreateIndirect, "SpinboxalHour", ID_SPINBOX_0, 190, 40, 110, 30, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "hour", ID_TEXT_2, 5, 40, 40, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "MIn", ID_TEXT_3, 4, 97, 40, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_4, 4, 147, 40, 20, 0, 0x64, 0 },
    { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_1, 45, 141, 110, 30, 0, 0x0, 0 },
    { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_2, 44, 90, 110, 30, 0, 0x0, 0 },
    { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_3, 190, 90, 110, 30, 0, 0x0, 0 },
    { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_4, 190, 141, 110, 30, 0, 0x0, 0 },
    { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 50, 181, 80, 20, 0, 0x0, 0 },
    { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_1, 224, 179, 80, 20, 0, 0x0, 0 },
    { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_5, 46, 40, 110, 30, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 240, 210, 60, 30, 0, 0x0, 0 },//cancle
};
/*
*********************************************************************************************************
*	函 数 名: InitDialogSetTimeAlarm
*	功能说明: 回调函数初始化消息
*	形    参: pMsg  消息指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
static void  InitDialogSetTimeAlarm(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    WM_HWIN hWin = pMsg->hWin;
    /*背景选色*/
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00808040));
    /* 按钮初始化 */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText(hItem, "Apply");
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetText(hItem, "Cancle");
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
    /* 文本初始化 */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetText(hItem, "Set Time");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x000000FF));
    TEXT_SetFont(hItem, GUI_FONT_16B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetText(hItem, "Set Alarm");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x001E1EFF));
    TEXT_SetFont(hItem, GUI_FONT_16B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetText(hItem, "Hour");
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x004080FF));
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x004080FF));
    TEXT_SetText(hItem, "Min");
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x004080FF));
    TEXT_SetText(hItem, "Sec");
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    /* 复选框初始化 */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
    CHECKBOX_SetText(hItem, "Save Time");
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
    CHECKBOX_SetText(hItem, "Save Alarm");
    /* 编辑框初始化 */
    EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_0)), GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_1)), GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_2)), GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_3)), GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_4)), GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetTextAlign(SPINBOX_GetEditHandle(WM_GetDialogItem(hWin,ID_SPINBOX_5)), GUI_TA_HCENTER | GUI_TA_VCENTER);

    /* SPINBOX初始化 */
    SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_0), SPINBOX_EDGE_CENTER);
    SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_1), SPINBOX_EDGE_CENTER);
    SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_2), SPINBOX_EDGE_CENTER);
    SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_3), SPINBOX_EDGE_CENTER);
    SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_4), SPINBOX_EDGE_CENTER);
    SPINBOX_SetEdge(WM_GetDialogItem(hWin,ID_SPINBOX_5), SPINBOX_EDGE_CENTER);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
    SPINBOX_SetFont(hItem, GUI_FONT_13HB_1);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1);
    SPINBOX_SetFont(hItem, GUI_FONT_13HB_1);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_2);
    SPINBOX_SetFont(hItem, GUI_FONT_13HB_1);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_3);
    SPINBOX_SetFont(hItem, GUI_FONT_13HB_1);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_4);
    SPINBOX_SetFont(hItem, GUI_FONT_13HB_1);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_5);
    SPINBOX_SetFont(hItem, GUI_FONT_13HB_1);

    SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_5), 0, 23);  /* 小时 */
    SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_2), 0, 59);  /* 分钟 */
    SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_1), 0, 59);  /* 秒钟 */

    SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_0), 0, 23);  /* 闹钟小时 */
    SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_3), 0, 59);  /* 闹钟分钟 */
    SPINBOX_SetRange(WM_GetDialogItem(hWin,ID_SPINBOX_4), 0, 59);  /* 闹钟秒钟 */

    static struct tm tm;
    time_t now=time(NULL);
    gmtime_r(&now, &tm);
    SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_5), tm.tm_hour); /* 时 */
    SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_2), tm.tm_min); /* 分 */
    SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_1), tm.tm_sec); /* 秒 */


    SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_0), tm.tm_hour);   /* 闹钟时 */
    SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_3), tm.tm_min); /* 闹钟分 */
    SPINBOX_SetValue(WM_GetDialogItem(hWin,ID_SPINBOX_4), tm.tm_sec); /* 闹钟秒 */

}

/*
*********************************************************************************************************
*	函 数 名: _cbCallbackSetTimeAlarm
*	功能说明: 回调函数
*	形    参: pMsg  消息指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackSetTimeAlarm(WM_MESSAGE * pMsg)
{
    int     NCode;
    int     Id;
    WM_HWIN hWin = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        InitDialogSetTimeAlarm(pMsg);
        break;
    case WM_KEY:/*用来获取按钮焦点，然后销毁窗口*/
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
    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id)
        {
        case ID_BUTTON_0: // Notifications sent by 'Button'
            switch(NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                if(CHECKBOX_GetState(WM_GetDialogItem(hWin,ID_CHECKBOX_0)) == 1)
                {
                    uint8_t hour, minute,  second;
                    hour = SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_5));
                    minute = SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_2));
                    second	= SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_1));
                    set_time(hour,minute,second);
                }
//								if(CHECKBOX_GetState(WM_GetDialogItem(hWin,ID_CHECKBOX_1)) == 1)
//                {
//                    struct rt_alarm_setup _alarm;
//									  struct rt_alarm *ALARM[10];
//                    _alarm.wktime.tm_hour  = SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_0));
//                   // _alarm.wktime.tm_min = SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_3));
//                    _alarm.wktime.tm_sec	= SPINBOX_GetValue(WM_GetDialogItem(hWin,ID_SPINBOX_4));
//									 if(ALARM[0] == NULL)
//									 { 
//										 ALARM[0] = rt_alarm_create(alarm_callback_entry, &_alarm); }
//                }
                GUI_EndDialog(hWin, 0);
                break;
            }
            break;
        case ID_BUTTON_1://取消按钮销毁窗口
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                GUI_EndDialog(hWin, 0);
                break;
            }
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateSetDate
*/
//extern char hwin[3];
void App_SetTimeAlarm(void)
{
    WM_HWIN hWin;
    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallbackSetTimeAlarm, 0, 0, 0);
    /* 设置为模态窗口 */
    //WM_MakeModal(hWinAlarm);
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
