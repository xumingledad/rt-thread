#ifndef __MAINTASK_H
#define __MAINTASK_H
#include <rtthread.h>
#include <rtdevice.h>
#include "DIALOG.h"
void App_SetTimeAlarm(void);
void GuiMainTask(void);
void App_SetDate(void);
void Showpng(const char * sFilename) ;
int dispbmp(uint8_t *BMPFileName,uint32_t x,uint32_t y);
void App_SetTimeAlarm(void);
void Show_IMAGE(IMAGE_Handle hWin,const char *sFilename);

#endif
