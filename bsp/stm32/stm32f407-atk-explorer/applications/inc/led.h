#include <stdint.h>
#include <drv_gpio.h>
#include "string.h"
#define LED0_PIN    GET_PIN(E, 3)       /*  PE3  */
#define LED1_PIN    GET_PIN(E, 4)       /*  PE4 */
#define LED2_PIN    GET_PIN(G, 9)       /*  PG8 */
#define BEEP_PIN    GET_PIN(G, 7)

#define KEY0_PIN    GET_PIN(F, 6)        /* PF6~PF9*/
#define KEY1_PIN    GET_PIN(F, 7)  
#define KEY2_PIN    GET_PIN(F, 8)  
#define KEY3_PIN    GET_PIN(F, 9)

#define PIN_KEY0    GET_PIN(F, 6)        /* PF6~PF9*/
#define PIN_KEY1   GET_PIN(F, 7)  
#define PIN_KEY2    GET_PIN(F, 8)  
#define PIN_KEY3    GET_PIN(F, 9)
#define PIN_WK_UP    GET_PIN(A, 0)
#define KEY_ON	0
#define KEY_OFF	1

void LEDOn(uint8_t led_number);
void LEDOff(uint8_t led_number);
void LEDToggle(uint8_t led_number);
void BeepOn(void);
void BeepOff(void);
void BeepToggle(void);
void alarm_callback_entry(rt_alarm_t alarm, time_t timestamp);
void led_key_init(void);
uint8_t Key_Scan(uint8_t key_number);
uint8_t Read_KEY_Level(uint8_t key_number);
void alarm_add(int argc, char **argv);
/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
typedef void (*flex_button_response_callback)(void*);

typedef enum
{
    FLEX_BTN_PRESS_DOWN = 0,
    FLEX_BTN_PRESS_CLICK,
    FLEX_BTN_PRESS_DOUBLE_CLICK,
    FLEX_BTN_PRESS_SHORT_START,
    FLEX_BTN_PRESS_SHORT_UP,
    FLEX_BTN_PRESS_LONG_START,
    FLEX_BTN_PRESS_LONG_UP,
    FLEX_BTN_PRESS_LONG_HOLD,
    FLEX_BTN_PRESS_LONG_HOLD_UP,
    FLEX_BTN_PRESS_MAX,
    FLEX_BTN_PRESS_NONE,
} flex_button_event_t;

/**
 * flex_button_t
 * 
 * @brief Button data structure
 *        Below are members that need to user init before scan.
 * 
 * @member pressed_logic_level:    Logic level when the button is pressed.
 *                                 Must be inited by 'flex_button_register' API
 *                                                     before start button scan.
 * @member debounce_tick:          The time of button debounce.
 *                                 The value is number of button scan cycles.
 * @member click_start_tick:       The time of start click.
 *                                 The value is number of button scan cycles.
 * @member short_press_start_tick: The time of short press start tick.
 *                                 The value is number of button scan cycles.
 * @member long_press_start_tick:  The time of long press start tick.
 *                                 The value is number of button scan cycles.
 * @member long_hold_start_tick:   The time of hold press start tick.
 *                                 The value is number of button scan cycles.
 * @member usr_button_read:        Read the logic level value of specified button.
 * @member cb:                     Button event callback function.
 *                                 If use 'flex_button_event_read' api,
 *                                 you don't need to initialize the 'cb' member.
 * @member next :                  Next button struct
*/
typedef struct flex_button
{
    uint8_t pressed_logic_level : 1; /* need user to init */

    /**
     * @event
     * The event of button in flex_button_evnt_t enum list.
     * Automatically initialized to the default value FLEX_BTN_PRESS_NONE
     *                                      by 'flex_button_register' API.
    */
    uint8_t event               : 4;

    /**
     * @status
     * Used to record the status of the button 
     * Automatically initialized to the default value 0.
    */
    uint8_t status              : 3;
    uint16_t scan_cnt;  /* default 0. Used to record the number of key scans */
    uint16_t click_cnt; /* default 0. Used to record the number of key click */

    uint16_t debounce_tick;          
    uint16_t click_start_tick;
    uint16_t short_press_start_tick;
    uint16_t long_press_start_tick;
    uint16_t long_hold_start_tick;

    uint8_t  (*usr_button_read)(void);
    flex_button_response_callback  cb;
    struct flex_button* next;
} flex_button_t;

#ifdef __cplusplus
extern "C" {
#endif

int8_t flex_button_register(flex_button_t *button);
flex_button_event_t flex_button_event_read(flex_button_t* button);
void flex_button_scan(void);

#ifdef __cplusplus
}
#endif 
