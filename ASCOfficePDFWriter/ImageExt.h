#pragma once
#include "stdafx.h"

namespace NSImageExt
{
	class CFile 
	{
	public:
		CFile() 
		{
			m_hFileHandle = NULL;
			m_lFileSize = 0;
			m_lFilePosition = 0;
		}

		virtual ~CFile()
		{
			CloseFile();
		}

		virtual HRESULT OpenFile(CString FileName)
		{	
			CloseFile();

			HRESULT hRes = S_OK;
			DWORD AccessMode = GENERIC_READ;
			DWORD ShareMode = FILE_SHARE_READ;
			DWORD Disposition = OPEN_EXISTING;
			m_hFileHandle = ::CreateFile(FileName, AccessMode, ShareMode, NULL, Disposition, FILE_ATTRIBUTE_NORMAL, NULL);

			if (NULL == m_hFileHandle || INVALID_HANDLE_VALUE == m_hFileHandle)
				hRes = S_FALSE;
			else 
			{
				ULARGE_INTEGER nTempSize;
				nTempSize.LowPart = ::GetFileSize(m_hFileHandle, &nTempSize.HighPart);
				m_lFileSize = nTempSize.QuadPart;

				SetPosition(0);
			}

			return hRes;
		}

		virtual HRESULT OpenFileRW(CString FileName)
		{	
			CloseFile();

			HRESULT hRes = S_OK;
			DWORD AccessMode = GENERIC_READ | GENERIC_WRITE;
			DWORD ShareMode = FILE_SHARE_READ;
			DWORD Disposition = OPEN_EXISTING;
			m_hFileHandle = ::CreateFile(FileName, AccessMode, ShareMode, NULL, Disposition, 0, 0);

			if (NULL == m_hFileHandle || INVALID_HANDLE_VALUE == m_hFileHandle)
			{
				hRes = S_FALSE;
			}
			else 
			{
				ULARGE_INTEGER nTempSize;
				nTempSize.LowPart = ::GetFileSize(m_hFileHandle, &nTempSize.HighPart);
				m_lFileSize = nTempSize.QuadPart;

				SetPosition(0);
			}

			return hRes;
		}

		HRESULT ReadFile(BYTE* pData, DWORD nBytesToRead)
		{
			DWORD nBytesRead = 0;
			if(NULL == pData)
				return S_FALSE;

			if(m_hFileHandle && (pData))
			{	
				SetPosition(m_lFilePosition);
				::ReadFile(m_hFileHandle, pData, nBytesToRead, &nBytesRead, NULL);
				m_lFilePosition += nBytesRead; 
			}
			return S_OK;
		}

		HRESULT ReadFile2(BYTE* pData, DWORD nBytesToRead)
		{
			DWORD nBytesRead = 0;
			if(NULL == pData)
				return S_FALSE;

			if(m_hFileHandle && (pData))
			{	
				SetPosition(m_lFilePosition);
				::ReadFile(m_hFileHandle, pData, nBytesToRead, &nBytesRead, NULL);
				m_lFilePosition += nBytesRead; 

				for (size_t index = 0; index < nBytesToRead / 2; ++index)
				{
					BYTE temp = pData[index];
					pData[index] = pData[nBytesToRead - index - 1];
					pData[nBytesToRead - index - 1] = temp;
				}
			}
			return S_OK;
		}
		HRESULT ReadFile3(void* pData, DWORD nBytesToRead)
		{
			DWORD nBytesRead = 0;
			if(NULL == pData)
				return S_FALSE;

			if(m_hFileHandle && (pData))
			{	
				SetPosition(m_lFilePosition);
				::ReadFile(m_hFileHandle, pData, nBytesToRead, &nBytesRead, NULL);
				m_lFilePosition += nBytesRead; 
			}
			return S_OK;
		}

		HRESULT WriteFile(void* pData, DWORD nBytesToWrite)
		{
			if(m_hFileHandle)
			{	
				DWORD dwWritten = 0;
				::WriteFile(m_hFileHandle, pData, nBytesToWrite, &dwWritten, NULL);
				m_lFilePosition += nBytesToWrite; 
			}
			return S_OK;
		}

