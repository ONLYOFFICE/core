#include <string>

#include "../../../src/zlib-1.2.11/contrib/minizip/ioapi.h"

#ifdef __cplusplus
extern "C" {
#endif

void fill_buffer_filefunc OF((zlib_filefunc64_def* pzlib_filefunc_def));

#ifdef __cplusplus
}
#endif
