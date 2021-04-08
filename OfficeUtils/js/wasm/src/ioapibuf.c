#include "ioapibuf.h"

static voidpf   ZCALLBACK open_buffer_func   OF((voidpf opaque, const void* filename, int mode));
static uLong    ZCALLBACK read_buffer_func   OF((voidpf opaque, voidpf stream, void* buf, uLong size));
static uLong    ZCALLBACK write_buffer_func  OF((voidpf opaque, voidpf stream, const void* buf, uLong size));
static ZPOS64_T ZCALLBACK tell64_buffer_func OF((voidpf opaque, voidpf stream));
static long     ZCALLBACK seek64_buffer_func OF((voidpf opaque, voidpf stream, ZPOS64_T offset, int origin));
static int      ZCALLBACK close_buffer_func  OF((voidpf opaque, voidpf stream));
static int      ZCALLBACK error_buffer_func  OF((voidpf opaque, voidpf stream));

typedef struct {
    char* buffer;
    uLong nSize;
    uLong nCurrentPos;
    int error;
} BUFFER_IO;

voidpf ZCALLBACK open_buffer_func(voidpf opaque, const void* buffer /* filename */, int size /* mode */)
{
    voidpf ret = NULL;
    if (buffer != NULL)
    {
        BUFFER_IO bufio;
        bufio.buffer = malloc(sizeof(char) * size);
        memcpy(bufio.buffer, buffer, size);
        bufio.nSize = size;
        bufio.nCurrentPos = 0;
        bufio.error = 0;

        ret = malloc(sizeof(BUFFER_IO));

        if (ret != NULL)
            *((BUFFER_IO*)ret) = bufio;
    }
    return ret;
}

voidpf ZCALLBACK open64_buffer_func(voidpf opaque, const void* buffer /* filename */, int size /* mode */)
{
    voidpf ret = NULL;
    if (buffer != NULL)
    {
        BUFFER_IO bufio;
        bufio.buffer = malloc(sizeof(char) * size);
        memcpy(bufio.buffer, buffer, size);
        bufio.nSize = size;
        bufio.nCurrentPos = 0;
        bufio.error = 0;

        ret = malloc(sizeof(BUFFER_IO));

        if (ret != NULL)
            *((BUFFER_IO*)ret) = bufio;
    }
    return ret;
}

uLong  ZCALLBACK read_buffer_func(voidpf opaque, voidpf stream, void* buf, uLong size)
{
    uLong ret = 0;
    BUFFER_IO* hFile = NULL;
    if (stream != NULL)
        hFile = (BUFFER_IO*)stream;

    if (hFile != NULL)
    {
        // buf = malloc(sizeof(char) * size); Память уже выделена

        if (hFile->nCurrentPos + size > hFile->nSize)
            ret = hFile->nSize - hFile->nCurrentPos;
        else
            ret = size;

        memcpy(buf, hFile->buffer + hFile->nCurrentPos, ret);

        hFile->nCurrentPos += ret;

        // ошибки
    }

    return ret;
}

uLong  ZCALLBACK write_buffer_func(voidpf opaque, voidpf stream, const void* buf, uLong size)
{
    uLong ret = 0;
    BUFFER_IO* hFile = NULL;
    if (stream != NULL)
        hFile = (BUFFER_IO*)stream;

    if (hFile != NULL)
    {
        if (hFile->nCurrentPos + size > hFile->nSize)
            ret = hFile->nSize - hFile->nCurrentPos;
        else
            ret = size;

        memcpy(hFile->buffer + hFile->nCurrentPos, buf, ret);

        hFile->nCurrentPos += ret;

        // ошибки
    }

    return ret;
}

ZPOS64_T ZCALLBACK tell64_buffer_func(voidpf opaque, voidpf stream)
{
    BUFFER_IO* hFile = NULL;
    if (stream != NULL)
        hFile = (BUFFER_IO*)stream;

    // ошибки

    return hFile->nCurrentPos;
}

long ZCALLBACK seek64_buffer_func(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin)
{
    BUFFER_IO* hFile = NULL;
    if (stream != NULL)
        hFile = (BUFFER_IO*)stream;

    int fseek_origin = 0;
    switch (origin)
    {
    case ZLIB_FILEFUNC_SEEK_CUR :
        fseek_origin = hFile->nCurrentPos;
        break;
    case ZLIB_FILEFUNC_SEEK_END :
        fseek_origin = hFile->nSize;
        break;
    case ZLIB_FILEFUNC_SEEK_SET :
        fseek_origin = 0;
        break;
    default: return -1;
    }

    hFile->nCurrentPos = fseek_origin + offset;

    // ошибки

    return hFile->nCurrentPos;
}

int ZCALLBACK close_buffer_func(voidpf opaque, voidpf stream)
{
    BUFFER_IO* hFile = NULL;
    if (stream != NULL)
        hFile = (BUFFER_IO*)stream;

    free(hFile->buffer);
    hFile->nCurrentPos = 0;
    hFile->nSize = 0;
    hFile->error = 0;
    free(stream);

    // ошибки

    return 0;
}

int ZCALLBACK error_buffer_func(voidpf opaque, voidpf stream)
{
    int ret = -1;
    BUFFER_IO* hFile = NULL;
    if (stream != NULL)
    {
        hFile = (BUFFER_IO*)stream;
        ret = hFile->error;
    }
    return ret;
}

void fill_buffer_filefunc(zlib_filefunc64_def* pzlib_filefunc_def)
{
    pzlib_filefunc_def->zopen64_file = open64_buffer_func;
    pzlib_filefunc_def->zread_file   = read_buffer_func;
    pzlib_filefunc_def->zwrite_file  = write_buffer_func;
    pzlib_filefunc_def->ztell64_file = tell64_buffer_func;
    pzlib_filefunc_def->zseek64_file = seek64_buffer_func;
    pzlib_filefunc_def->zclose_file  = close_buffer_func;
    pzlib_filefunc_def->zerror_file  = error_buffer_func;
    pzlib_filefunc_def->opaque = NULL;
}
