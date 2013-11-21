#pragma once
#include "../Common/ASCUtils.h"
#include "windows.h"

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

public:
	static DWORD CorrectDirectorySrc(CString sPath)
	{
		DWORD dwFileAttrib = ::GetFileAttributes( sPath );

		if (dwFileAttrib == INVALID_FILE_ATTRIBUTES)
			return 1;

		if (0 == (FILE_ATTRIBUTE_DIRECTORY & dwFileAttrib))
			return 1;

		bool bIsNeedRemove = true;
		CAtlArray<CString> arrPieces;

		HANDLE Handle;
		WIN32_FIND_DATA FindData;
		DWORDLONG Result = 0;

		Handle = FindFirstFile( ( sPath + _T("\\*.*") ), &FindData );
		if ( Handle == INVALID_HANDLE_VALUE )
			return 0;
		do
		{
			if ( ( CString( FindData.cFileName ) != _T(".") ) && ( CString( FindData.cFileName ) != _T("..") ) )
			{
				if ( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					Result += CorrectDirectorySrc( sPath + _T("\\") + FindData.cFileName );
					bIsNeedRemove = false;
				}
				else
				{
					if (bIsNeedRemove == true)
					{
						CString strName(FindData.cFileName);
						int nIndexExt = strName.ReverseFind(TCHAR('.'));
						if (nIndexExt != -1)
						{
							CString strExt = strName.Mid(nIndexExt);
							if (_T(".piece") != strExt)
							{
								bIsNeedRemove = false;
							}
							else
							{
								arrPieces.Add(sPath + _T("\\") + FindData.cFileName);
							}
						}
					}
				}
			}
		}
		while( FindNextFile( Handle, &FindData ) != 0 );
		FindClose( Handle );
		
		if (bIsNeedRemove && (0 != arrPieces.GetCount()))
		{
			CFile oFile;
			CString sPathDst = sPath + _T(".temp");
			oFile.CreateFile(sPathDst);

			size_t nCount = arrPieces.GetCount();
			
			// сначала нужно отсортировать по номерам piece.
			CAtlArray<LONG> arrayInds;
			CAtlArray<LONG> arraySorts;

			arrayInds.SetCount(nCount);
			arraySorts.SetCount(nCount);

			for (size_t i = 0; i < nCount; ++i)
			{
				arrayInds[i] = (LONG)i;
				arraySorts[i] = (LONG)i;

				LONG i1 = arrPieces[i].ReverseFind('[');
				LONG i2 = arrPieces[i].ReverseFind(']');

				if (i1 != -1 && i2 != -1 && i1 < (i2 - 1))
				{
					CString sInd = arrPieces[i].Mid(i1 + 1, i2 - i1 - 1);
					arraySorts[i] = XmlUtils::GetInteger(sInd);
				}
			}
			// теперь сортируем
			for (size_t i = 0; i < nCount; ++i)
			{
				for (size_t j = i + 1; j < nCount; ++j)
				{
					if (arraySorts[i] > arraySorts[j])
					{
						LONG tmp = arraySorts[i];
						arraySorts[i] = arraySorts[j];
						arraySorts[j] = tmp;

						tmp = arrayInds[i];
						arrayInds[i] = arrayInds[j];
						arrayInds[j] = tmp;
					}
				}
			}

			for (size_t i = 0; i < nCount; ++i)
			{
				CFile oPiece;
				oPiece.OpenFile(arrPieces[arrayInds[i]]);

				DWORD dwPieceSize = (DWORD)oPiece.GetFileSize();
				BYTE* pData = new BYTE[dwPieceSize];
				oPiece.ReadFile(pData, dwPieceSize);
				oFile.WriteFile((void*)pData, dwPieceSize);
				RELEASEARRAYOBJECTS(pData);
				oPiece.CloseFile();
			}

			oFile.CloseFile();

			RemoveDir(sPath);
			MoveFile(sPathDst, sPath);
		}

		return 0;
	}

	static DWORD RemoveDir(CString sPath)
	{
		DWORD dwFileAttrib = ::GetFileAttributes( sPath );
		if(  dwFileAttrib != INVALID_FILE_ATTRIBUTES )
		{
			DWORD dwResult = 0;
			if( 0 != (FILE_ATTRIBUTE_DIRECTORY & dwFileAttrib) )
			{
				HANDLE Handle;
				WIN32_FIND_DATA FindData;
				DWORDLONG Result = 0;

				Handle = FindFirstFile( ( sPath + _T("\\*.*") ), &FindData );
				if ( Handle == INVALID_HANDLE_VALUE )
					return 0;
				do
				{
					BOOL bRes = TRUE;
					if( ( CString( FindData.cFileName ) != _T(".") ) && ( CString( FindData.cFileName ) != _T("..") ) )
						if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
							Result += RemoveDir( sPath + _T("\\") + FindData.cFileName ); //
						else
							bRes = DeleteFile( sPath + _T("\\") + FindData.cFileName );
					if( FALSE == bRes )
						dwResult += 1;
				}
				while( FindNextFile( Handle, &FindData ) != 0 );
				FindClose( Handle );
				BOOL bRes = RemoveDirectory( sPath );
				if( FALSE == bRes )
					dwResult += 1;
			}

			return dwResult;
		}
		return 0;
	}
};