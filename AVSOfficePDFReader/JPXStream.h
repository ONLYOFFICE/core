#ifndef _JPXSTREAM_H
#define _JPXSTREAM_H

#include "Object.h"
#include "Stream.h"

//-------------------------------------------------------------------------------------------------------------------------------
// JPXStream
//-------------------------------------------------------------------------------------------------------------------------------

class JPXStream: public FilterStream 
{
public:

	JPXStream(Stream *pStream);
	virtual ~JPXStream();
	virtual StreamType GetType() 
	{ 
		return strJPX; 
	}
	virtual void Reset();
	virtual void Close();
	virtual int GetChar();
	virtual int LookChar();
	virtual StringExt *GetPSFilter(int psLevel, char *indent);
	virtual BOOL IsBinary(BOOL bLast = TRUE);
	virtual void GetImageParams(int *pBitsPerComponent, StreamColorSpaceMode *peModeCS);

private:

	LONG          m_lBufferSize;
	BYTE         *m_pSourceBuffer;
	LONG          m_lCurPos;

};

#endif /* _JPXSTREAM_H */
