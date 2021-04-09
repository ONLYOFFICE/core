#include <string.h>

#include "../../../src/zlib-1.2.11/zlib.h"
#include "../../../src/zlib-1.2.11/contrib/minizip/ioapi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char* buffer;
    uLong nSize;
    uLong nLimit;
    uLong nCurrentPos;
    int error;
} BUFFER_IO;

void fill_buffer_filefunc OF((zlib_filefunc_def* pzlib_filefunc_def, BUFFER_IO* buffer));

#ifdef __cplusplus
}
#endif
