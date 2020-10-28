#ifndef __GUI_MainTask_H
#define __GUI_MainTask_H
#include <rtthread.h>

#ifndef EEP_I2CBUS_NAME

#define EEP_I2CBUS_NAME          "i2c1"  /*连接的i2c总线设备名称 */

#endif

#define EEP_ADDR             0x50  //从设备EEPROM地址

static struct rt_i2c_bus_device *eep_i2c_bus = RT_NULL;

typedef struct
{
    rt_uint8_t top1;
    rt_uint8_t bottom1;
    rt_uint8_t right1;
    rt_uint8_t left1;

    rt_uint8_t buf[4];

    rt_uint8_t top2;
    rt_uint8_t bottom2;
    rt_uint8_t right2;
    rt_uint8_t left2;

}TOUCH_T;

rt_err_t eeprom_iic_write(rt_uint8_t write_addr, rt_uint8_t data, rt_uint32_t number);

rt_err_t eeprom_iic_read(rt_uint8_t read_addr, rt_uint32_t len, rt_uint8_t *buf);

static void TOUCH_DispPoint(rt_uint8_t _ucIndex);
void Touch_MainTask(void);
void Task_eep(void);
void Task_eep1(void);

#endif

