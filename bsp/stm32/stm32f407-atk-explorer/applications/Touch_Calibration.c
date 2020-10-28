#include "GUI.h"   
#include <rtthread.h>
#include <rtdevice.h>
#include  "Touch_Calibration.h"

#ifndef EEP_I2CBUS_NAME

#define EEP_I2CBUS_NAME          "i2c1"  /*连接的i2c总线设备名称 */

#endif

 
#define EEP_ADDR             0x50  //从设备EEPROM地址

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

/*写EEPROM
write_addr:写入地址
data:写入数据
number:写入字节*/
rt_err_t eeprom_iic_write(rt_uint8_t write_addr, rt_uint8_t data, rt_uint32_t number)
{
    eep_i2c_bus = rt_i2c_bus_device_find(EEP_I2CBUS_NAME);//查找I2C设备
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

//读EEPROM
//read_addr:开始读的地址
//number:
rt_err_t eeprom_iic_read(rt_uint8_t read_addr, rt_uint32_t len, rt_uint8_t *buf)
{   
    eep_i2c_bus = rt_i2c_bus_device_find(EEP_I2CBUS_NAME);//查找I2C设备
    rt_i2c_master_send(eep_i2c_bus, EEP_ADDR, RT_I2C_WR, &read_addr, 1);

    //读取数据到buf
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
		GUI_SetColor(GUI_WHITE);/* 擦除第1个点 */
		GUI_DrawCircle(160, 0, 6);
		GUI_SetColor(GUI_BLUE);
		GUI_DrawCircle(0,120, 6);
	}
	else if (_ucIndex == 2)
	{
		GUI_SetColor(GUI_WHITE);/* 擦除第2个点 */
		GUI_DrawCircle(0,120, 6);
		GUI_SetColor(GUI_BLUE);	
		GUI_DrawCircle(320, 120, 6);
	}
	else
	{
		GUI_SetColor(GUI_WHITE);/* 擦除第3个点 */
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
    GUI_CURSOR_Show();   //显示鼠标等指示箭头
    GUI_CURSOR_Select(&GUI_CursorCrossL);   //设置指示箭头的类型，默认为鼠标
    GUI_SetBkColor(GUI_WHITE);   
    GUI_SetColor(GUI_BLACK);   
    GUI_Clear();   
    GUI_DispString("Measurement of\nA/D converter values");   

    for(n=0;n<4;n++){
        TOUCH_DispPoint(n); //显示校准点
        usCount=0;
	 
    for(i=0;i<500;i++)
    {
        GUI_PID_STATE TouchState;   
        GUI_TOUCH_GetState(&TouchState);  /* 以像素为单位获取触摸位置*/   
        xPhys = GUI_TOUCH_GetxPhys();     /* 在X中得到A/D测量结果*/   
        yPhys = GUI_TOUCH_GetyPhys();     /* 在Y中得到A/D测量结� */ 
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
                if (n == 0&&yPhys < 475)  //读取第一个触摸校准点的数据
                {  		
                    top=yPhys;
                    rt_kprintf("x:%d\n",xPhys);
                    rt_kprintf("y:%d\n",top);
                }
                else if(n == 1&&xPhys > 3400) //读取第二个触摸校准点的数据
                {
                    left=xPhys;
                    rt_kprintf("x:%d\n",left);
                    rt_kprintf("y:%d\n",yPhys);
                }
                else if(n == 2&& xPhys<470)//读取第三个触摸校准点的数据
                {
                    right=xPhys;
                    rt_kprintf("x:%d\n",right);
                    rt_kprintf("y:%d\n",yPhys);
                }
                else if(n==3&& yPhys>3400) //读取第四个触摸校准点的数据
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

//将获取的触摸数据写入EEPROM中
void Task_eep(void)
{ 
    eep.top1=(top/2);       
    eep.bottom1=(bottom/16);
    eep.right1=(right/2);
    eep.left1=(left/16);

    rt_kprintf("data is: %d   %d \r\n",eep.bottom1,eep.right1);
    eep_i2c_bus = rt_i2c_bus_device_find(EEP_I2CBUS_NAME);//查找I2C设备
    rt_kprintf("EEP set i2c bus to %s\r\n", EEP_I2CBUS_NAME);

    eeprom_iic_write(0x00,eep.top1,sizeof(eep.top1));   //向地址0x00写入top的数据
    eeprom_iic_write(0x01,eep.bottom1,sizeof(eep.bottom1)); //向地址0x01写入bottom的数据
    eeprom_iic_write(0x10,eep.left1,sizeof(eep.left1));//向地址0x10写入left的数据
    eeprom_iic_write(0x11,eep.right1,sizeof(eep.right1));//向地址0x11写入right的数据
}

//读取EEPROM中的校准数据
void Task_eep1(void)
{ 
    int i;
    eep_i2c_bus = rt_i2c_bus_device_find(EEP_I2CBUS_NAME);//查找I2C设备
    rt_kprintf("EEP set i2c bus to %s\r\n", EEP_I2CBUS_NAME);

    eeprom_iic_read(0x00, 1, &eep.buf[0]);  //读取地址0x00的数据给buf数组
    eeprom_iic_read(0x01, 1, &eep.buf[1]);//读取地址0x01的数据给buf数组
    eeprom_iic_read(0x10, 1, &eep.buf[2]); //读取地址0x10的数据给buf数组
    eeprom_iic_read(0x11, 1, &eep.buf[3]);//读取地址0x11的数据给buf数组

    for(i=0;i<4;i++)
    {		
        rt_kprintf("EEP read ok ,data is: %d \r\n",eep.buf[i]);
    }
}

//重置EEPROM对应地址中的数据，置为0
void Test_eep(void)
{
    eeprom_iic_write(0x00,0,sizeof(eep.top1)); 
    eeprom_iic_write(0x01,0,sizeof(eep.bottom1));
    eeprom_iic_write(0x10,0,sizeof(eep.left1));
    eeprom_iic_write(0x11,0,sizeof(eep.right1));
}

MSH_CMD_EXPORT(Task_eep1, i2c_eeprom sample);
MSH_CMD_EXPORT(Test_eep, i2c_eeprom sample2);
