#pragma once
#include "..\stdafx.h"

namespace NSTextRenderer
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
		LONG64 GetPosition()
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
}


namespace NSTextRenderer
{
	inline static double FABS(double dVal)
	{
		return (dVal >= 0) ? dVal : -dVal;
	}
	inline static int round(double dVal)
	{
		return (int)(dVal + 0.5);
	}

	class CStringWriter
	{
	private:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		CStringWriter()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;
		}
		~CStringWriter()
		{
			RELEASEMEM(m_pData);
		}

		__forceinline void AddSize(size_t nSize)
		{
			if (NULL == m_pData)
			{
				m_lSize = max(nSize, 1000);				
				m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
				
				m_lSizeCur = 0;
				m_pDataCur = m_pData;
				return;
			}

			if ((m_lSizeCur + nSize) > m_lSize)
			{
				while ((m_lSizeCur + nSize) > m_lSize)
				{
					m_lSize *= 2;
				}

				wchar_t* pRealloc = (wchar_t*)realloc(m_pData, m_lSize * sizeof(wchar_t));
				if (NULL != pRealloc)
				{
					// реаллок сработал
					m_pData		= pRealloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
				else
				{
					wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
					memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

					free(m_pData);
					m_pData		= pMalloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
			}
		}

	public:
		
		inline void WriteString(wchar_t* pString, size_t& nLen)
		{
			AddSize(nLen);
			//memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
			memcpy(m_pDataCur, pString, nLen << 1);
			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}
		inline void WriteString(_bstr_t& bsString)
		{
			size_t nLen = bsString.length();
			WriteString(bsString.GetBSTR(), nLen);
		}
		inline void WriteString(CString& sString)
		{
			size_t nLen = (size_t)sString.GetLength();

			#ifdef _UNICODE
			WriteString(sString.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)sString;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}

		inline size_t GetCurSize()
		{
			return m_lSizeCur;
		}

		inline void Write(CStringWriter& oWriter)
		{
			WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
		}

		inline void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
		inline void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}

		CString GetData()
		{
			CString str(m_pData, (int)m_lSizeCur);
			return str;
		}
	};
}