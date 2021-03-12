#ifndef __xmemfile_h
#define __xmemfile_h

#include "xfile.h"

class CxMemFile : public CxFile
{
public:
	CxMemFile(uint8_t* pBuffer = NULL, uint32_t size = 0);
	~CxMemFile();

    virtual bool	Close();
    virtual bool	Seek(int32_t offset, int32_t origin);
    virtual int32_t	Tell();

protected:
    uint8_t*	m_pBuffer;
    uint32_t	m_Size;
    bool	m_bFreeOnClose;
    int32_t	m_Position;	//current position
    int32_t	m_Edge;		//buffer size
    bool	m_bEOF;
};

#endif // __xmemfile_h
