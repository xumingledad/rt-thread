//#include "GUI_Test.h"
#include "GUI.h"
//#include "GUIDemo.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

static void test_thread(void *param)
{
CRC_HandleTypeDef   CrcHandle;
	CrcHandle.Instance = CRC; 
	HAL_CRC_Init(&CrcHandle);
	GUI_Init();    
	//GUIDEMO_Main();
	time_t now; /* 保存获取的当前时间值 */
	GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_BLACK); 
	GUI_SetFont(&GUI_Font24_ASCII); 
	GUI_Clear();
  rt_kprintf("%s\n", ctime(&now));
	while(1)
	{
		now = time(RT_NULL);
		GUI_DispStringAt( ctime(&now),10,10);
		rt_thread_mdelay(1000);
	}
}

static int lcd_RTC(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("gui_test",
                           test_thread, RT_NULL,
                           2048,
    RT_THREAD_PRIORITY_MAX - 1, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return RT_EOK;
}
INIT_APP_EXPORT(lcd_RTC);
