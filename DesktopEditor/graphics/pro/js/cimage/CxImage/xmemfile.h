#ifndef __xmemfile_h
#define __xmemfile_h

#include "xfile.h"

class CxMemFile : public CxFile
{
public:
    CxMemFile(uint8_t* pBuffer = NULL, uint32_t size = 0);
    ~CxMemFile();

    virtual bool    Close();
    virtual size_t  Read(void* buffer, size_t size, size_t count, void* limit_start = NULL, void* limit_end = NULL);
    virtual size_t	Write(const void* buffer, size_t size, size_t count);
    virtual bool    Seek(int32_t offset, int32_t origin);
    virtual int32_t Tell();
    virtual bool    Flush();
    virtual int32_t Error();
    virtual int32_t	GetC();

protected:
    bool Alloc(uint32_t nBytes);

    uint8_t* m_pBuffer;
    uint32_t m_Size;
    bool     m_bFreeOnClose;
    int32_t  m_Position;
    int32_t  m_Edge;
    bool     m_bEOF;
};

#endif // __xmemfile_h
