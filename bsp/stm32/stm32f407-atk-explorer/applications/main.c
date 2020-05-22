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
// FILE            *fp = NULL;
//    cJSON           *json;
//    char            *out;
//    char            line[1024] = {0};

//    if(NULL != (fp = fopen("./test.ply", "r"))) {
//        while (NULL != fgets(line, sizeof(line), fp))
//					{
//            json=cJSON_Parse(line); //��ȡ������ľ��
//            out=cJSON_Print(json);  //����ǿ�������ġ�Ϊ��ȡ������json��ֵ
//            cJSON *arrayItem = cJSON_GetObjectItem(json,"syslog_db"); //��ȡ��������Ա
//            cJSON *object = cJSON_GetArrayItem(arrayItem,0);   //��Ϊ��������Ǹ������ȡ����ֻ��һ��Ԫ������д�±�Ϊ
//            cJSON *item = cJSON_GetObjectItem(object,"db_user");  //
//            printf("db_user:%s\n",item->valuestring);

//            item = cJSON_GetObjectItem(object,"db_password");
//            printf("db_password:%s\n",item->valuestring);

//            item = cJSON_GetObjectItem(object,"db_type");
//            printf("db_type:%s\n",item->valuestring);

//            item = cJSON_GetObjectItem(object,"db_ip");
//            printf("db_ip:%s\n",item->valuestring);

//            item = cJSON_GetObjectItem(object,"db_port");
//            printf("db_port:%s\n",item->valuestring);

//            item = cJSON_GetObjectItem(object,"db_name");
//            printf("db_name:%s\n",item->valuestring);

//            item = cJSON_GetObjectItem(object,"sql");
//            printf("db_sql:%s\n",item->valuestring);
//						/*���������ֱ�ӿ��Ի�ȡֵ��*/
//            arrayItem = cJSON_GetObjectItem(json,"syslog_enable");
//            printf("%s\n",arrayItem->valuestring);
//        }
//    }
//        cJSON_Delete(json);
//    fclose(fp);

    return RT_EOK;
}
