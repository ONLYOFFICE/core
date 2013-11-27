#pragma once

#include <atlenc.h>
#include <atlcoll.h>
#include "gdiplus.h"
#include "CHtmlUtility.h"

#include "../prj/Base64.h"
	

class CHtmlBitmap
{
	CString m_strName;
	CString m_strData;
	CString m_strPath;
	
	BYTE   *m_pBuffer;
	int     m_nSize;

public:

	CHtmlBitmap()
	{
		m_pBuffer = NULL;
		m_nSize   = 0;
	}
	~CHtmlBitmap()
	{
		if ( m_pBuffer && m_nSize != 0 )
		{
			delete []m_pBuffer;
			m_pBuffer = NULL;
			m_nSize = 0;
		}
	}

	BOOL Create(Gdiplus::Bitmap* pBitmap, CString strName, CString strPath)
	{
		if (!pBitmap || strName.GetLength() < 1)
			return FALSE;

		CString strTempPath = CHtmlUtility::GetTempName();

		if (!CHtmlUtility::SaveAsPng(pBitmap, strTempPath) )
			//if (!CHtmlUtility::SaveAsJpeg(pBitmap, strTempPath, 95))
			return FALSE;

		m_nSize = 0;

		//CHtmlUtility::GetFileData( strTempPath, m_pBuffer, m_nSize );

		// открываем файл
		HANDLE hFile = CreateFile( strTempPath.GetBuffer(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if ( INVALID_HANDLE_VALUE == hFile )
			return FALSE;

		// мапим этот файл в память - так быстрее читаются данные из файла
		DWORD  nFileSize = GetFileSize( hFile, NULL );
		HANDLE hMapFile  = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL );
		if ( NULL == hMapFile )
		{
			CloseHandle( hFile );
			return FALSE; // Невозможно создать отображение файла
		}

		// создаем view of file
		void* pBaseAddress	= MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
		if ( !pBaseAddress )
		{
			CloseHandle( hMapFile );
			CloseHandle( hFile );
			return FALSE;
		}

		if ( m_pBuffer )
			delete []m_pBuffer;

		m_pBuffer = new BYTE[ nFileSize ];
		if ( !m_pBuffer )
		{
			UnmapViewOfFile( pBaseAddress );
			CloseHandle( hMapFile );
			CloseHandle( hFile );
			return FALSE;
		}
		m_nSize = nFileSize;

		::memcpy( m_pBuffer, (BYTE*)pBaseAddress, sizeof(BYTE) * m_nSize );

		UnmapViewOfFile( pBaseAddress );
		CloseHandle( hMapFile );
		CloseHandle( hFile );

		CBase64 oBase64;
		oBase64.Encode( &m_pBuffer, (unsigned int *)&m_nSize );

		m_strData = CHtmlUtility::GetFileInBase64(strTempPath);
		m_strName = strName;
		m_strPath = strPath;

		DeleteFile(strTempPath);

		return TRUE;
	}


	BOOL Create(CString bitmap, CString strName, CString strPath)
	{
		m_strData = bitmap;
		m_strName = strName;
		m_strPath = strPath;

		m_pBuffer = NULL;
		m_nSize   = 0;

		return TRUE;
	}


	BOOL WriteFile()
	{
		BYTE* bytes = NULL;
		int size;
		CHtmlUtility::GetFileInBinary(m_strData, bytes, size);

		if (!CHtmlUtility::SaveFile(bytes, size, m_strPath))
		{
			delete[] bytes;
			return FALSE;
		}
		delete[] bytes;
		return TRUE;
	}


	CString GetName()
	{
		return m_strName;
	}


	CString GetData()
	{
		return m_strData;
	}

	BYTE *GetData2()
	{
		return m_pBuffer;
	}
	int   GetSize2()
	{
		return m_nSize;
	}

	CString GetPath()
	{
		return m_strPath;
	}

};