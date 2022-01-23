//
// Created by simi on 1/22/22.
//

#include <cstdio>

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

extern "C" DLLEXPORT double print(double X) {
    printf("%f", X);
    return 0;
}

extern "C" DLLEXPORT double printc(int X) {
    printf("%c", X);
    return 0;
}