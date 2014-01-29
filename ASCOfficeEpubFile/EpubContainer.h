#pragma once
#include "EpubPackage.h"

class EpubContainer
{
public: EpubPackage m_oEpubPackage;
public: int Load( CString sFolderPath )
		{
			CString sContainer = sFolderPath + _T("\\META-INF\\container.xml");
			XmlUtils::CXmlReader oXmlReader;
			oXmlReader.SetProperty(CString(_T("SelectionNamespaces")), CString(_T("xmlns:main=\"urn:oasis:names:tc:opendocument:xmlns:container\"")));
			if( TRUE == oXmlReader.OpenFromFile( sContainer ) )
				if( TRUE == oXmlReader.ReadRootNode( _T("main:container") ) )
					if( TRUE == oXmlReader.ReadNode( _T("main:rootfiles") ) )
						if( TRUE == oXmlReader.ReadNodeList( _T("main:rootfile") ) )
						{
							for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
							{
								CString sMime = oXmlReader.ReadNodeAttribute( i, _T("media-type") );
								CString sPackagePath = oXmlReader.ReadNodeAttribute( i, _T("full-path") );
								if( _T("") != sPackagePath && _T("application/oebps-package+xml") == sMime )
									return m_oEpubPackage.Load( sFolderPath + _T("\\") + sPackagePath );
							}
						}
			return -1;
		}
public: int Create( CString sFolderPath )
		{
			HANDLE hFile = NULL;
			DWORD dwBetysWritten = 0;
			//создаем mime файл
			hFile = ::CreateFile( sFolderPath + _T("\\mimetype"), GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);
			if( INVALID_HANDLE_VALUE == hFile )
				return -1;
			CStringA sMime = "application/epub+zip";
			::WriteFile( hFile, sMime, sMime.GetLength(), &dwBetysWritten, NULL );	
			RELEASEHANDLE( hFile );

			//создаем META-INF
			CreateDirectory( sFolderPath + _T("\\META-INF"), NULL );
			//container.xml
			hFile = ::CreateFile( sFolderPath + _T("\\META-INF\\container.xml"), GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);
			if( INVALID_HANDLE_VALUE == hFile )
				return -1;
			CStringA sContainer = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			sContainer.Append( "<container xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\" version=\"1.0\"><rootfiles><rootfile full-path=\"OPS/content.opf\" media-type=\"application/oebps-package+xml\"/></rootfiles></container>" );
			::WriteFile( hFile, sContainer, sContainer.GetLength(), &dwBetysWritten, NULL );	
			RELEASEHANDLE( hFile );

			//создаем папку для package
			CreateDirectory( sFolderPath + _T("\\OPS"), NULL );
			return m_oEpubPackage.Create( sFolderPath + _T("\\OPS\\"), _T("OPS/") );
		}
public: void Save( CString sFolderPath )
		{
			m_oEpubPackage.Save();
			//encryption.xml
			if( m_oEpubPackage.m_aCrypted.GetCount() > 0 )
			{
				HANDLE hFile = NULL;
				DWORD dwBetysWritten = 0;
				hFile = ::CreateFile( sFolderPath + _T("\\META-INF\\encryption.xml"), GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);
				if( INVALID_HANDLE_VALUE == hFile )
					return ;
				CString sEncryption = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
				sEncryption.Append( _T("<encryption xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">") );
				for( int i = 0; i < (int)m_oEpubPackage.m_aCrypted.GetCount(); i++ )
					sEncryption.Append( m_oEpubPackage.m_aCrypted[i] );
				sEncryption.Append( _T("</encryption>") );
				//переводим в utf-8
				CStringA sContentUtf;
				int nBufSize = WideCharToMultiByte(CP_UTF8, 0, sEncryption, -1, NULL, NULL, NULL, NULL);
				LPSTR p = sContentUtf.GetBuffer( nBufSize );
				WideCharToMultiByte(CP_UTF8, 0, sEncryption, -1, p, nBufSize, NULL, NULL);
				sContentUtf.ReleaseBuffer();

				::WriteFile( hFile, sContentUtf, sContentUtf.GetLength(), &dwBetysWritten, NULL );	
				RELEASEHANDLE( hFile );
			}

		}
};