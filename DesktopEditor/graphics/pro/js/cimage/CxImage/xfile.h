#ifndef __xfile_h
#define __xfile_h

#include "ximadef.h"

class CxFile
{
public:
    CxFile() {}
    virtual ~CxFile() {}

    virtual bool Close() = 0;
    virtual size_t Read(void* buffer, size_t size, size_t count, void* limit_start = NULL, void* limit_end = NULL) = 0;
    virtual size_t Write(const void* buffer, size_t size, size_t count) = 0;
    virtual bool Seek(int32_t offset, int32_t origin) = 0;
    virtual int32_t Tell()  = 0;
    virtual bool    Flush() = 0;
    virtual int32_t	Error() = 0;
    virtual int32_t	GetC()  = 0;
};

#endif //__xfile_h
