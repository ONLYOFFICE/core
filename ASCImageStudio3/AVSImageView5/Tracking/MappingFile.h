#pragma once

namespace MemoryMapping
{
	class CMappingFile
	{
	public:
		CMappingFile()
			: m_hMapping(NULL)
			, m_pView(NULL)
			, m_hFile(NULL)
		{
		}
		~CMappingFile()
		{
			Close();
			
		}
		
		BOOL Open(const CString& strPath)
		{
			Close();
			if (OpenExistingFile(strPath))
				return TRUE;
			return FALSE;
		}

		void Close()
		{
			if (m_pView)
			{
				UnmapViewOfFile(m_pView);
				m_pView = 0;
			}

			if (m_hMapping)
			{
				CloseHandle(m_hMapping);
				m_hMapping = 0;
			}
			if (m_hFile)
			{
				CloseHandle(m_hFile);
				m_hFile = NULL;
			}
		}
		BOOL Valid()
		{
			if (m_pView)
				return TRUE;

			return FALSE;
		}
		BYTE* GetData()
		{
			return (BYTE*)m_pView;
		}
		long GetSize() const
		{
			return m_dwDataSize;
		}
	private:
		HANDLE m_hMapping;
		HANDLE m_hFile;
		LPVOID m_pView;
		DWORD m_dwDataSize;
		BOOL OpenExistingFile(const CString& strPath)
		{
			// try to open existing file
			m_hFile= CreateFile(strPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

			// check for valid file
			if (INVALID_HANDLE_VALUE == m_hFile)
			{
				DWORD dwErr = GetLastError();
				return FALSE;
			}

			// variables
			DWORD dwFileSizeHigh = 0;
			m_dwDataSize = 0;

			// retrieve file size to read whole data from file
			m_dwDataSize = GetFileSize(m_hFile, &dwFileSizeHigh);

			// create primary mapping
			m_hMapping = CreateFileMapping(m_hFile, NULL, PAGE_READONLY, dwFileSizeHigh, m_dwDataSize, NULL);

			// check for valid created mapping
			if (!m_hMapping)
			{
				Close();
				return FALSE;
			}

			// create mapping view
			m_pView = MapViewOfFile(m_hMapping, FILE_MAP_READ, 0, 0, 0);

			// check for valid view
			if (!m_pView)
			{
				DWORD dwErr = GetLastError();
				Close();
				return FALSE;
			}				
			// all ok
			return TRUE;
		}
	};
};