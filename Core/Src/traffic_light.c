#include "main.h"
#include "global.h"

static void write_pins(uint8_t road, GPIO_PinState a, GPIO_PinState b)
{
    if (road == ROAD_1) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, a);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, b);
    } else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, a);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, b);
    }
}

void TL_Init(void)
{
    TL_AllOff();
}

void TL_Set(uint8_t road, LightColor_t color)
{
    switch (color) {
    case LIGHT_RED:
        write_pins(road, GPIO_PIN_RESET, GPIO_PIN_SET);
        break;
    case LIGHT_GREEN:
        write_pins(road, GPIO_PIN_SET, GPIO_PIN_RESET);
        break;
    case LIGHT_YELLOW:
        write_pins(road, GPIO_PIN_SET, GPIO_PIN_SET);
        break;
    default:
        write_pins(road, GPIO_PIN_RESET, GPIO_PIN_RESET);
        break;
    }
}

void TL_AllOff(void)
{
    TL_Set(ROAD_1, LIGHT_OFF);
    TL_Set(ROAD_2, LIGHT_OFF);
}
