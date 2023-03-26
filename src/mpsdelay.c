#include "mpsdelay.h"

void mpsdelay(int millis) {
    while (millis-- > 0) {
        volatile int x = 5971;
        while (x-- > 0) {
            __asm("nop");
        }
    }
}

void mpsudelay(int micro) {
    while (micro-- > 0) {
        volatile int x = 59;
        while (x-- > 0) {
            __asm("nop");
        }
    }
}