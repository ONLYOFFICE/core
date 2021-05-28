#include <string.h>

#include "../../zlib.h"
#include "ioapi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned char* buffer;
    uLong nSize;
    uLong nLimit;
    uLong nCurrentPos;
    int error;
    int bGrow;
} BUFFER_IO;

void fill_buffer_filefunc OF((zlib_filefunc_def* pzlib_filefunc_def, BUFFER_IO* buffer));

#ifdef __cplusplus
}
#endif
