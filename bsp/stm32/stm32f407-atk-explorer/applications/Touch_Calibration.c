#include "GUI.h"   
#include <rtthread.h>
#include <rtdevice.h>
#include  "Touch_Calibration.h"

#ifndef EEP_I2CBUS_NAME

#define EEP_I2CBUS_NAME          "i2c1"  /*Á¬½ÓµÄi2c×ÜÏßÉè±¸Ãû³Æ */

#endif

 
#define EEP_ADDR             0x50  //´ÓÉè±¸EEPROMµØÖ·

TOUCH_T eep;
/*********************************************************************  
*  
*       Public code  
*  
**********************************************************************  
*/   
/*********************************************************************  
*  
*     
*/   
rt_uint16_t top;
rt_uint16_t bottom;
rt_uint16_t right;
rt_uint16_t left;

//static struct rt_i2c_bus_device *eep_i2c_bus = RT_NULL;

/*Ð´EEPROM
write_addr:Ð´ÈëµØÖ·
data:Ð´ÈëÊý¾Ý
number:Ð´Èë×Ö½Ú*/
rt_err_t eeprom_iic_write(rt_uint8_t write_addr, rt_uint8_t data, rt_uint32_t number)
{
    eep_i2c_bus = rt_i2c_bus_device_find(EEP_I2CBUS_NAME);//²éÕÒI2CÉè±¸
    rt_uint8_t buf[2];

    buf[0] = write_addr;
    buf[1] = data;

    rt_size_t result;
    result = rt_i2c_master_send(eep_i2c_bus, EEP_ADDR, RT_I2C_WR, buf, 2);
    rt_thread_mdelay(10);  
    if (result == 2){
        rt_kprintf("EEP write ok \r\n");
        return RT_EOK;       
    }
    else
    {
        rt_kprintf("EEP write failed ,ERR is: %d \r\n",result);
        return -RT_ERROR;       
    }
}

//¶ÁEEPROM
//read_addr:¿ªÊ¼¶ÁµÄµØÖ·
//number:
rt_err_t eeprom_iic_read(rt_uint8_t read_addr, rt_uint32_t len, rt_uint8_t *buf)
{   
    eep_i2c_bus = rt_i2c_bus_device_find(EEP_I2CBUS_NAME);//²éÕÒI2CÉè±¸
    rt_i2c_master_send(eep_i2c_bus, EEP_ADDR, RT_I2C_WR, &read_addr, 1);

    //¶ÁÈ¡Êý¾Ýµ½buf
    rt_i2c_master_recv(eep_i2c_bus, EEP_ADDR, RT_I2C_RD, buf, len);
    return RT_EOK;
}

static void TOUCH_DispPoint(rt_uint8_t _ucIndex)
{
	if (_ucIndex == 0)
	{
		GUI_DrawCircle(160, 0, 6);
	}
	else if (_ucIndex == 1)
	{
		GUI_SetColor(GUI_WHITE);/* ²Á³ýµÚ1¸öµã */
		GUI_DrawCircle(160, 0, 6);
		GUI_SetColor(GUI_BLUE);
		GUI_DrawCircle(0,120, 6);
	}
	else if (_ucIndex == 2)
	{
		GUI_SetColor(GUI_WHITE);/* ²Á³ýµÚ2¸öµã */
		GUI_DrawCircle(0,120, 6);
		GUI_SetColor(GUI_BLUE);	
		GUI_DrawCircle(320, 120, 6);
	}
	else
	{
		GUI_SetColor(GUI_WHITE);/* ²Á³ýµÚ3¸öµã */
		GUI_DrawCircle(320, 120, 6);
		GUI_SetColor(GUI_BLUE);
		GUI_DrawCircle(160, 240, 6);
	}
}

