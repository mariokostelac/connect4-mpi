/* Copyright 2014 - Mario Kostelac (mario.kostelac@gmail.com) */
#include "./timer.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>

Timer::Timer(const char * n) : name(NULL) {
    int len = strlen(n);
    name = new char[len + 1];
    snprintf(name, len + 1, n);
    gettimeofday(&start_time, NULL);
}

Timer::~Timer() {
    delete[] name;
}

Timer* Timer::end(bool print) {
    gettimeofday(&end_time, NULL);

    unsigned long long msecs = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_usec - start_time.tv_usec)/1000;

    if (print == true) {
        printf("* %s finished in %llu msec.\n", name, msecs);
    }

    return this;
}
