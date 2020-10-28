#ifndef __GUI_WEATHERUPDAT_H__
#define __GUI_WEATHERUPDAT_H__

#include <rthw.h>
#include <rtthread.h>
#include "stdio.h"

/* �������ݽṹ�� */
/* �������ݽṹ�� */
typedef struct
{
	/* ʵ���������� */
	char name[32];	
  char city[32]; 	//����
	char text[32];				//����Ԥ������
	char code[32];				//����Ԥ������
	char temperature[32];   	//����
	
	/* ���졢���졢������������ */
	char date[3][32];			//����
	char text_day[3][64];	    //����������������
	char code_day[3][32];		//���������������
	char text_night[3][32];		//���������������
	char code_night[3][64]; 	//��������������
	char high[3][32];			//�����
	char low[3][32];		//�����
	char wind_direction[3][64]; //����
	char wind_speed[3][32];  	//���٣���λkm/h����unit=cʱ��
	char wind_scale[3][32];  	//�����ȼ�
	char humidity[3][32];//ʪ��
	char life_suggestion[128][2];	//�����
}WeatherType;

extern WeatherType weather_data;

void GetWeather(void);
#endif
