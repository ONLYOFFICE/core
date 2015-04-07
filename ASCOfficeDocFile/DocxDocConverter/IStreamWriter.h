#pragma once

#include "../pole/pole.h"
#include "../../DesktopEditor/common/Types.h"

namespace ASCDocFileFormat
{
	struct IStreamWriter
	{
		virtual ~IStreamWriter()
		{

		}

		virtual long WriteToStream(POLE::Stream* pStream, unsigned long offset = 0) const
		{
			HRESULT hr				=	S_FALSE;

			unsigned long size		=	0;
			ULONG writtenSize		=	0;

			unsigned char* bytes				=	GetBytes(&size);

			if (NULL != bytes)
			{
				pStream->seek(offset/*, STREAM_SEEK_SET*/);
				writtenSize			=	pStream->write(bytes, size );

				RELEASEARRAYOBJECTS (bytes);
			}

			return hr;
		}

		//
		virtual unsigned char* GetBytes(unsigned long* size) const	=	0;
	};
}
