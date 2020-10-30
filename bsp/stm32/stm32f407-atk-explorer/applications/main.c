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

int main(void)
{
	
    rt_device_t dev = RT_NULL;
    char buf[] = "hello rt-thread!\r\n";

    dev = rt_device_find("vcom");
    
    if (dev)
        rt_device_open(dev, RT_DEVICE_FLAG_RDWR);
    else
        return -RT_ERROR;
    
    while (1)
    {
        rt_device_write(dev, 0, buf, rt_strlen(buf));
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}
