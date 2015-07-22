#include "MemoryUtils.h"
#include "JPXStream.h"
#include "File.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"

namespace PdfReader
{
	JPXStream::JPXStream(Stream *pStream) :
		FilterStream(pStream)
	{
		m_lCurPos       = 0;
		m_lBufferSize   = 0;
		m_pSourceBuffer = NULL;
	}

	JPXStream::~JPXStream()
	{
		Close();
		delete m_pStream;
	}

	void JPXStream::Reset()
	{
		m_pStream->Reset();

		// »нициализаци¤
		m_lCurPos       = 0;
		m_lBufferSize   = 0;
		m_pSourceBuffer = NULL;

		// —оздаем темповый файл, в который сбрасываем картинку
		FILE *pTempFile = NULL;

		std::wstring wsTempFile = L"";
		if (!NSFile::CFileBinary::OpenTempFile(&wsTempFile, &pTempFile, L"wb", NULL, NULL))
		{
			NSFile::CFileBinary::Remove(wsTempFile);
			return;
		}

		int nChar = 0;
		unsigned char *pBuffer = (unsigned char*)MemUtilsMalloc(32);
		if (pBuffer)
		{
			int nSize = 32, nLen = 0;
			while (EOF != (nChar = m_pStream->GetChar()))
			{
				if (nLen >= nSize)
				{
					nSize *= 2;
					pBuffer = (unsigned char*)MemUtilsRealloc(pBuffer, nSize);
				}

				pBuffer[nLen] = nChar;
				nLen++;
			}
			fwrite(pBuffer, 1, nLen, pTempFile);
			fclose(pTempFile);
			MemUtilsFree(pBuffer);
		}
		else
		{
			while (EOF != (nChar = m_pStream->GetChar()))
			{
				fputc(nChar, pTempFile);
			}
			fclose(pTempFile);
		}

		CBgraFrame oFrame;
		if (!oFrame.OpenFile(wsTempFile, _CXIMAGE_FORMAT_JP2))
		{
			NSFile::CFileBinary::Remove(wsTempFile);
			return;
		}

		int nHeight          = oFrame.get_Height();
		int nWidth           = oFrame.get_Width();
		int nStride          = oFrame.get_Stride();
		BYTE* pBufferPointer = oFrame.get_Data();

		m_lBufferSize = 3 * nWidth * nHeight;

		m_pSourceBuffer = (unsigned char*)MemUtilsMalloc(m_lBufferSize);
		if (!m_pSourceBuffer)
		{
			NSFile::CFileBinary::Remove(wsTempFile);
			m_lBufferSize = 0;
			return;
		}

		unsigned char* pDst = m_pSourceBuffer;
		for (int nY = 0; nY < nHeight; nY++)
		{
			unsigned char* pSrc = pBufferPointer + nWidth * 4 * (nHeight - nY - 1);

			for (int nX = 0; nX < nWidth; nX++)
			{
				pDst[0] = pSrc[2];
				pDst[1] = pSrc[1];
				pDst[2] = pSrc[0];

				pDst += 3;
				pSrc += 4;
			}
		}

		NSFile::CFileBinary::Remove(wsTempFile);
	}

	void JPXStream::Close()
	{
		MemUtilsFree(m_pSourceBuffer);
		m_pSourceBuffer = NULL;

		FilterStream::Close();
	}

	int JPXStream::GetChar()
	{
		int nChar = 0;

		if (m_lCurPos < m_lBufferSize)
		{
			nChar = m_pSourceBuffer[m_lCurPos];
			m_lCurPos++;
		}
		else
			return EOF;

		return nChar;
	}

	int JPXStream::LookChar()
	{
		if (m_lBufferSize > 0 && m_lCurPos < m_lBufferSize)
			return m_pSourceBuffer[m_lCurPos];

		return EOF;
	}
	StringExt *JPXStream::GetPSFilter(int nPSLevel, char *sIndent)
	{
		return NULL;
	}

	bool JPXStream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(true);
	}

	void JPXStream::GetImageParams(int *pBitsPerComponent, StreamColorSpaceMode *peModeCS)
	{
		*pBitsPerComponent = 8;
		*peModeCS = streamCSDeviceRGB;
	}
}
