#include "main.h"
#include "global.h"

static GPIO_TypeDef *ports[BTN_COUNT] = {GPIOA, GPIOA, GPIOC, GPIOB};
static const uint16_t pins[BTN_COUNT] = {GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_7, GPIO_PIN_6};

static uint8_t last_raw[BTN_COUNT];
static uint8_t stable[BTN_COUNT];
static uint8_t same_count[BTN_COUNT];
static uint16_t hold_count[BTN_COUNT];
static uint8_t pressed_flag[BTN_COUNT];
static uint8_t held_flag[BTN_COUNT];

void Button_Init(void)
{
    for (int i = 0; i < BTN_COUNT; i++) {
        last_raw[i] = 0;
        stable[i] = 0;
        same_count[i] = 0;
        hold_count[i] = 0;
        pressed_flag[i] = 0;
        held_flag[i] = 0;
    }
}

void Button_Read(void)
{
    for (int i = 0; i < BTN_COUNT; i++) {
        uint8_t raw = (HAL_GPIO_ReadPin(ports[i], pins[i]) == GPIO_PIN_RESET);

        if (raw == last_raw[i]) {
            if (same_count[i] < DEBOUNCE_TICKS) {
                same_count[i]++;
            }
        } else {
            last_raw[i] = raw;
            same_count[i] = 0;
        }

        if (same_count[i] >= DEBOUNCE_TICKS && raw != stable[i]) {
            stable[i] = raw;
            hold_count[i] = 0;
            held_flag[i] = 0;
            if (raw) {
                pressed_flag[i] = 1;
            }
        }

        if (stable[i]) {
            if (hold_count[i] < LONG_PRESS_TICKS) {
                hold_count[i]++;
            }
            if (hold_count[i] >= LONG_PRESS_TICKS) {
                held_flag[i] = 1;
            }
        }
    }
}

uint8_t Button_IsPressed(uint8_t index)
{
    uint8_t result = pressed_flag[index];
    pressed_flag[index] = 0;
    return result;
}

uint8_t Button_IsHeld(uint8_t index)
{
    return held_flag[index];
}