void Touch_MainTask(void) {   
    rt_uint32_t i; 
    rt_uint16_t xPhys, yPhys;
    rt_uint16_t n;
    rt_uint8_t usCount;

    GUI_Init();   
    GUI_CURSOR_Show();   //ÏÔÊ¾Êó±êµÈÖ¸Ê¾¼ýÍ·
    GUI_CURSOR_Select(&GUI_CursorCrossL);   //ÉèÖÃÖ¸Ê¾¼ýÍ·µÄÀàÐÍ£¬Ä¬ÈÏÎªÊó±ê
    GUI_SetBkColor(GUI_WHITE);   
    GUI_SetColor(GUI_BLACK);   
    GUI_Clear();   
    GUI_DispString("Measurement of\nA/D converter values");   

    for(n=0;n<4;n++){
        TOUCH_DispPoint(n); //ÏÔÊ¾Ð£×¼µã
        usCount=0;
	 
    for(i=0;i<500;i++)
    {
        GUI_PID_STATE TouchState;   
        GUI_TOUCH_GetState(&TouchState);  /* ÒÔÏñËØÎªµ¥Î»»ñÈ¡´¥ÃþÎ»ÖÃ*/   
        xPhys = GUI_TOUCH_GetxPhys();     /* ÔÚXÖÐµÃµ½A/D²âÁ¿½á¹û*/   
        yPhys = GUI_TOUCH_GetyPhys();     /* ÔÚYÖÐµÃµ½A/D²âÁ¿½á¹ */ 
        GUI_SetColor(GUI_BLUE);   
        GUI_DispStringAt("Analog input:\n", 0, 20);   
        GUI_GotoY(GUI_GetDispPosY() + 2);   
        GUI_DispString("x:");   
        GUI_DispDec(xPhys, 4);  		
        GUI_DispString(", y:");   
        GUI_DispDec(yPhys, 4); 		
        GUI_SetColor(GUI_RED);   
        GUI_GotoY(GUI_GetDispPosY() + 4);   
        GUI_DispString("\nPosition:\n");   
        GUI_GotoY(GUI_GetDispPosY() + 2);   
        GUI_DispString("x:");   
        GUI_DispDec(TouchState.x,4);   
        GUI_DispString(", y:");   
        GUI_DispDec(TouchState.y,4);   
        /* Wait a while */   
        GUI_Delay(100);  
		
        if(xPhys<4095 && yPhys>0)
        {
            if (++usCount > 5)
            {
                if (n == 0&&yPhys < 475)  //¶ÁÈ¡µÚÒ»¸ö´¥ÃþÐ£×¼µãµÄÊý¾Ý
                {  		
                    top=yPhys;
                    rt_kprintf("x:%d\n",xPhys);
                    rt_kprintf("y:%d\n",top);
                }
                else if(n == 1&&xPhys > 3400) //¶ÁÈ¡µÚ¶þ¸ö´¥ÃþÐ£×¼µãµÄÊý¾Ý
                {
                    left=xPhys;
                    rt_kprintf("x:%d\n",left);
                    rt_kprintf("y:%d\n",yPhys);
                }
                else if(n == 2&& xPhys<470)//¶ÁÈ¡µÚÈý¸ö´¥ÃþÐ£×¼µãµÄÊý¾Ý
                {
                    right=xPhys;
                    rt_kprintf("x:%d\n",right);
                    rt_kprintf("y:%d\n",yPhys);
                }
                else if(n==3&& yPhys>3400) //¶ÁÈ¡µÚËÄ¸ö´¥ÃþÐ£×¼µãµÄÊý¾Ý
                { 
                    bottom=yPhys;
                    rt_kprintf("x:%d\n",xPhys);
                    rt_kprintf("y:%d\n",bottom);
                }
                break;
            }	
        }				
    }		
    if (n == 500)
    {
        return;
    }
    }   
    GUI_Clear();
    GUI_CURSOR_Hide();
    Task_eep();
} 

//½«»ñÈ¡µÄ´¥ÃþÊý¾ÝÐ´ÈëEEPROMÖÐ
void Task_eep(void)
{ 
    eep.top1=(top/2);       
    eep.bottom1=(bottom/16);
    eep.right1=(right/2);
    eep.left1=(left/16);

    rt_kprintf("data is: %d   %d \r\n",eep.bottom1,eep.right1);
    eep_i2c_bus = rt_i2c_bus_device_find(EEP_I2CBUS_NAME);//²éÕÒI2CÉè±¸
    rt_kprintf("EEP set i2c bus to %s\r\n", EEP_I2CBUS_NAME);

    eeprom_iic_write(0x00,eep.top1,sizeof(eep.top1));   //ÏòµØÖ·0x00Ð´ÈëtopµÄÊý¾Ý
    eeprom_iic_write(0x01,eep.bottom1,sizeof(eep.bottom1)); //ÏòµØÖ·0x01Ð´ÈëbottomµÄÊý¾Ý
    eeprom_iic_write(0x10,eep.left1,sizeof(eep.left1));//ÏòµØÖ·0x10Ð´ÈëleftµÄÊý¾Ý
    eeprom_iic_write(0x11,eep.right1,sizeof(eep.right1));//ÏòµØÖ·0x11Ð´ÈërightµÄÊý¾Ý
}

//¶ÁÈ¡EEPROMÖÐµÄÐ£×¼Êý¾Ý
void Task_eep1(void)
{ 
    int i;
    eep_i2c_bus = rt_i2c_bus_device_find(EEP_I2CBUS_NAME);//²éÕÒI2CÉè±¸
    rt_kprintf("EEP set i2c bus to %s\r\n", EEP_I2CBUS_NAME);

    eeprom_iic_read(0x00, 1, &eep.buf[0]);  //¶ÁÈ¡µØÖ·0x00µÄÊý¾Ý¸øbufÊý×é
    eeprom_iic_read(0x01, 1, &eep.buf[1]);//¶ÁÈ¡µØÖ·0x01µÄÊý¾Ý¸øbufÊý×é
    eeprom_iic_read(0x10, 1, &eep.buf[2]); //¶ÁÈ¡µØÖ·0x10µÄÊý¾Ý¸øbufÊý×é
    eeprom_iic_read(0x11, 1, &eep.buf[3]);//¶ÁÈ¡µØÖ·0x11µÄÊý¾Ý¸øbufÊý×é

    for(i=0;i<4;i++)
    {		
        rt_kprintf("EEP read ok ,data is: %d \r\n",eep.buf[i]);
    }
}

//ÖØÖÃEEPROM¶ÔÓ¦µØÖ·ÖÐµÄÊý¾Ý£¬ÖÃÎª0
void Test_eep(void)
{
    eeprom_iic_write(0x00,0,sizeof(eep.top1)); 
    eeprom_iic_write(0x01,0,sizeof(eep.bottom1));
    eeprom_iic_write(0x10,0,sizeof(eep.left1));
    eeprom_iic_write(0x11,0,sizeof(eep.right1));
}

MSH_CMD_EXPORT(Task_eep1, i2c_eeprom sample);
MSH_CMD_EXPORT(Test_eep, i2c_eeprom sample2);
