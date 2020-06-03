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
#include <stdio.h>
#include "cJSON.h"

/* defined the LED0 pin: PF9 */
#define LED0_PIN    GET_PIN(E, 3)
#define LED1_PIN    GET_PIN(E, 4)
#define LED2_PIN    GET_PIN(G, 4)

int main(void)
  {


    return RT_EOK;
}
