#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include <stdint.h>

#define TICK_MS 10
#define SCH_MAX_TASKS 16
#define SCH_ERROR 0xFFFFFFFF

#define BTN_COUNT 4
#define BTN_1 0
#define BTN_2 1
#define BTN_3 2
#define BTN_4 3
#define DEBOUNCE_TICKS 2
#define LONG_PRESS_TICKS 100

#define ROAD_1 0
#define ROAD_2 1

#define TIME_MIN 1
#define TIME_MAX 99

#define SEC_TO_TICKS(s) ((uint32_t)(s) * 1000 / TICK_MS)

#define TIMER_COUNT 4
#define TIMER_AUTO 0
#define TIMER_MANUAL 1
#define TIMER_TUNING 2
#define TIMER_LCD 3

typedef enum {
    MODE_AUTO = 0,
    MODE_MANUAL,
    MODE_TUNING
} SystemMode_t;

typedef enum {
    LIGHT_OFF = 0,
    LIGHT_RED,
    LIGHT_GREEN,
    LIGHT_YELLOW
} LightColor_t;

extern SystemMode_t g_mode;
extern uint8_t g_time_red;
extern uint8_t g_time_green;
extern uint8_t g_time_yellow;
extern float g_temp;
extern float g_humi;

void SCH_Init(void);
uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t delay, uint32_t period);
uint8_t SCH_Delete_Task(uint32_t taskId);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);

void Timer_Init(void);
void Timer_Set(uint8_t index, uint32_t ticks);
void Timer_Run(void);
uint8_t Timer_IsExpired(uint8_t index);

void TL_Init(void);
void TL_Set(uint8_t road, LightColor_t color);
void TL_AllOff(void);

void Button_Init(void);
void Button_Read(void);
uint8_t Button_IsPressed(uint8_t index);
uint8_t Button_IsHeld(uint8_t index);

uint8_t DHT20_Init(void);
uint8_t DHT20_Read(float *temp, float *humi);

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(const char *str);

void fsm_auto_init(void);
void fsm_auto_run(void);
void fsm_manual_init(void);
void fsm_manual_run(void);
void fsm_tuning_init(void);
void fsm_tuning_run(void);

#endif
