#include <webclient.h>  /* 使用 HTTP 协议与服务器通信需要包含此头文件 */
#include <sys/socket.h> /* 使用BSD socket，需要包含socket.h头文件 */
#include <netdb.h>
#include <cJSON.h>
#include <finsh.h>
#include <dfs_posix.h>

#define GET_HEADER_BUFSZ        1024        //头部大小
#define GET_RESP_BUFSZ          1024*2        //响应缓冲区大小
#define GET_URL_LEN_MAX         256         //网址最大长度
#define GET_URI               "http:www.tianqiapi.com/free/day?appid=79931957&appsecret=dBq4JGRZ" //获取天气的 API
/* 天气数据解析 */
 void forecast_parse(char * data)//无符号  天气预报
{
   cJSON *root = RT_NULL, *object = RT_NULL, *list = RT_NULL, *item = RT_NULL, *weatheritem = RT_NULL;
   int index, list_size = 0;
   root = cJSON_Parse(data);//恒定的const
   if (!root)
   {
       rt_kprintf("No memory for cJSON root!\n");
       return;
   }
   rt_kprintf("\n\t七天天气预报");
   list = cJSON_GetObjectItem(root, "data");
   list_size = cJSON_GetArraySize(list);
   for(index=0;index<list_size;index++)
   {
           weatheritem = cJSON_GetArrayItem(list, index);   
           item = cJSON_GetObjectItem(weatheritem, "date");       rt_kprintf("\n日期    :%s ",item->valuestring);
		       item =	cJSON_GetObjectItem(weatheritem, "tem_day");    rt_kprintf("\n最高温度:%s ",item->valuestring);
           item =	cJSON_GetObjectItem(weatheritem, "tem_night");  rt_kprintf("\n最低温度:%s ",item->valuestring);
           item =	cJSON_GetObjectItem(weatheritem, "win");        rt_kprintf("\n风向    :%s ",item->valuestring);
           item =	cJSON_GetObjectItem(weatheritem, "win_speed");  rt_kprintf("\n风力    :%s ",item->valuestring);
           rt_kprintf("\n");
   }   
        rt_kprintf("\n");
__EXIT:
   if (root != RT_NULL)
       cJSON_Delete(root);        
}

void weather_parse(char *data)//解析的是实时天气
{  
	cJSON *root = RT_NULL, *object = RT_NULL, *item = RT_NULL,*weatheritem=RT_NULL;int index;
  root = cJSON_Parse(data);
	cJSON           *json;
    if (!root)
    {
        rt_kprintf("No memory for cJSON root!\n");
        return;
    }
		rt_kprintf("\n\t今日实时天气");
		item =	cJSON_GetObjectItem(root, "city");       rt_kprintf("\n所在城市:%s ",item->valuestring);
    item =	cJSON_GetObjectItem(root, "tem");        rt_kprintf("\n实时温度:%s ",item->valuestring);		
    item =	cJSON_GetObjectItem(root, "tem_day");    rt_kprintf("\n最高温度:%s ",item->valuestring);
    item =	cJSON_GetObjectItem(root, "tem_night");  rt_kprintf("\n最低温度:%s ",item->valuestring);
    item =	cJSON_GetObjectItem(root, "win");        rt_kprintf("\n风向    :%s ",item->valuestring);
    item =	cJSON_GetObjectItem(root, "win_speed");  rt_kprintf("\n风力    :%s ",item->valuestring);
    item =	cJSON_GetObjectItem(root, "win_meter");  rt_kprintf("\n风速    :%s ",item->valuestring);
    item =	cJSON_GetObjectItem(root, "air");        rt_kprintf("\n湿度    :%s ",item->valuestring);
    item =	cJSON_GetObjectItem(root, "update_time");rt_kprintf("\n更新时间:%s ",item->valuestring);
		rt_kprintf("\n");
    if (root != RT_NULL)
     cJSON_Delete(root);	
}

static void forecast(void)
{
	unlink("/sdcard/forecast.txt");
	FILE*WEA=fopen("/sdcard/forecast.txt", "w");
	fclose(WEA);
	FILE            *fp = NULL;
	cJSON           *json;	
	char            *out;    
  char            line[2048] = {0};
  int ret =0;
	ret=webclient_get_file("http://www.tianqiapi.com/free/week?appid=79931957&appsecret=dBq4JGRZ","/sdcard/forecast.txt");
    if(NULL != (fp = fopen("/sdcard/forecast.txt", "r"))) 
			{
        while (NULL != fgets(line, sizeof(line), fp))
					{
            forecast_parse(line); //获取整个大的句柄
					}
    fclose(fp);
			}	
}
static void weather(void)
{
	unlink("/sdcard/weather.txt");
	FILE*WEA=fopen("/sdcard/weather.txt", "w");
	fclose(WEA);
	FILE            *fp = NULL;
	cJSON           *json;	
	char            *out;    
  char            line[1024] = {0};
  int ret =0;
	ret=webclient_get_file("http://www.tianqiapi.com/free/day?appid=79931957&appsecret=dBq4JGRZ","/sdcard/weather.txt");
    if(NULL != (fp = fopen("/sdcard/weather.txt", "r"))) 
			{
        while (NULL != fgets(line, sizeof(line), fp))
					{
            weather_parse(line); //获取整个大的句柄
					}
    fclose(fp);
			}	
		forecast();
}
MSH_CMD_EXPORT(weather, Get weather by webclient);
