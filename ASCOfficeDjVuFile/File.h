#pragma once
#include "XmlUtils.h"
#include "AVSUtils.h"
#include "windows.h"

#ifndef		pow2_16
#define		pow2_16			65536
#endif

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
