#include <setjmp.h>
#include <stdio.h>

//#define DEBUG
#include "debug.h"

#include "coroutine.h"

enum State
{
    Ready,
    Running,
    Terminated,
};

enum
{
    Start = 0,
    Terminat,
};

// Routine contorl block
struct RCB
{
    jmp_buf context;
    method entry;
    enum State state;
};

static jmp_buf Matrix_contex;
static struct RCB Routines[MAX_ROUTINE_CNT];
static int Routine_cnt = 0;
static int Current = 0;

static void set_stack(struct RCB *rcb, int height)
{
    volatile char _padding[STACK_SIZE];
    if (height > 0)
    {
        set_stack(rcb, height - 1);
        return;
    }
    dbg("Routine %d: %p - %p\n", Routine_cnt, &_padding[0], &_padding[STACK_SIZE - 1]);
    int state = setjmp(rcb->context);
    if (0 != state)
    {
        struct RCB *curr = &Routines[Current];
        method entry = curr->entry;
        if (entry)
        {
            entry();
            longjmp(Matrix_contex, Terminated);
        }
    }
}

static void new_routine(struct RCB *rcb, method func)
{
    rcb->entry = func;
    rcb->state = Ready;
}

static int next_ready(int curr)
{
    int i = (curr + 1) % Routine_cnt;
    while (i != curr)
    {
        if (Ready == Routines[i].state)
        {
            return i;
        }
        i = (i + 1) % Routine_cnt;
    }
    return -1;
}

static void start_currnet_routine()
{
    struct RCB *curr = &Routines[Current];
    curr->state = Running;
    dbg("Start %d\n", Current);
    longjmp(curr->context, 1);
}

static void terminate_current_routine()
{
    struct RCB *curr = &Routines[Current];
    curr = &Routines[Current];
    curr->state = Terminated;
}

static void start_next_routine()
{
    int next = next_ready(Current);
    if (0 > next)
    {
        return;
    }
    Current = next;
    start_currnet_routine();
}

static void matrix()
{
    int state = setjmp(Matrix_contex);
    switch (state)
    {
    case Start:
        start_currnet_routine();
        break;
    case Terminated:
        dbg("Routine %d has terminated\n", Current);
        terminate_current_routine();
        start_next_routine();
        dbg("All routine terminated\n");
        break;
    }
}

void spawn(method func)
{
    struct RCB *curr = &Routines[Routine_cnt];
    new_routine(curr, func);
    set_stack(curr, Routine_cnt);
    Routine_cnt += 1;
}

void yield()
{
    struct RCB *curr = &Routines[Current];
    curr->state = Ready;
    int state = setjmp(curr->context);
    if (0 == state)
    {
        start_next_routine();
    }
}

void start()
{
    Current = 0;
    if (Current < Routine_cnt)
    {
        matrix();
    }
}