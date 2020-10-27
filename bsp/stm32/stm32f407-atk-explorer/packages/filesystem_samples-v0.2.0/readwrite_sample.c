/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date             Author      Notes
 *
 */
/*
 * 浠ｇ爜娓呭崟锛氭枃浠惰鍐欎緥瀛�
 *
 * 杩欎釜渚嬪瓙婕旂ず浜嗗浣曡鍐欎竴涓枃浠躲��
 */

//#include <rtthread.h>
//#include <dfs_posix.h>
//#include "ff.h"
///* 褰撻渶瑕佷娇鐢ㄦ枃浠舵搷浣滄椂锛岄渶瑕佸寘鍚繖涓ご鏂囦欢 */
//
//static void readwrite_sample(void)
//{
//    int fd, size;
//   // UINT byte_write;
//    char s[] = "RT-Thread Programmer!", buffer[80];
//
//    rt_kprintf("Write string %s to test.txt.\n", s);
//
//    /* 浠ュ垱寤哄拰璇诲啓妯″紡鎵撳紑 /text.txt 鏂囦欢锛屽鏋滆鏂囦欢涓嶅瓨鍦ㄥ垯鍒涘缓璇ユ枃浠�*/
//    fd = open("sdcard/text.txt", O_WRONLY | O_CREAT);
//    if (fd >= 0)
//    {
//       // f_write((FIL*)fd,s,1,byte_write);
//        //write(fd, s, sizeof(s));
//        close(fd);
//        rt_kprintf("Write done.\n");
//    }
//
//    /* 浠ュ彧璇绘ā寮忔墦寮� /text.txt 鏂囦欢 */
//    fd = open("sdcard/text.txt", O_RDONLY);
//    if (fd >= 0)
//    {
//        size = read(fd, buffer, sizeof(buffer));
//        close(fd);
//        rt_kprintf("Read from file test.txt : %s \n", buffer);
//        if (size < 0)
//            return ;
//    }
//}
///* 瀵煎嚭鍒� msh 鍛戒护鍒楄〃涓� */
//MSH_CMD_EXPORT(readwrite_sample, readwrite sample);
