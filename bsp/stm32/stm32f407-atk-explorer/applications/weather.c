#include "GUI_weatherUpdata.h"
#include <webclient.h>  /* 使用 HTTP 协议与服务器通信需要包含此头文件 */
#include <sys/socket.h> /* 使用BSD socket，需要包含socket.h头文件 */
#include <netdb.h>
#include <cJSON.h>
#include <finsh.h>
#include <dfs_posix.h>
#pragma diag_suppress 870
/* 心知天气（api.seniverse.com）--天气数据的来源 */
#define WEATHER_IP 		"116.62.81.138"
#define WEATHER_PORT 	"80"
#define GET_URL_LEN_MAX         256         //网址最大长度
#define GET_URI                 "http://api.seniverse.com/v3/weather/daily.json?key=npk2xtj6qv1ayzcg&location=%s&language=en" //获取天气的 API]
/* 定义一些全局变量 */
//------------------------------------------------------------------------------------------
//
//所需要的天气数据,供其他应用使用
WeatherType weather_data;
char weatherUpdatFlag = 0;

/* 天气数据解析 */
static int weather_data_parse(char *data)
{
    cJSON *root, *arrayItem,*object,*subobject,*item;
    //char buffer[50];
    root =  cJSON_Parse(data);//解析JSON数据包
    if(root == NULL)
    {
        printf("数据解析出错.错误:[%s] \r\n",cJSON_GetErrorPtr() );
        return 1;
    }
    else
    {
        if((arrayItem = cJSON_GetObjectItem(root,"results")) != NULL) //匹配字符串"results",获取数组内容
        {
            int size = cJSON_GetArraySize(arrayItem);     //获取数组中对象个数
            if((object = cJSON_GetArrayItem(arrayItem,0)) != NULL)
            {
                printf("********查询天气*******\r\n");
                if((subobject = cJSON_GetObjectItem(object,"location")) != NULL) //查询地点
                {
                    if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) //查询地名
                    {
                        printf("地名:%s  \r\n",item->valuestring); 
                        sprintf(weather_data.name,"%s",item->valuestring);
                    }
                }
                /****************以下是天气信息********************/

                /**天气**/
                if((arrayItem = cJSON_GetObjectItem(object,"daily")) != NULL)
                {
                    size = cJSON_GetArraySize(arrayItem);     //获取数组中对象个数
                    for(int i = 0; i < size; i ++) //一个循环获取三天的信息
                    {
                        if((object = cJSON_GetArrayItem(arrayItem,i)) != NULL)
                        {
                            if((item = cJSON_GetObjectItem(object,"date")) != NULL)
                            {
                                printf("---------%s---------\r\n",item->valuestring);	 //显示时间
                                sprintf(weather_data.date[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"text_day")) != NULL)
                            {
                                printf("白天天气:%s\r\n",item->valuestring);
                                sprintf(weather_data.text_day[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"code_day")) != NULL)
                            {
                                sprintf(weather_data.code_day[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"text_night")) != NULL)
                            {
                                printf("夜间天气:%s\r\n",item->valuestring);
                                sprintf(weather_data.text_night[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"code_night")) != NULL)
                            {
                                sprintf(weather_data.code_night[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"high")) != NULL)
                            {
                                printf( "最高气温:%s摄氏度 \r\n",item->valuestring);
                                sprintf(weather_data.high[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"low")) != NULL)
                            {
                                printf( "最低气温:%s摄氏度 \r\n",item->valuestring);
                                sprintf(weather_data.low[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"humidity")) != NULL)
                            {
                                printf("湿度:%s%% \r\n",item->valuestring);
															sprintf(weather_data.humidity[i],"%s",item->valuestring);
                            }
														if((item = cJSON_GetObjectItem(object,"wind_scale")) != NULL)
                            {
                                printf("风力:%s%% \r\n",item->valuestring);
															sprintf(weather_data.wind_scale[i],"%s",item->valuestring);
                            }
                        }
                        else
                        {
                            printf("object");
                        }

                    }
                }
                else
                {
                    printf("arrayItem");
                }
            }
        }
        else
        {
            printf("NULL\r\n");
        }
    }
    cJSON_Delete(root); //释放cJSON_Parse()分配出来的内存空间
    return 0;
}
void GetWeather()
{
    fopen("/sdcard/city.txt", "wt+");
    FILE            *fp = NULL;
    char            line[2048] = {0};
    cJSON *root = RT_NULL, *item = RT_NULL;
    webclient_get_file("http://tianqiapi.com/api?version=v6&appid=79931957&appsecret=dBq4JGRZ","/sdcard/city.txt");
    if(NULL != (fp = fopen("/sdcard/city.txt", "r")))
    {
        while (NULL != fgets(line, sizeof(line), fp))
        {
            root = cJSON_Parse(line); //获取整个大的句柄
            if (!root)
            {
                rt_kprintf("No memory for cJSON root!\n");
                return;
            }
            item =	cJSON_GetObjectItem(root, "cityEn");
            rt_kprintf("\nCITY:%s ",item->valuestring);
            if (root != RT_NULL)
                cJSON_Delete(root);
        }
        fclose(fp);
    }		
		FILE*WEA=fopen("/sdcard/weather.txt", "wt+");
    char *weather_url = RT_NULL;
    weather_url = rt_calloc(1, GET_URL_LEN_MAX);
    if (weather_url == RT_NULL)
    {
        rt_kprintf("No memory for weather_url!\n");
    }
    /* 拼接 GET 网址 */
    rt_snprintf(weather_url, GET_URL_LEN_MAX, GET_URI, item->valuestring);
    rt_kprintf("%s",weather_url);
    FILE            *fp1 = NULL;
    char            line1[1024] = {0};
    webclient_get_file(weather_url,"/sdcard/weather.txt");
    if(NULL != (fp1= fopen("/sdcard/weather.txt", "r")))
    {
        while (NULL != fgets(line1, sizeof(line1), fp1))
        {
            weather_data_parse(line1); //获取整个大的句柄
        }
    fclose(fp1);
    }
    weatherUpdatFlag = 1;
    if (weather_url != RT_NULL)
     rt_free(weather_url);
}
MSH_CMD_EXPORT(GetWeather, Update Weather.);
