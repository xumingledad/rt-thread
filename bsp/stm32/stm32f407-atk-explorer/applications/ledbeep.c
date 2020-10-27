#include <rthw.h>
#include <rtdevice.h>
#include <led.h>

const uint16_t leds[] = {LED0_PIN, LED1_PIN, LED2_PIN};
const uint8_t keys[] = {KEY0_PIN, KEY1_PIN, KEY2_PIN, KEY3_PIN};

void LEDOn(uint8_t led_number)
{
    rt_pin_write(leds[led_number], PIN_LOW);
}

void LEDOff(uint8_t led_number)
{
    rt_pin_write(leds[led_number], PIN_HIGH);
}

void LEDToggle(uint8_t led_number)
{
    //uint8_t on = ;
    //on = (~on) & 0x01;
    rt_pin_write(leds[led_number], (~rt_pin_read(leds[led_number])) & 0x01);
}

void BeepOn(void)
{
    rt_pin_write(BEEP_PIN, PIN_HIGH);
}

void BeepOff(void)
{
    rt_pin_write(BEEP_PIN, PIN_LOW);
}

void BeepToggle(void)
{
    rt_pin_write(BEEP_PIN, (~rt_pin_read(BEEP_PIN) & 0x01));
}

void led_key_init(void)
{
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(BEEP_PIN, PIN_MODE_OUTPUT);

    rt_pin_write(LED0_PIN, PIN_HIGH);
    rt_pin_write(LED1_PIN, PIN_HIGH);
    rt_pin_write(LED2_PIN, PIN_HIGH);
    rt_pin_write(BEEP_PIN, PIN_LOW);

//    rt_pin_mode(KEY0_PIN, PIN_MODE_INPUT_PULLUP);
//    rt_pin_mode(KEY1_PIN, PIN_MODE_INPUT_PULLUP);
//    rt_pin_mode(KEY2_PIN, PIN_MODE_INPUT_PULLUP);
//    rt_pin_mode(KEY3_PIN, PIN_MODE_INPUT_PULLUP);
}

uint8_t Read_KEY_Level(uint8_t key_number)
{
    uint8_t v = rt_pin_read(keys[key_number]);
    return v;
}

uint8_t Key_Scan(uint8_t key_number)
{
    if(Read_KEY_Level(key_number) == KEY_ON )
    {

        while(Read_KEY_Level(key_number) == KEY_ON);
        return 	KEY_ON;
    }
    else
        return KEY_OFF;
}


INIT_DEVICE_EXPORT(led_key_init);
