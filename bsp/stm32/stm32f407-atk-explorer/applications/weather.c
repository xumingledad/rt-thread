#include "GUI_weatherUpdata.h"
#include <webclient.h>  /* ʹ�� HTTP Э���������ͨ����Ҫ������ͷ�ļ� */
#include <sys/socket.h> /* ʹ��BSD socket����Ҫ����socket.hͷ�ļ� */
#include <netdb.h>
#include <cJSON.h>
#include <finsh.h>
#include <dfs_posix.h>
#pragma diag_suppress 870
/* ��֪������api.seniverse.com��--�������ݵ���Դ */
#define WEATHER_IP 		"116.62.81.138"
#define WEATHER_PORT 	"80"
#define GET_URL_LEN_MAX         256         //��ַ��󳤶�
#define GET_URI                 "http://api.seniverse.com/v3/weather/daily.json?key=npk2xtj6qv1ayzcg&location=%s&language=en" //��ȡ������ API]
/* ����һЩȫ�ֱ��� */
//------------------------------------------------------------------------------------------
//
//����Ҫ����������,������Ӧ��ʹ��
WeatherType weather_data;
char weatherUpdatFlag = 0;

/* �������ݽ��� */
static int weather_data_parse(char *data)
{
    cJSON *root, *arrayItem,*object,*subobject,*item;
    //char buffer[50];
    root =  cJSON_Parse(data);//����JSON���ݰ�
    if(root == NULL)
    {
        printf("���ݽ�������.����:[%s] \r\n",cJSON_GetErrorPtr() );
        return 1;
    }
    else
    {
        if((arrayItem = cJSON_GetObjectItem(root,"results")) != NULL) //ƥ���ַ���"results",��ȡ��������
        {
            int size = cJSON_GetArraySize(arrayItem);     //��ȡ�����ж������
            if((object = cJSON_GetArrayItem(arrayItem,0)) != NULL)
            {
                printf("********��ѯ����*******\r\n");
                if((subobject = cJSON_GetObjectItem(object,"location")) != NULL) //��ѯ�ص�
                {
                    if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) //��ѯ����
                    {
                        printf("����:%s  \r\n",item->valuestring); 
                        sprintf(weather_data.name,"%s",item->valuestring);
                    }
                }
                /****************������������Ϣ********************/

                /**����**/
                if((arrayItem = cJSON_GetObjectItem(object,"daily")) != NULL)
                {
                    size = cJSON_GetArraySize(arrayItem);     //��ȡ�����ж������
                    for(int i = 0; i < size; i ++) //һ��ѭ����ȡ�������Ϣ
                    {
                        if((object = cJSON_GetArrayItem(arrayItem,i)) != NULL)
                        {
                            if((item = cJSON_GetObjectItem(object,"date")) != NULL)
                            {
                                printf("---------%s---------\r\n",item->valuestring);	 //��ʾʱ��
                                sprintf(weather_data.date[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"text_day")) != NULL)
                            {
                                printf("��������:%s\r\n",item->valuestring);
                                sprintf(weather_data.text_day[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"code_day")) != NULL)
                            {
                                sprintf(weather_data.code_day[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"text_night")) != NULL)
                            {
                                printf("ҹ������:%s\r\n",item->valuestring);
                                sprintf(weather_data.text_night[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"code_night")) != NULL)
                            {
                                sprintf(weather_data.code_night[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"high")) != NULL)
                            {
                                printf( "�������:%s���϶� \r\n",item->valuestring);
                                sprintf(weather_data.high[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"low")) != NULL)
                            {
                                printf( "�������:%s���϶� \r\n",item->valuestring);
                                sprintf(weather_data.low[i],"%s",item->valuestring);
                            }
                            if((item = cJSON_GetObjectItem(object,"humidity")) != NULL)
                            {
                                printf("ʪ��:%s%% \r\n",item->valuestring);
															sprintf(weather_data.humidity[i],"%s",item->valuestring);
                            }
														if((item = cJSON_GetObjectItem(object,"wind_scale")) != NULL)
                            {
                                printf("����:%s%% \r\n",item->valuestring);
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
    cJSON_Delete(root); //�ͷ�cJSON_Parse()����������ڴ�ռ�
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
            root = cJSON_Parse(line); //��ȡ������ľ��
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
    /* ƴ�� GET ��ַ */
    rt_snprintf(weather_url, GET_URL_LEN_MAX, GET_URI, item->valuestring);
    rt_kprintf("%s",weather_url);
    FILE            *fp1 = NULL;
    char            line1[1024] = {0};
    webclient_get_file(weather_url,"/sdcard/weather.txt");
    if(NULL != (fp1= fopen("/sdcard/weather.txt", "r")))
    {
        while (NULL != fgets(line1, sizeof(line1), fp1))
        {
            weather_data_parse(line1); //��ȡ������ľ��
        }
    fclose(fp1);
    }
    weatherUpdatFlag = 1;
    if (weather_url != RT_NULL)
     rt_free(weather_url);
}
MSH_CMD_EXPORT(GetWeather, Update Weather.);
