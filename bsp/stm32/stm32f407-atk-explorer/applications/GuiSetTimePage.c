#include "DIALOG.h"
#include <sys/time.h>
#include <maintask.h>
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x11)
#define ID_BUTTON_B (GUI_ID_USER + 0x13)
#define ID_CHECKBOX_0 (GUI_ID_USER + 0x15)
#define ID_TEXT_0 (GUI_ID_USER + 0x16)
#define ID_TEXT_1 (GUI_ID_USER + 0x17)
#define ID_TEXT_2 (GUI_ID_USER + 0x18)
#define ID_TEXT_3 (GUI_ID_USER + 0x19)
#define ID_TEXT_4 (GUI_ID_USER + 0x1A)
#define ID_TEXT_5 (GUI_ID_USER + 0x1B)


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
const char *apDays[] =
{
    "Sat",
    "Sun",
    "Mon",
    "Tues",
    "Wed",
    "Thur",
    "Fri",
    "Sat",
};

const char *apMonths[]=
{
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "June",
    "July",
    "Aug",
    "Sept",
    "Oct",
    "Nov",
    "Dec",
};
// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect, "cle", ID_WINDOW_0, 0, 0, 320, 240, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "B", ID_BUTTON_B, 280, 189, 40, 50, 0, 0x0, 0 },
    { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 287, 14, 28, 27, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 280, 41, 40, 27, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 280, 62, 40, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_2, 280, 97, 40, 22, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_3, 280, 115, 40, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_4, 280, 135, 40, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_5, 280, 154, 40, 20, 0, 0x64, 0 },
    // USER START (Optionally insert additional widgets)
    // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END
/*
*********************************************************************************************************
*	函 数 名: InitDialogSetTimeAlarm
*	功能说明: 回调函数初始化消息
*	形    参: pMsg  消息指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
static void  InitDialogSetDate(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    WM_HWIN hWin = pMsg->hWin;

    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00808040));

    /* 日历控件初始化 */
    CALENDAR_SetDefaultSize(CALENDAR_SI_HEADER, 30 );
    CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_X, 40 );
    CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_Y, 30 );

    CALENDAR_SetDefaultFont(CALENDAR_FI_CONTENT, GUI_FONT_16B_1);
    CALENDAR_SetDefaultFont(CALENDAR_FI_HEADER, GUI_FONT_16B_1);
    CALENDAR_SetDefaultBkColor(CALENDAR_CI_WEEKDAY, 0xFF00FF);

    CALENDAR_SetDefaultDays(apDays);
    CALENDAR_SetDefaultMonths(apMonths);
    time_t now=time(NULL);
    static struct tm tm;
    gmtime_r(&now, &tm);
    CALENDAR_Create(hWin, 0, 0, tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, 2, GUI_ID_CALENDAR0, WM_CF_SHOW);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_B);
    BUTTON_SetText(hItem, "B");
    //
    // Initialization of 'Checkbox'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
    CHECKBOX_SetText(hItem, "Check");
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_TOP);
    TEXT_SetText(hItem, "Save");
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x000000FF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_TOP);
    TEXT_SetText(hItem, "Date");
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x000000FF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    TEXT_SetText(hItem, "Click");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_TOP);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00000000));
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_TOP);
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    TEXT_SetText(hItem, "To");
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_TOP);
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    TEXT_SetText(hItem, "Set");
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_TOP);
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    TEXT_SetText(hItem, "Time");
}
static void _cbCallbackSetDate(WM_MESSAGE * pMsg)
{
    int     NCode;
    int     Id;
    WM_HWIN hWin = pMsg->hWin;
    CALENDAR_DATE Date;
    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        InitDialogSetDate(pMsg);
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
        case ID_BUTTON_B: // Notifications sent by 'Button'
            switch(NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                /* 是否保存年月日设置 */
                if(CHECKBOX_GetState(WM_GetDialogItem(hWin,ID_CHECKBOX_0)) == 1)
                {
                    CALENDAR_GetSel(WM_GetDialogItem(hWin,GUI_ID_CALENDAR0), &Date);
                    uint32_t year;//注意字节
                    uint8_t month,day ;
                    year = Date.Year;
                    month = Date.Month;
                    day =Date.Day;
                    set_date(year,month,day);
                    CALENDAR_SetDate(WM_GetDialogItem(hWin,GUI_ID_CALENDAR0), &Date);
                }
                GUI_EndDialog(hWin, 0);
                App_SetTimeAlarm();
                break;
            }
            break;
        }
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

void App_SetDate(void)
{
    WM_HWIN hWin;
    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallbackSetDate, 0, 0, 0);
    /* 设置为模态窗口 */
    //WM_MakeModal(hWinAlarm);
}
