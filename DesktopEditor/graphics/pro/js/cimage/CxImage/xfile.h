#ifndef __xfile_h
#define __xfile_h

#include "ximadef.h"

class CxFile
{
public:
    CxFile(void) {}
    virtual ~CxFile() {}

    virtual bool	Close() = 0;
    virtual bool	Seek(int32_t offset, int32_t origin) = 0;
    virtual int32_t	Tell() = 0;
};

#endif //__xfile_h