		HRESULT WriteFile2(void* pData, DWORD nBytesToWrite)
		{
			if(m_hFileHandle)
			{	
				BYTE* mem = new BYTE[nBytesToWrite];
				memcpy(mem, pData, nBytesToWrite);
				
				for (size_t index = 0; index < nBytesToWrite / 2; ++index)
				{
					BYTE temp = mem[index];
					mem[index] = mem[nBytesToWrite - index - 1];
					mem[nBytesToWrite - index - 1] = temp;
				}
				
				DWORD dwWritten = 0;
				::WriteFile(m_hFileHandle, (void*)mem, nBytesToWrite, &dwWritten, NULL);
				m_lFilePosition += nBytesToWrite; 
				RELEASEARRAYOBJECTS(mem);
			}
			return S_OK;
		}

		HRESULT CreateFile(CString strFileName)
		{
			CloseFile();
			DWORD AccessMode = GENERIC_WRITE;
			DWORD ShareMode = FILE_SHARE_WRITE;
			DWORD Disposition = CREATE_ALWAYS;
			m_hFileHandle = ::CreateFile(strFileName, AccessMode, ShareMode, NULL, Disposition, FILE_ATTRIBUTE_NORMAL, NULL);
			return SetPosition(0);
		}
		HRESULT SetPosition( ULONG64 nPos )
		{	
			if (m_hFileHandle && nPos < (ULONG)m_lFileSize)
			{
				LARGE_INTEGER nTempPos;
				nTempPos.QuadPart = nPos;
				::SetFilePointer(m_hFileHandle, nTempPos.LowPart, &nTempPos.HighPart, FILE_BEGIN);
				m_lFilePosition = nPos;
				return S_OK;
			}
			else 
			{
				return (INVALID_HANDLE_VALUE == m_hFileHandle) ? S_FALSE : S_OK;
			}
		}
		LONG64  GetPosition()
		{
			return m_lFilePosition;
		}
		HRESULT SkipBytes(ULONG64 nCount)
		{
			return SetPosition(m_lFilePosition + nCount);
		}

		HRESULT CloseFile()
		{
			m_lFileSize = 0;
			m_lFilePosition = 0;
			RELEASEHANDLE(m_hFileHandle);
			return S_OK;
		}

		ULONG64 GetFileSize()
		{
			return m_lFileSize;
		}

		HRESULT WriteReserved(DWORD dwCount)
		{
			BYTE* buf = new BYTE[dwCount];
			memset(buf, 0, (size_t)dwCount);
			HRESULT hr = WriteFile(buf, dwCount);
			RELEASEARRAYOBJECTS(buf);
			return hr;
		}
		HRESULT WriteReserved2(DWORD dwCount)
		{
			BYTE* buf = new BYTE[dwCount];
			memset(buf, 0xFF, (size_t)dwCount);
			HRESULT hr = WriteFile(buf, dwCount);
			RELEASEARRAYOBJECTS(buf);
			return hr;
		}
		HRESULT WriteReservedTo(DWORD dwPoint)
		{
			if (m_lFilePosition >= dwPoint)
				return S_OK;

			DWORD dwCount = dwPoint - (DWORD)m_lFilePosition;
			BYTE* buf = new BYTE[dwCount];
			memset(buf, 0, (size_t)dwCount);
			HRESULT hr = WriteFile(buf, dwCount);
			RELEASEARRAYOBJECTS(buf);
			return hr;
		}
		HRESULT SkipReservedTo(DWORD dwPoint)
		{
			if (m_lFilePosition >= dwPoint)
				return S_OK;

			DWORD dwCount = dwPoint - (DWORD)m_lFilePosition;
			return SkipBytes(dwCount);
		}

		LONG GetProgress()
		{
			if (0 >= m_lFileSize)
				return -1;

			double dVal = (double)(100 * m_lFilePosition);
			LONG lProgress = (LONG)(dVal / m_lFileSize);
			return lProgress;
		}

	protected:
		HANDLE m_hFileHandle;		
		LONG64 m_lFileSize;
		LONG64 m_lFilePosition;
	};

	class CImageExt
	{
	public:
		LONG GetImageType(CString strFile)
		{		
			CFile oFile;
			if (S_OK != oFile.OpenFile(strFile))
			{
				oFile.CloseFile();
				return 0;
			}

			DWORD dwSize = (DWORD)oFile.GetFileSize();

			if (44 > dwSize)
			{
				return 0;
			}

			DWORD dwKey = 0;
			oFile.ReadFile((BYTE*)(&dwKey), 4);

			if (0x9AC6CDD7 == dwKey)
			{
				// placeable meta
				oFile.CloseFile();
				return 1;
			}

			if (0x00000001 == dwKey)
			{
				oFile.SetPosition(40);
				oFile.ReadFile((BYTE*)(&dwKey), 4);
				oFile.CloseFile();

				if (0x464D4520 == dwKey)
				{
					// EMF/EMF+
					return 2;
				}
				else
				{
					// 
					return 0;
				}
			}

			oFile.CloseFile();

			if (0x00090001 == dwKey)
			{
				return 1;
			}

			return 0;
		}

