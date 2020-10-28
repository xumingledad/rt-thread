#ifndef __GUI_WEATHERUPDAT_H__
#define __GUI_WEATHERUPDAT_H__

#include <rthw.h>
#include <rtthread.h>
#include "stdio.h"

/* 天气数据结构体 */
/* 天气数据结构体 */
typedef struct
{
	/* 实况天气数据 */
	char name[32];	
  char city[32]; 	//地名
	char text[32];				//天气预报文字
	char code[32];				//天气预报代码
	char temperature[32];   	//气温
	
	/* 今天、明天、后天天气数据 */
	char date[3][32];			//日期
	char text_day[3][64];	    //白天天气现象文字
	char code_day[3][32];		//白天天气现象代码
	char text_night[3][32];		//晚间天气现象文字
	char code_night[3][64]; 	//晚间天气现象代码
	char high[3][32];			//最高温
	char low[3][32];		//最低温
	char wind_direction[3][64]; //风向
	char wind_speed[3][32];  	//风速，单位km/h（当unit=c时）
	char wind_scale[3][32];  	//风力等级
	char humidity[3][32];//湿度
	char life_suggestion[128][2];	//生活建议
}WeatherType;

extern WeatherType weather_data;

void GetWeather(void);
#endif
