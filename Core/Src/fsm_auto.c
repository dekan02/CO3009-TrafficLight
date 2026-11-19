#include "global.h"

typedef enum {
    AUTO_R1_GREEN,
    AUTO_R1_YELLOW,
    AUTO_R2_GREEN,
    AUTO_R2_YELLOW
} AutoState_t;

static AutoState_t state;
static uint32_t ticks;

static uint32_t to_ticks(uint8_t seconds)
{
    return (uint32_t)seconds * 1000 / TICK_MS;
}

static void enter_state(AutoState_t next)
{
    state = next;
    ticks = 0;

    switch (state) {
    case AUTO_R1_GREEN:
        TL_Set(ROAD_1, LIGHT_GREEN);
        TL_Set(ROAD_2, LIGHT_RED);
        break;
    case AUTO_R1_YELLOW:
        TL_Set(ROAD_1, LIGHT_YELLOW);
        TL_Set(ROAD_2, LIGHT_RED);
        break;
    case AUTO_R2_GREEN:
        TL_Set(ROAD_1, LIGHT_RED);
        TL_Set(ROAD_2, LIGHT_GREEN);
        break;
    case AUTO_R2_YELLOW:
        TL_Set(ROAD_1, LIGHT_RED);
        TL_Set(ROAD_2, LIGHT_YELLOW);
        break;
    }
}

void fsm_auto_init(void)
{
    enter_state(AUTO_R1_GREEN);
}

void fsm_auto_run(void)
{
    ticks++;

    switch (state) {
    case AUTO_R1_GREEN:
        if (ticks >= to_ticks(g_time_green)) {
            enter_state(AUTO_R1_YELLOW);
        }
        break;
    case AUTO_R1_YELLOW:
        if (ticks >= to_ticks(g_time_yellow)) {
            enter_state(AUTO_R2_GREEN);
        }
        break;
    case AUTO_R2_GREEN:
        if (ticks >= to_ticks(g_time_green)) {
            enter_state(AUTO_R2_YELLOW);
        }
        break;
    case AUTO_R2_YELLOW:
        if (ticks >= to_ticks(g_time_yellow)) {
            enter_state(AUTO_R1_GREEN);
        }
        break;
    }
}
