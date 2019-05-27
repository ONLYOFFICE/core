#include <stdlib.h>
#include <stdio.h>
#include "gcvt.h"

char* gcvt(double x, int n, char* b) {
    sprintf(b, "%.*g", n, x);
    return b;
}