//
// Created by simi on 1/22/22.
//

#include <cstdio>
#include <cstdlib>
#include <ctime>

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

extern "C" DLLEXPORT double read() {
    char num[100];
    scanf("%s", num);
    return strtod(num, nullptr);
}

extern "C" DLLEXPORT double next_random() {
    return (double)rand() / (double)RAND_MAX;
}

extern "C" DLLEXPORT double set_rand_seed() {
    srand(time(0));
    return 0;
}

extern "C" DLLEXPORT double tprint(double x) {
    printf("%i", (int)x);
    return 0;
}