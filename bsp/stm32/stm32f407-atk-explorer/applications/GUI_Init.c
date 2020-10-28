#include <rtthread.h>
#include <board.h>
#include "GUI.h"
#include "DIALOG.h"
#include "Touch_Calibration.h"
#include "maintask.h"
#include "sensor.h"

#define LED1_PIN    GET_PIN(E, 4)
#define LED2_PIN    GET_PIN(G, 9)

TOUCH_T eeprom;

extern void Touch_MainTask(void);

//extern  WM_HWIN CreateFramewin(void);
//读取EEPROM中的校准数据，用来判断是否存在数据
void Task_eep2(void)
{
    eeprom_iic_read(0x00, 1, &eeprom.buf[0]);
    eeprom_iic_read(0x01, 1, &eeprom.buf[1]);
    eeprom_iic_read(0x10, 1, &eeprom.buf[2]);
    eeprom_iic_read(0x11, 1, &eeprom.buf[3]);
}

rt_thread_t emWinDemoThread = RT_NULL;
void emWinDemoEntry(void *parameter)
{
    int i;
    Task_eep2();
    CRC_HandleTypeDef   CrcHandle;
    CrcHandle.Instance = CRC;
    HAL_CRC_Init(&CrcHandle);
    //2. 初始化GUI
    GUI_Init();
    /* 开启光标 */
    GUI_CURSOR_Show();
    /* 启用UTF-8编码 */
    GUI_UC_SetEncodeUTF8();
    /* 开启多缓冲 */
    WM_MULTIBUF_Enable(1);
    //_ShowPNG1("/sdcard/png/1.png",0,0);

    if(eeprom.buf[0]==0||eeprom.buf[1]==0||eeprom.buf[2]==0||eeprom.buf[3]==0) //判是否要进行触摸校准
    {
        Touch_MainTask();
        Task_eep2();
    }
    //再次检查
    for(i=0; i<4; i++)
    {
        if(eeprom.buf[i]<100)
        {
            rt_kprintf("EEP read ok ,data is: %d \r\n",eeprom.buf[i]);
            Touch_MainTask();
        }
    }
  LCD_X_Config(); //更新触摸校准数据
    while(1) {
         GUI_Delay(20);
        GUI_TOUCH_Exec();
    }
}

static void emwin(int argc, char* argv[])
{
    if (emWinDemoThread != RT_NULL) {
        rt_thread_delete(emWinDemoThread);
    }
    emWinDemoThread = rt_thread_create("emWinDemo", emWinDemoEntry, RT_NULL,4096,20, 10);
    if(emWinDemoThread != RT_NULL)
        rt_thread_startup(emWinDemoThread);
}
INIT_APP_EXPORT(emwin);
/*************************** End of file ****************************/
