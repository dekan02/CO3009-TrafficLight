#include "global.h"

static uint32_t counter[TIMER_COUNT];
static uint8_t expired[TIMER_COUNT];

void Timer_Init(void)
{
    for (int i = 0; i < TIMER_COUNT; i++) {
        counter[i] = 0;
        expired[i] = 0;
    }
}

void Timer_Set(uint8_t index, uint32_t ticks)
{
    if (index >= TIMER_COUNT) {
        return;
    }
    counter[index] = ticks;
    expired[index] = (ticks == 0);
}

void Timer_Run(void)
{
    for (int i = 0; i < TIMER_COUNT; i++) {
        if (counter[i] > 0) {
            counter[i]--;
            if (counter[i] == 0) {
                expired[i] = 1;
            }
        }
    }
}

uint8_t Timer_IsExpired(uint8_t index)
{
    if (index >= TIMER_COUNT) {
        return 0;
    }
    return expired[index];
}
