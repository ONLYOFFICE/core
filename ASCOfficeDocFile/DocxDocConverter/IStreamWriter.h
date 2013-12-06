#pragma once

namespace ASCDocFileFormat
{
	struct IStreamWriter
	{
		virtual ~IStreamWriter()
		{

		}

		virtual long WriteToStream(IStream* pStream, unsigned long offset = 0) const
		{
			HRESULT hr				=	S_FALSE;

			unsigned long size		=	0;
			ULONG writtenSize		=	0;

			byte* bytes				=	GetBytes(&size);

			if (NULL != bytes)
			{
				LARGE_INTEGER pos	=	{0, 0};
				pos.HighPart		=	0;
				pos.LowPart			=	offset;

				hr					=	pStream->Seek(pos, STREAM_SEEK_SET, NULL);
				hr					=	pStream->Write(bytes, size, &writtenSize);

				RELEASEARRAYOBJECTS (bytes);
			}

			return hr;
		}

		//
		virtual byte* GetBytes(unsigned long* size) const	=	0;
	};
}