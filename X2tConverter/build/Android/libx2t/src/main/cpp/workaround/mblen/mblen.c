#include <stdlib.h>
#include <wchar.h>
#include "mblen.h"

int mblen(const char* s, size_t n) {
    mbstate_t state = {};
    return mbrlen(s, n, &state);
}