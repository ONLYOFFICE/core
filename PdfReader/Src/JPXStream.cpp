#include "MemoryUtils.h"
#include "JPXStream.h"
#include "File.h"
#include "../../DesktopEditor/common/File.h"

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

		// Инициализация
		m_lCurPos       = 0;
		m_lBufferSize   = 0;
		m_pSourceBuffer = NULL;

		// Создаем темповый файл, в который сбрасываем картинку
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

		// TODO: Jpeg2000 реализовать в DesktopEditor
		NSFile::CFileBinary::Remove(wsTempFile);

		//// Создаем интерфейс для сохранения картинки
		//MediaCore::IAVSUncompressedVideoFrame *pImageMediaData = NULL;
		//CoCreateInstance(MediaCore::CLSID_CAVSMediaData, NULL, CLSCTX_ALL, MediaCore::IID_IAVSMediaData, (void**)(&pImageMediaData));
		//if (!pImageMediaData)
		//{
		//	_wunlink(wsTempFile.GetBuffer());
		//	return;
		//}

		//// Конвертируем картинку в интерфейс
		//AVSImageJpeg2000::IJ2kFile *pJpeg = NULL;
		//CoCreateInstance(__uuidof(AVSImageJpeg2000::CJ2kFile), NULL, CLSCTX_INPROC_SERVER, __uuidof(AVSImageJpeg2000::IJ2kFile), (void **)(&pJpeg));
		//if (!pJpeg)
		//{
		//	_wunlink(wsTempFile.GetBuffer());
		//	RELEASEINTERFACE(pImageMediaData);
		//	return;
		//}

		//IUnknown **ppImage = (IUnknown**)&pImageMediaData;
		//pJpeg->J2kToInterface(wsTempFile.GetBuffer(), ppImage, L"<Jpeg2000-Options><OpenOptions><ConvertToRGBA value=\"0\"/></OpenOptions></Jpeg2000-Options>");
		//// Удаляем временный файл
		//_wunlink(wsTempFile.GetBuffer());

		//if (NULL == ppImage || NULL == (*ppImage))
		//{
		//	RELEASEINTERFACE(pImageMediaData);
		//	RELEASEINTERFACE(pJpeg);
		//	return;
		//}

		//MediaCore::IAVSUncompressedVideoFrame* pMediaDataIn = NULL;
		//(*ppImage)->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaDataIn));
		//if (NULL == pMediaDataIn)
		//{
		//	RELEASEINTERFACE(pImageMediaData);
		//	RELEASEINTERFACE(pJpeg);
		//	return;
		//}

		//LONG lWidth          =    0; pMediaDataIn->get_Width(&lWidth);
		//LONG lHeight         =    0; pMediaDataIn->get_Height(&lHeight);
		//LONG lAspectX        =    0; pMediaDataIn->get_AspectRatioX(&lAspectX);
		//LONG lAspectY        =    0; pMediaDataIn->get_AspectRatioY(&lAspectY);
		//LONG lStride         =    0; pMediaDataIn->get_Stride(0, &lStride);
		//unsigned char *pBufferPointer = NULL; pMediaDataIn->get_BufferSize(&m_lBufferSize);

		//pMediaDataIn->get_Buffer(&pBufferPointer);

		//// Копируем данные в буфер
		//m_pSourceBuffer = (unsigned char*)MemUtilsMalloc(m_lBufferSize);
		//if (!m_pSourceBuffer)
		//{
		//	m_lBufferSize = 0;
		//	RELEASEINTERFACE(pMediaDataIn);
		//	RELEASEINTERFACE(pImageMediaData);
		//	RELEASEINTERFACE(pJpeg);
		//	return;
		//}

		//// Переворачиваем картинку
		//for (int nY = 0; nY < lHeight; nY++)
		//{
		//	::memcpy(m_pSourceBuffer + lStride * nY, pBufferPointer + lStride * (lHeight - nY - 1), lStride);
		//}

		//// Освобождаем все интерфейсы
		//RELEASEINTERFACE(pMediaDataIn);
		//RELEASEINTERFACE(pJpeg);
		//RELEASEINTERFACE(pImageMediaData);
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