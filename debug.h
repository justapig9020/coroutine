#pragma once

#ifdef DEBUG
    #include <stdio.h>
    #define dbg(...) printf(__VA_ARGS__)
#else
    #define dbg(...)
#endif