#include "coroutine.h"
#include <stdio.h>

void func1() {
    printf("%s 1\n", __func__);
    yield();
    printf("%s 2\n", __func__);
}

void func2() {
    printf("%s 1\n", __func__);
    yield();
    printf("%s 2\n", __func__);
}

int main(void) {
    spawn(func1);
    spawn(func2);
    start();
    return 0;
}
