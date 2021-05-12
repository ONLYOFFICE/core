#include "ioapibuf.h"

static voidpf ZCALLBACK open_buffer_func  OF((voidpf opaque, const char* filename, int mode));
static uLong  ZCALLBACK read_buffer_func  OF((voidpf opaque, voidpf stream, void* buf, uLong size));
static uLong  ZCALLBACK write_buffer_func OF((voidpf opaque, voidpf stream, const void* buf, uLong size));
static long   ZCALLBACK tell_buffer_func  OF((voidpf opaque, voidpf stream));
static long   ZCALLBACK seek_buffer_func  OF((voidpf opaque, voidpf stream, uLong offset, int origin));
static int    ZCALLBACK close_buffer_func OF((voidpf opaque, voidpf stream));
static int    ZCALLBACK error_buffer_func OF((voidpf opaque, voidpf stream));

voidpf ZCALLBACK open_buffer_func (voidpf opaque, const char* filename, int mode)
{
    BUFFER_IO* hFile = NULL;
    if (opaque != NULL)
        hFile = (BUFFER_IO*)opaque;
    if (hFile != NULL)
    {
        if (mode & ZLIB_FILEFUNC_MODE_CREATE)
        {
            if (hFile->bGrow)
            {
                hFile->nSize  = 0xffff;
                hFile->buffer = malloc(hFile->nSize);
            }
            hFile->nLimit = 0;
        }
        else
            hFile->nLimit = hFile->nSize;
    }
    return hFile;
}

uLong  ZCALLBACK read_buffer_func (voidpf opaque, voidpf stream, void* buf, uLong size)
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
        memcpy(buf, hFile->buffer + hFile->nCurrentPos, ret);
        hFile->nCurrentPos += ret;
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
        {
            if (hFile->bGrow)
            {
                uLong nNewSize = hFile->nSize + (size < 0xffff ? 0xffff : size);
                unsigned char* NewBuffer = malloc(nNewSize);
                memcpy(NewBuffer, hFile->buffer, hFile->nSize);
                free(hFile->buffer);
                hFile->buffer = NewBuffer;
                hFile->nSize  = nNewSize;
                ret = size;
            }
            else
                ret = hFile->nSize - hFile->nCurrentPos;
        }
        else
            ret = size;
        memcpy(hFile->buffer + hFile->nCurrentPos, buf, ret);
        hFile->nCurrentPos += ret;
        if (hFile->nCurrentPos > hFile->nLimit)
            hFile->nLimit = hFile->nCurrentPos;
    }
    return ret;
}

long   ZCALLBACK tell_buffer_func (voidpf opaque, voidpf stream)
{
    BUFFER_IO* hFile = NULL;
    if (stream != NULL)
        hFile = (BUFFER_IO*)stream;
    return hFile->nCurrentPos;
}

long   ZCALLBACK seek_buffer_func (voidpf opaque, voidpf stream, uLong offset, int origin)
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
        fseek_origin = hFile->nLimit;
        break;
    case ZLIB_FILEFUNC_SEEK_SET :
        fseek_origin = 0;
        break;
    default: return -1;
    }
    if (fseek_origin + offset  > hFile->nSize)
    {
        hFile->error = 1;
        return -1;
    }
    hFile->nCurrentPos = fseek_origin + offset;
    return 0;
}

int    ZCALLBACK close_buffer_func(voidpf opaque, voidpf stream)
{
    /*
    BUFFER_IO* hFile = NULL;
    if (stream != NULL)
        hFile = (BUFFER_IO*)stream;
    free(hFile->buffer);
    hFile->nCurrentPos = 0;
    hFile->nSize = 0;
    hFile->error = 0;
    free(stream);
    */
    return 0;
}

int    ZCALLBACK error_buffer_func(voidpf opaque, voidpf stream)
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

void fill_buffer_filefunc(zlib_filefunc_def* pzlib_filefunc_def, BUFFER_IO* buffer)
{
    pzlib_filefunc_def->zopen_file   = open_buffer_func;
    pzlib_filefunc_def->zread_file   = read_buffer_func;
    pzlib_filefunc_def->zwrite_file  = write_buffer_func;
    pzlib_filefunc_def->ztell_file   = tell_buffer_func;
    pzlib_filefunc_def->zseek_file   = seek_buffer_func;
    pzlib_filefunc_def->zclose_file  = close_buffer_func;
    pzlib_filefunc_def->zerror_file  = error_buffer_func;
    pzlib_filefunc_def->opaque       = buffer;
}
