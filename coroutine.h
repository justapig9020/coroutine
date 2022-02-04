#pragma once

#define MAX_ROUTINE_CNT 10
#define STACK_SIZE (4 * 1024)

typedef void (*method)();

void spawn(method func);

void yield();

void start();
