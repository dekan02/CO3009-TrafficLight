#include "global.h"
#include "main.h"

typedef struct {
    void (*pTask)(void);
    uint32_t delay;
    uint32_t period;
    int8_t next;
    uint8_t used;
} sTask;

static sTask tasks[SCH_MAX_TASKS];
static int8_t head = -1;

static void insert_task(int8_t id, uint32_t delay)
{
    int8_t prev = -1;
    int8_t cur = head;

    while (cur != -1 && tasks[cur].delay <= delay) {
        delay -= tasks[cur].delay;
        prev = cur;
        cur = tasks[cur].next;
    }

    tasks[id].delay = delay;
    tasks[id].next = cur;
    if (cur != -1) {
        tasks[cur].delay -= delay;
    }
    if (prev == -1) {
        head = id;
    } else {
        tasks[prev].next = id;
    }
}

void SCH_Init(void)
{
    __disable_irq();
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        tasks[i].pTask = 0;
        tasks[i].delay = 0;
        tasks[i].period = 0;
        tasks[i].next = -1;
        tasks[i].used = 0;
    }
    head = -1;
    __enable_irq();
}

uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t delay, uint32_t period)
{
    int8_t id = -1;

    __disable_irq();
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (!tasks[i].used) {
            id = i;
            break;
        }
    }
    if (id == -1) {
        __enable_irq();
        return SCH_ERROR;
    }

    tasks[id].pTask = pFunction;
    tasks[id].period = period;
    tasks[id].used = 1;
    insert_task(id, delay);
    __enable_irq();

    return id;
}

uint8_t SCH_Delete_Task(uint32_t taskId)
{
    int8_t prev = -1;
    int8_t cur;

    if (taskId >= SCH_MAX_TASKS || !tasks[taskId].used) {
        return 0;
    }

    __disable_irq();
    cur = head;
    while (cur != -1 && cur != (int8_t)taskId) {
        prev = cur;
        cur = tasks[cur].next;
    }

    if (cur != -1) {
        int8_t next = tasks[cur].next;
        if (next != -1) {
            tasks[next].delay += tasks[cur].delay;
        }
        if (prev == -1) {
            head = next;
        } else {
            tasks[prev].next = next;
        }
    }
    tasks[taskId].used = 0;
    __enable_irq();

    return 1;
}

void SCH_Update(void)
{
    if (head != -1 && tasks[head].delay > 0) {
        tasks[head].delay--;
    }
}

void SCH_Dispatch_Tasks(void)
{
    while (1) {
        int8_t id;
        void (*func)(void);

        __disable_irq();
        if (head == -1 || tasks[head].delay > 0) {
            __enable_irq();
            return;
        }
        id = head;
        head = tasks[id].next;
        tasks[id].next = -1;
        func = tasks[id].pTask;
        __enable_irq();

        func();

        __disable_irq();
        if (tasks[id].used) {
            if (tasks[id].period > 0) {
                insert_task(id, tasks[id].period);
            } else {
                tasks[id].used = 0;
            }
        }
        __enable_irq();
    }
}
