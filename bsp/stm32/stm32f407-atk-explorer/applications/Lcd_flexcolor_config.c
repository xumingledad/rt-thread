#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "Touch_Calibration.h"

TOUCH_T eeprom_1;

#define TOUCH_AD_TOP        406//���´������Ķ�����д�� Y ��ģ������ֵ��
#define TOUCH_AD_BOTTOM  3568   //���´������ĵײ���д�� Y ��ģ������ֵ��
#define TOUCH_AD_LEFT       3552    //���´���������࣬д�� X ��ģ������ֵ��
#define TOUCH_AD_RIGHT      308//���´��������Ҳ࣬д�� X ��ģ������ֵ��


rt_device_t lcddevice = RT_NULL;


/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/

void Task_eep3(void){
    eeprom_iic_read(0x00, 1, &eeprom_1.buf[0]); //top
    eeprom_iic_read(0x01, 1, &eeprom_1.buf[1]);  //bottom
    eeprom_iic_read(0x10, 1, &eeprom_1.buf[2]);  //left
    eeprom_iic_read(0x11, 1, &eeprom_1.buf[3]);  //right
}

void LCD_X_Config_override(void)
{
    if(!lcddevice)
    {
        lcddevice = rt_device_find("lcd");
        if (!lcddevice)
            LOG_E("find %s failed!", "lcd");
        rt_device_open(lcddevice, RT_DEVICE_FLAG_RDWR);
    }
    
    struct rt_device_graphic_info info;
    rt_device_control(lcddevice, RTGRAPHIC_CTRL_GET_INFO, &info);
    
    const LCD_API_COLOR_CONV *color;
    switch(info.pixel_format)
    {
        case RTGRAPHIC_PIXEL_FORMAT_MONO:
            color = GUICC_1;
            break;
        case RTGRAPHIC_PIXEL_FORMAT_RGB565:
            color = GUICC_M565;
            break;
        case RTGRAPHIC_PIXEL_FORMAT_RGB888:
            color = GUICC_M888;
            break;
        default:
            color = GUICC_M565;
            break;
    }

    Task_eep3();//��ȡEEPRON�е�У׼���ݣ�Ȼ�������غ����У��������ô�������

    GUI_DEVICE_CreateAndLink(&GUIDRV_Template_API, color, 0, 0);
    //
    // Display driver configuration, required for Lin-driver
    //
    LCD_SetSizeEx(0, info.width, info.height); //LCD�ĳߴ�
    LCD_SetVSizeEx(0, info.width, info.height);

    GUI_TOUCH_SetOrientation(GUI_SWAP_XY | GUI_MIRROR_Y);
    GUI_TOUCH_Calibrate(GUI_COORD_X, 0, 320, eeprom_1.buf[3]*2 ,eeprom_1.buf[2]*16  );//����ȡ�������õ����������
    GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, 240, eeprom_1.buf[0]*2 ,eeprom_1.buf[1]*16 );
}