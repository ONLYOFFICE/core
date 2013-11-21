#pragma once
#include "OOXColorSchemeReader.h"
#include "OOXFontSchemeReader.h"
#include "Utils.h"
#include "../Common/ATLDefine.h"

	class OOXThemeReader
	{
	private: CString m_sFileName;

	public: OOXThemeReader( CString sFileName )
			{
				m_sFileName = sFileName;
			}
	public: bool Parse( ReaderParameter oParam )
			{
				//CreateRtfZip( m_sFileName, oParam.oDocument->m_sThemeData );

				XmlUtils::CXmlReader oXmlReader;
				oXmlReader.OpenFromFile( m_sFileName );
				oXmlReader.ReadRootNode( _T("a:theme") );
				oXmlReader.ReadNode( _T("a:themeElements") );
				oXmlReader.ReadNodeList( _T("*") );
				for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
				{
					CString sNodeName = oXmlReader.ReadNodeName(i);
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode(i,oNode);
					ReaderParameter oNewParam = oParam;
					oNewParam.oNode = oNode;
					if( _T("a:clrScheme") == sNodeName )
					{
						OOXColorSchemeReader oColorSchemeReader;
						oColorSchemeReader.Parse( oNewParam );
					}
					else if( _T("a:fontScheme") == sNodeName )
					{
						OOXFontSchemeReader oFontSchemeReader;
						oFontSchemeReader.Parse( oNewParam );
					}
				}
				return true;
			}
	private: bool CreateRtfZip( CString sThemeFilename, CString& sOutput )
			{
			////возвращаем тип по расширению
			//TCHAR achrExt[256];
			//_tsplitpath( sFileName,   NULL,NULL,NULL, achrExt );

			//CString sExtention(achrExt);
			//sExtention.MakeLower();
				CString sThemeName = sThemeFilename.Right( sThemeFilename.GetLength() - sThemeFilename.ReverseFind('\\') - 1 );
				CString sTempDirectory = sThemeFilename + _T(".folder");
				CString sTempZip = sThemeFilename + _T(".zip");
				//создаем директорию
				CreateDirectory( sTempDirectory, NULL );

				CStringA sToWrite;
				//создаем наполнение каталога

				DWORD dwBytesWritten;
				//ContentTypes
				CString sContentTypes = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\"><Override PartName=\"/theme/theme/themeManager.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.themeManager+xml\"/><Override PartName=\"/theme/theme/"+sThemeName+"\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/><Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/><Default Extension=\"xml\" ContentType=\"application/xml\"/></Types>");
				HANDLE hFile = ::CreateFile( sTempDirectory + _T("\\[Content_Types].xml"), GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
				sToWrite = Convert::UnicodeToUtf8( sContentTypes );
				::WriteFile(hFile,sToWrite ,sToWrite.GetLength(), &dwBytesWritten, NULL);	
				CloseHandle( hFile );

				//rels
				CreateDirectory( sTempDirectory + _T("\\_rels"), NULL );
				CString sRels = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
				sRels.Append( _T("\n") );
				sRels.Append( _T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"theme/theme/themeManager.xml\"/></Relationships>") );
				hFile = ::CreateFile(  sTempDirectory + _T("\\_rels\\.rels"), GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
				sToWrite = Convert::UnicodeToUtf8( sRels );
				::WriteFile(hFile,sToWrite ,sToWrite.GetLength(), &dwBytesWritten, NULL);	
				CloseHandle( hFile );

				//ThemeManager
				CreateDirectory( sTempDirectory + _T("\\theme"), NULL );
				CreateDirectory( sTempDirectory + _T("\\theme\\theme"), NULL );
				CString sThemeManager = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			   	sThemeManager.Append( _T("\n") );
				sThemeManager.Append( _T("<a:themeManager xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"/>") );
				hFile = ::CreateFile(  sTempDirectory + _T("\\theme\\theme\\themeManager.xml"), GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
				sToWrite = Convert::UnicodeToUtf8( sThemeManager );
				::WriteFile(hFile,sToWrite ,sToWrite.GetLength(), &dwBytesWritten, NULL);	
				CloseHandle( hFile );

				//ThemeManagerRels
				CreateDirectory( sTempDirectory + _T("\\theme\\theme\\_rels"), NULL );
				CString sThemeManagerRels =  _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
				sThemeManagerRels.Append( _T("\n") );
				sThemeManagerRels.Append( _T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\""+sThemeName+"\"/></Relationships>" ));
				hFile = ::CreateFile(  sTempDirectory + _T("\\theme\\theme\\_rels\\themeManager.xml.rels"), GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
				sToWrite = Convert::UnicodeToUtf8( sThemeManagerRels );
				::WriteFile(hFile,sToWrite ,sToWrite.GetLength(), &dwBytesWritten, NULL);	
				CloseHandle( hFile );

				//копируем theme*.xml
				CopyFile( sThemeFilename, sTempDirectory + _T("\\theme\\theme\\") + sThemeName ,FALSE );

				//«апаковываем архив
				AVSOfficeUtils::IOfficeUtils* piZipUtil = NULL;
				HRESULT hRes = CoCreateInstance( AVSOfficeUtils::CLSID_COfficeUtils , NULL, CLSCTX_INPROC, AVSOfficeUtils::IID_IOfficeUtils, (void**)(&piZipUtil));
				if( FAILED( hRes ) || NULL == piZipUtil )
				 return false; 
				BSTR bstrZipFile = sTempZip.AllocSysString();
				BSTR bstrUnZipDirectory = sTempDirectory.AllocSysString();
				BSTR bstrPassword = NULL;
				piZipUtil->CompressFileOrDirectory(bstrUnZipDirectory, bstrZipFile, 0);
				SysFreeString( bstrZipFile );
				SysFreeString( bstrUnZipDirectory );
				RELEASEINTERFACE( piZipUtil );

				//переводим архив в строку
				RtfUtility::ReadDataFromFile( sTempZip, sOutput );

				//удал€ем все временные файлы в папке
				int nResult = Utils::RemoveDirOrFile( sTempDirectory );
				nResult = Utils::RemoveDirOrFile( sTempZip );
				return true;
			}
	};
