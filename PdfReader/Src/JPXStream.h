#ifndef _PDF_READER_JPXSTREAM_H
#define _PDF_READER_JPXSTREAM_H

#include "Object.h"
#include "Stream.h"

namespace PdfReader
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// JPXStream
	//-------------------------------------------------------------------------------------------------------------------------------

	class JPXStream : public FilterStream
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
		virtual bool IsBinary(bool bLast = true);
		virtual void GetImageParams(int *pBitsPerComponent, StreamColorSpaceMode *peModeCS);

	private:

		long           m_lBufferSize;
		unsigned char* m_pSourceBuffer;
		long           m_lCurPos;

	};
}

#endif // _PDF_READER_JPXSTREAM_H
