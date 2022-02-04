#include "coroutine.h"
#include <stdio.h>

void func1() {
    for (int i=0; i<10; i++) {
        printf("%s %d\n", __func__, i);
        yield();
    }
    printf("%s end\n", __func__);
}

void func2() {
    for (int i=0; i<10; i++) {
        printf("%s %d\n", __func__, i);
        yield();
    }
    printf("%s end\n", __func__);
}

void func3() {
    for (int i=0; i<10; i++) {
        printf("%s %d\n", __func__, i);
        yield();
    }
    printf("%s end\n", __func__);
}

int do_fib(int n) {
    if (n < 2) {
        return 1;
    }
    yield();
    return do_fib(n - 1) + do_fib(n - 2);
}

void fib() {
    int v = do_fib(10);
    printf("fib 10 = %d\n", v);
}

int main(void) {
    spawn(func1);
    spawn(func2);
    spawn(func3);
    spawn(fib);
    start();
    return 0;
}