		BOOL DrawOnRenderer(IASCRenderer* pRenderer, BSTR strFile, double dX, double dY, double dW, double dH)
		{
#if BUILD_CONFIG_FULL_VERSION
			if (NULL == pRenderer)
				return FALSE;

			LONG lFileType = GetImageType((CString)strFile);

			if (1 == lFileType || 2 == lFileType)
			{
				// Запоминаем все настройки Brush, Font, Pen, Shadow и т.д.
				LONG lBrAlpha1 = 0; pRenderer->get_BrushAlpha1( &lBrAlpha1 );
				LONG lBrAlpha2 = 0; pRenderer->get_BrushAlpha2( &lBrAlpha2 );
				LONG lBrColor1 = 0; pRenderer->get_BrushColor1( &lBrColor1 );
				LONG lBrColor2 = 0; pRenderer->get_BrushColor2( &lBrColor2 );
				double lBrAngle = 0; pRenderer->get_BrushLinearAngle( &lBrAngle );
				LONG lBrTxMode = 0; pRenderer->get_BrushTextureMode( &lBrTxMode );
				LONG lBrTxAlpha = 0; pRenderer->get_BrushTextureAlpha( &lBrTxAlpha );
				BSTR bsTxPath = NULL; pRenderer->get_BrushTexturePath( &bsTxPath );
				LONG lBrType = 0; pRenderer->get_BrushType( &lBrType );

				BSTR bsFontPath	= NULL; pRenderer->get_FontPath(&bsFontPath);
				BSTR bsFontName	= NULL; pRenderer->get_FontName(&bsFontName);
				double dFontSize = 0; pRenderer->get_FontSize(&dFontSize);
				LONG lFontStyle = 0; pRenderer->get_FontStyle(&lFontStyle);
				LONG bFontGid = 0; pRenderer->get_FontStringGID( &bFontGid );
				
				LONG lPenAlpha = 0; pRenderer->get_PenAlpha(&lPenAlpha);
				LONG lPenColor = 0; pRenderer->get_PenColor(&lPenColor);
				LONG lPenAlign = 0; pRenderer->get_PenAlign(&lPenAlign);
				double dPenSize = 0; pRenderer->get_PenSize(&dPenSize);

				AVSGraphics::IAVSImage *pImage = NULL;
				CoCreateInstance( __uuidof(AVSGraphics::CAVSImage), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IAVSImage), (void**)(&pImage) );

				pImage->put_FontManager(NULL);

				pImage->LoadFromFile( strFile );
				pImage->DrawOnRenderer( pRenderer, dX, dY, dW, dH );

				RELEASEINTERFACE( pImage );

				// Восстанавливаем параметры

				pRenderer->put_BrushAlpha1( lBrAlpha1 );
				pRenderer->put_BrushAlpha2( lBrAlpha2 );
				pRenderer->put_BrushColor1( lBrColor1 );
				pRenderer->put_BrushColor2( lBrColor2 );
				pRenderer->put_BrushLinearAngle( lBrAngle );
				pRenderer->put_BrushTextureMode( lBrTxMode );
				pRenderer->put_BrushTextureAlpha( lBrTxAlpha );
				pRenderer->put_BrushTexturePath( bsTxPath ); 
				RELEASESYSSTRING(bsTxPath);
				pRenderer->put_BrushType( lBrType );

				pRenderer->put_FontName( bsFontName ); RELEASESYSSTRING(bsFontName);
				pRenderer->put_FontPath( bsFontPath ); RELEASESYSSTRING(bsFontPath);
				pRenderer->put_FontSize( dFontSize );
				pRenderer->put_FontStyle( lFontStyle );
				pRenderer->put_FontStringGID( bFontGid );

				pRenderer->put_PenAlign( lPenAlign );
				pRenderer->put_PenAlpha( lPenAlpha );
				pRenderer->put_PenColor( lPenColor );
				pRenderer->put_PenSize(dPenSize);

				// на всякий случай скидываем path
				pRenderer->PathCommandEnd();

				return TRUE;
			}
#endif
			
			return FALSE;
		}
	};
}