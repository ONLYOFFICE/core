#include <unistd.h>
#include <byteswap.h>
#include "swab.h"

void swab(void *from, void *to, int n) {
    if (n < 0)
        return;

    for (int i = 0; i < (n/2)*2; i += 2) {
        *((uint16_t*)to+i) = bswap_16(*((uint16_t*)from+i));
    }
}