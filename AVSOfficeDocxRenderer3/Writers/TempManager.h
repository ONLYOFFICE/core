#pragma once
#include "../stdafx.h"
#include <time.h>

namespace NSCommon
{
	class CTempManager
	{
	public:
		CString m_strTempDirectory;
		CString m_strTempImagesDirectory;

	public:
		CTempManager()
		{
			m_strTempDirectory			= _T("");
			m_strTempImagesDirectory	= _T("");
		}
		~CTempManager()
		{
		}

	public:
		void InitTempDirectory()
		{
			if (_T("") == m_strTempDirectory)
			{
				CStringW sTempPath;
				GetTempPathW(4096, sTempPath.GetBuffer(4096));
				sTempPath.ReleaseBuffer();

				m_strTempDirectory = sTempPath;
			}	

			srand( (unsigned)time(NULL) );
			CString sNewDirName;	
			do
			{
				int nRandom = rand() % 100000000;
				CString sRandName;
				sRandName.Format( _T("AVSDocxTemp%d"), nRandom );
				sNewDirName = m_strTempDirectory + _T("\\") + sRandName;
			}
			while( INVALID_FILE_ATTRIBUTES != ::GetFileAttributes( sNewDirName ) );

			m_strTempDirectory = sNewDirName;
			m_strTempImagesDirectory = m_strTempDirectory + _T("\\media");

			::CreateDirectory(m_strTempDirectory, NULL);
			::CreateDirectory(m_strTempImagesDirectory, NULL);
		}

		void ZipDirectory(CString strDstFile)
		{
			AVSOfficeUtils::IOfficeUtils* pOfficeUtils = NULL;
			if (S_OK != CoCreateInstance(__uuidof(AVSOfficeUtils::COfficeUtils), NULL, CLSCTX_INPROC_SERVER, __uuidof(AVSOfficeUtils::IOfficeUtils),(void**)&pOfficeUtils))
				return;

			BSTR bstrSrcPath = m_strTempDirectory.AllocSysString();
			BSTR bstrDstPath = strDstFile.AllocSysString();
			pOfficeUtils->CompressFileOrDirectory( bstrSrcPath, bstrDstPath, -1 );
			SysFreeString(bstrSrcPath);
			SysFreeString(bstrDstPath);
			RELEASEINTERFACE(pOfficeUtils);
		}

		void RemoveTempDirectory()
		{
			if ( _T("") == m_strTempDirectory )
				return;
		
			SHFILEOPSTRUCTW shfos;
			ZeroMemory(&shfos, sizeof(shfos));
			shfos.wFunc = FO_DELETE;
			CStringW _local = m_strTempDirectory + L"*.*";
			_local.AppendChar(0);
			_local.AppendChar(0);
			shfos.pFrom = _local.GetString();
			shfos.fFlags = FOF_SILENT + FOF_NOCONFIRMATION;

			SHFileOperationW(&shfos);
			RemoveDirectory(m_strTempDirectory);

			m_strTempDirectory = _T("");
		}
	};
}