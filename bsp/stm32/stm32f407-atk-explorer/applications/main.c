/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2018-11-19     flybreak     add stm32f407-atk-explorer bsp
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "GUI.h"
#include "maintask.h"
#include <GUI_shot.h>
#include <dfs_posix.h>
#include "spi_flash_sfud.h"
#include "DIALOG.h"
//void key_task(void*parameter)
//{
//    rt_uint8_t key;
//	
//    while(1)
//    {
//        key = Key_Scan(0);
//        switch(key)
//        {
//        case KEY_ON:
//            LEDOn (2);
//            create_bmppicture("/sdcard/shot.bmp",0,0,320,240);
//            LEDOff(2);
//            break;
//        }
//        rt_thread_mdelay(100);
//    }
//}
int main(void)//ึ๗าช
{
	 GuiMainTask();
    while(1)
    {
			rt_thread_mdelay(100);
    }
}
