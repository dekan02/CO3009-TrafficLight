#include "global.h"

typedef enum {
    AUTO_R1_GREEN,
    AUTO_R1_YELLOW,
    AUTO_R2_GREEN,
    AUTO_R2_YELLOW
} AutoState_t;

static AutoState_t state;

static void enter_state(AutoState_t next)
{
    state = next;

    switch (state) {
    case AUTO_R1_GREEN:
        TL_Set(ROAD_1, LIGHT_GREEN);
        TL_Set(ROAD_2, LIGHT_RED);
        Timer_Set(TIMER_AUTO, SEC_TO_TICKS(g_time_green));
        break;
    case AUTO_R1_YELLOW:
        TL_Set(ROAD_1, LIGHT_YELLOW);
        TL_Set(ROAD_2, LIGHT_RED);
        Timer_Set(TIMER_AUTO, SEC_TO_TICKS(g_time_yellow));
        break;
    case AUTO_R2_GREEN:
        TL_Set(ROAD_1, LIGHT_RED);
        TL_Set(ROAD_2, LIGHT_GREEN);
        Timer_Set(TIMER_AUTO, SEC_TO_TICKS(g_time_green));
        break;
    case AUTO_R2_YELLOW:
        TL_Set(ROAD_1, LIGHT_RED);
        TL_Set(ROAD_2, LIGHT_YELLOW);
        Timer_Set(TIMER_AUTO, SEC_TO_TICKS(g_time_yellow));
        break;
    }
}

void fsm_auto_init(void)
{
    enter_state(AUTO_R1_GREEN);
}

void fsm_auto_run(void)
{
    if (!Timer_IsExpired(TIMER_AUTO)) {
        return;
    }

    switch (state) {
    case AUTO_R1_GREEN:
        enter_state(AUTO_R1_YELLOW);
        break;
    case AUTO_R1_YELLOW:
        enter_state(AUTO_R2_GREEN);
        break;
    case AUTO_R2_GREEN:
        enter_state(AUTO_R2_YELLOW);
        break;
    case AUTO_R2_YELLOW:
        enter_state(AUTO_R1_GREEN);
        break;
    }
}
