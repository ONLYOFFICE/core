// CAVSOfficeOfficeEpubFile.cpp : Implementation of CAVSOfficeOfficeEpubFile

#include "stdafx.h"
#include "COfficeEpubFile.h"
#include "EpubContainer.h"
#include "EventHandler.h"
#include "StringUtils.h"
#include "../Common/OfficeFileErrorDescription.h"
#include "../Common/OfficeFileFormats.h"
#include "../Common/OfficeDefines.h"
#include "../AVSOfficeUniversalConverter/Utils.h"
#include "../../AVSVideoStudio3/Common/MediaFormatDefine.h"

#define ZLIB_WINAPI
#define CODEPAGE_ISSUE_FIX
#include "..\AVSOfficeUtils\ZLIB\zlib-1.2.3\contrib\minizip\zip.h"

#define ONE_INCH 2.54

// CAVSOfficeOfficeEpubFile
class FolderRemove
{
private: CString m_sFolder;
public: FolderRemove(CString sFolder )
	{
		m_sFolder = sFolder;
	}
public: ~FolderRemove( )
	{
		Utils::RemoveDirOrFile( m_sFolder );
	}
};
// bEnd == true (делает чтобы в конце был '/' или '\'), bEnd == false (делает чтобы в начале не было '/' или '\')
void NormalizePath( CString& sPath, bool bEnd = true )
		 {
			 int nPathLen = sPath.GetLength();
			 if( nPathLen > 0 ) // sPath должен заканчиваться '/' или '\'
			 {
				 if( true == bEnd )
				 {
					 TCHAR tcLastChar = sPath[ nPathLen - 1 ];
					 if( '\\' != tcLastChar && '/' != tcLastChar )
						 sPath.AppendChar( '/' );
				 }
				 else
				 {
					 TCHAR tcFirstChar = sPath[ 0 ];
					 if( '\\' == tcFirstChar || '/' == tcFirstChar )
						 sPath = sPath.Right( sPath.GetLength() - 1 );
				 }
			 }
		 }
void MergeRels( CString sDest, CString sSource, CString sDestDir, CString sSourceDir, CString& sSourceDoc )
{
	int nImageCounter = 1;
	int nStartRelsIndex = 0;
	//пробегаемся по document.xml.rels чтобы вычислить nStartRelsIndex
	XmlUtils::CXmlReader oXmlReader;
	if( ::GetFileAttributes( sDest ) != INVALID_FILE_ATTRIBUTES )
		if( TRUE == oXmlReader.OpenFromFile( sDest ) )
			if( TRUE == oXmlReader.ReadRootNode( _T("Relationships") ) )
				if( TRUE == oXmlReader.ReadNodeList( _T("Relationship") ) )
				{
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sCurRID = oXmlReader.ReadNodeAttribute( i , _T("Id") );
						if( 0 == sCurRID.Find( _T("rId") ) )
						{
							//ищем число за rId
							int nStartIndex = 3;// strlen("rId");
							CString sDigitAfter = _T("");
							int nCurRIDLen = sCurRID.GetLength();
							while( nStartIndex < nCurRIDLen && _istdigit( sCurRID[nStartIndex] ) )
							{
								sDigitAfter.AppendChar( sCurRID[nStartIndex] );
								nStartIndex++;
							}
							if( _T("") != sDigitAfter )
							{
								int nDigitAfter = Strings::ToInteger( sDigitAfter );
								if( nDigitAfter >= nStartRelsIndex )
									nStartRelsIndex = nDigitAfter + 1;
							}
						}
					}
				}
	if( ::GetFileAttributes( sSource ) != INVALID_FILE_ATTRIBUTES )
		if( TRUE == oXmlReader.OpenFromFile( sSource ) )
			if( TRUE == oXmlReader.ReadRootNode( _T("Relationships") ) )
				if( TRUE == oXmlReader.ReadNodeList( _T("Relationship") ) )
				{
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sCurRID = oXmlReader.ReadNodeAttribute( i , _T("Id") );
						if( 0 == sCurRID.Find( _T("rId") ) )
						{
							//ищем число за rId
							int nStartIndex = 3;// strlen("rId");
							CString sDigitAfter = _T("");
							int nCurRIDLen = sCurRID.GetLength();
							while( nStartIndex < nCurRIDLen && _istdigit( sCurRID[nStartIndex] ) )
							{
								sDigitAfter.AppendChar( sCurRID[nStartIndex] );
								nStartIndex++;
							}
							if( _T("") != sDigitAfter )
							{
								int nDigitAfter = Strings::ToInteger( sDigitAfter );
								if( nDigitAfter >= nStartRelsIndex )
									nStartRelsIndex = nDigitAfter + 1;
							}
						}
					}
				}
	nStartRelsIndex++;
	//читаем из rels только Image и Hyperlink
	CString sAdditionalRels = _T("");
	if( ::GetFileAttributes( sSource ) != INVALID_FILE_ATTRIBUTES )
		if( TRUE == oXmlReader.OpenFromFile( sSource ) )
			if( TRUE == oXmlReader.ReadRootNode( _T("Relationships") ) )
				if( TRUE == oXmlReader.ReadNodeList( _T("Relationship") ) )
				{
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sCurType = oXmlReader.ReadNodeAttribute( i , _T("Type") );
						CString sCurTarget = oXmlReader.ReadNodeAttribute( i , _T("Target") );
						CString sTargetMode = oXmlReader.ReadNodeAttribute( i , _T("TargetMode") );
						CString sCurRID = oXmlReader.ReadNodeAttribute( i , _T("Id") );
						bool bImage = (-1 != sCurType.Find( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image") ));
						bool bHyp = (-1 != sCurType.Find( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink") ));
						if( bImage || bHyp )
						{
							CString sNewName = _T("");
							if( bImage && "External" != sTargetMode )
							{
								//копируем картинку
								NormalizePath( sCurTarget, false );
								if( ::GetFileAttributes( sDestDir + _T("media") ) == INVALID_FILE_ATTRIBUTES )
									CreateDirectory( sDestDir + _T("media"), NULL );
								CString sDestLocation = sDestDir + sCurTarget;
								//создаем новое имя
								TCHAR tDrive[256];
								TCHAR tDir[256];
								TCHAR tFilename[256];
								TCHAR tExt[256];
								_tsplitpath( sDestLocation, tDrive, tDir, tFilename, tExt );
								CString sNewFilename;
								while ( ::GetFileAttributes( sDestLocation ) != INVALID_FILE_ATTRIBUTES)
								{
									sNewFilename = _T("");sNewFilename.AppendFormat( CString(_T("image%d")) + tExt, nImageCounter++ );
									sDestLocation = CString(tDrive) + CString(tDir) + sNewFilename;
								}
								sNewName = sCurTarget;
								if( _T("") != sNewFilename )
									sNewName.Replace( CString(tFilename) + CString(tExt), sNewFilename );
								//Копируем
								CopyFile( sSourceDir + sCurTarget, sDestLocation, FALSE );
							}
							CString sCurXml = oXmlReader.ReadNodeXml( i );
							CString sNewRID;sNewRID.AppendFormat( _T("\"rId%d\""), nStartRelsIndex++ );
							sCurXml.Replace( CString(_T("\"")) + sCurRID + _T("\""), sNewRID );
							sSourceDoc.Replace( CString( _T("\"")) + sCurRID + _T("\""), sNewRID );
							if( _T("") != sNewName )
								sCurXml.Replace( sCurTarget, sNewName );
							sAdditionalRels += sCurXml;
						}
					}
				}
	//добавляем
	if( _T("") != sAdditionalRels )
	{
		CString sAddition = _T("<root xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">") + sAdditionalRels +_T("</root>");
		XML::IXMLDOMDocumentPtr	m_pXmlDocument;
		m_pXmlDocument.CreateInstance(CLSID_DOMDocument);
		if( NULL != m_pXmlDocument )
		{
			VARIANT vtLoad;
			vtLoad.vt = VT_BSTR;
			vtLoad.bstrVal = sDest.AllocSysString();

			try{
				if( VARIANT_TRUE == m_pXmlDocument->load( vtLoad ) )
				{
					XML::IXMLDOMNodePtr oRootNode = m_pXmlDocument->selectSingleNode( _T("Relationships") );
					if( NULL != oRootNode )
					{
						XmlUtils::CXmlReader oRelationReader;
						if( TRUE == oRelationReader.OpenFromXmlString( sAddition ) )
							if( TRUE == oRelationReader.ReadNodeList( _T("*") ) )
							{
								int nLengthList = oRelationReader.GetLengthList();
								for( int i = 0; i < nLengthList ; i++ )
								{
									XML::IXMLDOMNodePtr oCurNode = NULL;
									oRelationReader.GetNode( i, oCurNode );
									if( NULL != oCurNode )
										XML::IXMLDOMNodePtr pBuf = oRootNode->appendChild( oCurNode );
								}
							}
							VARIANT vtSave;
							vtSave.vt = VT_BSTR;
							vtSave.bstrVal = sDest.AllocSysString();
							m_pXmlDocument->save( vtSave );
							SysFreeString( vtSave.bstrVal );
					}
				}
			}
			catch( ... )
			{
			}
			SysFreeString( vtLoad.bstrVal );
		}
	}
}
//мерж основан на тех docx что приходят из html:
//fontTable.xml, styles.xml стандартные их мержить не надо.
//картинки заданы тегами 
//numbering.xml надо мержить
void MergeDocx( CString sDest, CString sSource )
{
	NormalizePath( sSource, true );
	NormalizePath( sDest, true );
	//находим document.xml из [Content_Types].xml
	CString sDestDocument;
	CString sSourceDocument;
	XmlUtils::CXmlReader oXmlReader;
	if( TRUE == oXmlReader.OpenFromFile( sDest + _T("[Content_Types].xml") ) )
		if( TRUE == oXmlReader.ReadRootNode( _T("Types") ) )
			if( TRUE == oXmlReader.ReadNodeList( _T("Override") ) )
				for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
				{
					CString sXml = oXmlReader.ReadNodeAttribute( i, _T("ContentType") );
					if( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml") == sXml )
					{
						sDestDocument = oXmlReader.ReadNodeAttribute( i, _T("PartName") );
						NormalizePath( sDestDocument, false );
					}
				}
	if( TRUE == oXmlReader.OpenFromFile( sSource + _T("[Content_Types].xml") ) )
		if( TRUE == oXmlReader.ReadRootNode( _T("Types") ) )
			if( TRUE == oXmlReader.ReadNodeList( _T("Override") ) )
				for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
				{
					if( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml") == oXmlReader.ReadNodeAttribute( i, _T("ContentType") ) )
					{
						sSourceDocument = oXmlReader.ReadNodeAttribute( i, _T("PartName") );
						NormalizePath( sSourceDocument, false );
					}
				}
	if( _T("") == sDestDocument || _T("") == sSourceDocument )
		return;
	sDestDocument = sDest + sDestDocument;
	sSourceDocument = sSource + sSourceDocument;

	//соединяем документы
	//читаем документ для вставки в строку
	CString sSourceContent;
	if( TRUE == oXmlReader.OpenFromFile( sSourceDocument ) )
		if( TRUE == oXmlReader.ReadRootNode( _T("w:document") ) )
			sSourceContent = oXmlReader.ReadNodeXml();
	if( _T("") == sSourceContent )
		return;

	//ищем пусти к rels
	TCHAR tDrive[256];
	TCHAR tDir[256];
	TCHAR tFilename[256];
	TCHAR tExt[256];
	_tsplitpath( sDestDocument, tDrive, tDir, tFilename, tExt );
	CString sDestDir = CString(tDrive) + CString(tDir);
	CString sDestRels = sDestDir + _T("_rels\\") +CString(tFilename) + CString(tExt) + _T(".rels");

	_tsplitpath( sSourceDocument, tDrive, tDir, tFilename, tExt );
	CString sSourceDir = CString(tDrive) + CString(tDir);
	CString sSourceRels = sSourceDir + _T("_rels\\") +CString(tFilename) + CString(tExt) + _T(".rels");

	//мержим rels
	MergeRels( sDestRels, sSourceRels, sDestDir, sSourceDir, sSourceContent );

	std::wstring sStdSource = sSourceContent;
	//убираем numbering, потому что пока не мержим numbering.xml
	boost::wregex xRegEx(_T("<w\\:numPr[ >].*?</w\\:numPr>") );
	sStdSource = boost::regex_replace( sStdSource, xRegEx, _T("") );

	XML::IXMLDOMDocumentPtr	pXmlDocument;
	pXmlDocument.CreateInstance(CLSID_DOMDocument);
	if( NULL != pXmlDocument )
	{
		VARIANT vtLoad;
		vtLoad.vt = VT_BSTR;
		vtLoad.bstrVal = sDestDocument.AllocSysString();

		try{
			if( VARIANT_TRUE == pXmlDocument->load( vtLoad ) )
			{
				XML::IXMLDOMNodePtr oBodyNode = pXmlDocument->selectSingleNode( _T("w:document/w:body") );
				if( NULL != oBodyNode )
				{
					//Ищем w:sectPr, чтобы вставлять дополнительные ноды перед ним
					CComVariant varSectNode(&(*oBodyNode->GetlastChild()));

					//если документ не пустой вставляем разрывы страниц
					if( oBodyNode->GetchildNodes()->Getlength() > 1 )
					{
						CString sPageBreak = L"<root xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"><w:p><w:r><w:br w:type=\"page\"/></w:r></w:p></root>";
						XmlUtils::CXmlReader oPageBreakReader;
						if( TRUE == oPageBreakReader.OpenFromXmlString( sPageBreak ) )
							if( TRUE == oPageBreakReader.ReadNode( _T("w:p") ) )
							{
								XML::IXMLDOMNodePtr oCurNode = NULL;
								oPageBreakReader.GetNode( oCurNode );
								if( NULL != oCurNode )
									oBodyNode->insertBefore( oCurNode, varSectNode );
							}
					}

					XmlUtils::CXmlReader oRelationReader;
					if( TRUE == oRelationReader.OpenFromXmlString( sStdSource.c_str() ) )
						if( TRUE == oRelationReader.ReadRootNode( _T("w:document") ) )
							if( TRUE == oRelationReader.ReadNode( _T("w:body") ) )
								if( TRUE == oRelationReader.ReadNodeList( _T("*") ) )
								{
									int nLengthList = oRelationReader.GetLengthList();
									//nLengthList - 1, чтобы не вставлять w:sectPr
									for( int i = 0; i < nLengthList - 1 ; i++ )
									{
										XML::IXMLDOMNodePtr oCurNode = NULL;
										oRelationReader.GetNode( i, oCurNode );
										if( NULL != oCurNode )
											oBodyNode->insertBefore( oCurNode, varSectNode );
									}
								}
					varSectNode.Clear();

					VARIANT vtSave;
					vtSave.vt = VT_BSTR;
					vtSave.bstrVal = sDestDocument.AllocSysString();
					pXmlDocument->save( vtSave );
					SysFreeString( vtSave.bstrVal );
				}
			}
		}
		catch( ... )
		{
		}
		::SysFreeString( vtLoad.bstrVal );
	}


}

int _WriteFileToZip( CString sSource, CString sSourceRel, zipFile zf, int nCompressionLevel )
{
	int err = 0;
	if( Z_NO_COMPRESSION == nCompressionLevel )
		err = zipOpenNewFileInZip( zf, Utils::UnicodeToCodepage (sSourceRel, CP_OEMCP), NULL, NULL, 0, NULL, 0, NULL, NULL, nCompressionLevel );
	else
		err = zipOpenNewFileInZip( zf, Utils::UnicodeToCodepage (sSourceRel, CP_OEMCP), NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, nCompressionLevel );
	HANDLE hSourceFile = ::CreateFile( sSource, GENERIC_READ, 0, 0, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, 0);
	if( INVALID_HANDLE_VALUE == hSourceFile )
		err = -1;
	DWORD dwBytesRead = 1;
	BYTE bBuffer[ BUFSIZE ];
	while( -1 != err && 0 != dwBytesRead )
	{
		ReadFile( hSourceFile, bBuffer, BUFSIZE, &dwBytesRead, NULL );
		if( dwBytesRead > 0 )
			err = zipWriteInFileInZip( zf, bBuffer, dwBytesRead );
	}
	RELEASEHANDLE( hSourceFile );
	if( 0 == err )
		err = zipCloseFileInZip( zf );
	else
		zipCloseFileInZip( zf );
	return err;
}
int _CompressEpubDir( CString sSource, CString sPathRel, zipFile zf, int nCompresstionLevel )
{
	DWORD dwFileAttrib = ::GetFileAttributes( sSource );
	if(  dwFileAttrib != INVALID_FILE_ATTRIBUTES )
	{
		DWORD dwResult = 0;
		if( 0 != (FILE_ATTRIBUTE_DIRECTORY & dwFileAttrib) )
		{
			HANDLE Handle;
			WIN32_FIND_DATA FindData;
			DWORDLONG Result = 0;

			Handle = FindFirstFile( ( sSource + _T("\\*.*") ), &FindData );
			if ( Handle == INVALID_HANDLE_VALUE )
				return 0;
			do
			{
				int nRes = 0;
				if( ( CString( FindData.cFileName ) != _T("mimetype") ) && ( CString( FindData.cFileName ) != _T(".") ) && ( CString( FindData.cFileName ) != _T("..") ) )
				{
					if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
					{
						if( _T("fonts") == CString( FindData.cFileName ) )
							dwResult += _CompressEpubDir( sSource + _T("\\") + FindData.cFileName, sPathRel + FindData.cFileName + _T("/"), zf, Z_NO_COMPRESSION );//фонты и так уже запакованы
						else
							dwResult += _CompressEpubDir( sSource + _T("\\") + FindData.cFileName, sPathRel + FindData.cFileName + _T("/"), zf, nCompresstionLevel );
					}
					else
						dwResult += _CompressEpubDir( sSource + _T("\\") + FindData.cFileName, sPathRel, zf, nCompresstionLevel );
				}
			}
			while( FindNextFile( Handle, &FindData ) != 0 );
			FindClose( Handle );
		}
		else
		{
			TCHAR tExt[256];
			TCHAR tFilename[256];
			_wsplitpath( sSource, NULL, NULL, tFilename, tExt );
			CString sExt = tExt;
			CString sFilename = tFilename;
			
			_WriteFileToZip( sSource, sPathRel + sFilename + sExt, zf, nCompresstionLevel );
		}

		return dwResult;
	}// 0 - все закончилось хорошо
	return 0;
}
int CompressEpubDir( CString sSource, CString sDestination )
{
	int err = 0;
	zipFile zf = zipOpen( sDestination, APPEND_STATUS_CREATE );
	if( NULL == zf )
		return 1;

	//первый в архиве всегда должен быть mimetype
	CString sMimeFile = sSource + _T("\\mimetype");
	if( ::GetFileAttributes( sMimeFile ) != INVALID_FILE_ATTRIBUTES )
		err = _WriteFileToZip( sMimeFile, _T("mimetype"), zf, Z_NO_COMPRESSION );

	if( err == 0 )
	{
		//записываем все остальное
		err = _CompressEpubDir( sSource, _T(""), zf, Z_BEST_COMPRESSION );
	}
	if( 0 == err )
		err = zipClose( zf, NULL );
	else
		zipClose( zf, NULL );
	return err;
}

HRESULT COfficeEpubFile::OnProgressHtml( LONG nID, LONG nPercent )
{//прогресс от 0 до c_shMaxProgressPercent
	long nNewProgress = ( c_nMaxProgressPercent * m_nTasksComplete + nPercent ) / m_nTasksAll;
	OnProgress( nID, nNewProgress );
	return S_OK;
}
HRESULT COfficeEpubFile::OnProgressHtmlEx( LONG nID, LONG nPercent, SHORT* Cancel )
{
	long nNewProgress = ( c_nMaxProgressPercent * m_nTasksComplete + nPercent ) / m_nTasksAll;
	OnProgressEx( nID, nNewProgress, Cancel );
	if( c_shProgressCancel == *Cancel )
		m_bCancel = true;
	return S_OK;
}
STDMETHODIMP COfficeEpubFile::LoadFromFile( BSTR bstrSrcFileName,  BSTR bstrDstPath,  BSTR bstrXMLOptions)
{
	//проверяем входной файл на epub
	long nError;
	if( FALSE == IsEpubFile( CString(bstrSrcFileName), nError ) )
	{
		if( NOERROR == nError )
			return AVS_ERROR_FILEFORMAT;
		else
			return nError;
	}
	HRESULT hRes = S_OK;
	//создаем временную папку для распаковки архива
	CString sTempFolder = Utils::CreateTempDir( CString( bstrDstPath ) );
	FolderRemove oFolderRemove( sTempFolder );

	//распаковываем архив в папку
	OfficeUtils::IOfficeUtilsPtr piOfficeUtils;
	piOfficeUtils.CreateInstance( __uuidof(OfficeUtils::COfficeUtils) );
	if( NULL == piOfficeUtils )
		return AVS_ERROR_UNEXPECTED;
	BSTR bstrTempFolder = sTempFolder.AllocSysString();
	hRes = piOfficeUtils->ExtractToDirectory( bstrSrcFileName, bstrTempFolder, NULL, 0 );
	SysFreeString( bstrTempFolder );
	if( FAILED( hRes ) )
		return AVS_ERROR_UNEXPECTED;

	//load
	EpubContainer oEpubContainer;
	if( 0 != oEpubContainer.Load( sTempFolder ) )
		return AVS_ERROR_UNEXPECTED;
	m_sMetadata = oEpubContainer.m_oEpubPackage.m_sMetadata;
	OfficeHtmlFile::IHtmlFilePtr piHtmlFile;
	piHtmlFile.CreateInstance( __uuidof(OfficeHtmlFile::CHtmlFile) );
	if( NULL == piHtmlFile )
		return AVS_ERROR_UNEXPECTED;
	//подцепляемся к событиям
	COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents, COfficeEpubFile >* oHtmlEvents = new COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents, COfficeEpubFile >( this );
	oHtmlEvents->AddRef();
	oHtmlEvents->Advise( piHtmlFile );
	COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents2, COfficeEpubFile >* oHtmlEvents2 = new COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents2, COfficeEpubFile >( this );
	oHtmlEvents2->AddRef();
	oHtmlEvents2->Advise( piHtmlFile );
	//заначения для прогресса
	m_nTasksAll = oEpubContainer.m_oEpubPackage.GetCount();
	m_nTasksComplete = 0;
	if( oEpubContainer.m_oEpubPackage.GetCount() > 0 )
	{//конвертируем Html в docx и соединяем docx
		int nHtmlsCount = oEpubContainer.m_oEpubPackage.GetCount();
		CString sDstPath = CString( bstrDstPath );
		CString sTempHtml = Utils::CreateTempDir( bstrDstPath );
		FolderRemove oTempFolderRemove(sTempHtml);
		bool bFirst = true;
		for( int i = 0; i < nHtmlsCount; i++ )
		{
			CString sCurHtml = oEpubContainer.m_oEpubPackage.GetItem( i );
			BSTR bstrCurHtml = sCurHtml.AllocSysString();
			BSTR bstrDst;
			if( true == bFirst )
				bstrDst = sDstPath.AllocSysString();
			else
				bstrDst = sTempHtml.AllocSysString();
			CString sOptions;
			sOptions.AppendFormat( _T("<Options><FileType>%d</FileType><DefaultCodePage>%d</DefaultCodePage></Options>"), AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML, CP_UTF8 );
			BSTR bstrOptions = sOptions.AllocSysString();
			hRes = piHtmlFile->LoadFromFile( bstrCurHtml, bstrDst, bstrOptions );
			SysFreeString( bstrOptions );
			SysFreeString( bstrDst );
			SysFreeString( bstrCurHtml );
			if( true == m_bCancel )
				break;
			m_nTasksComplete++;
			if( S_OK != hRes )
				continue;
			if( true == bFirst )
				bFirst = false;
			else
			{
				MergeDocx( sDstPath, sTempHtml );
				Utils::RemoveDirOrFile( sTempHtml );
				CreateDirectory( sTempHtml, NULL );
			}
		}
	}
	//отцепляемся от событий
	oHtmlEvents->UnAdvise( piHtmlFile );
	RELEASEINTERFACE( oHtmlEvents );
	oHtmlEvents2->UnAdvise( piHtmlFile );
	RELEASEINTERFACE( oHtmlEvents2 );
	return hRes;
}
STDMETHODIMP COfficeEpubFile::SaveToFile( BSTR bstrDstFileName,  BSTR bstrSrcPath,  BSTR bstrXMLOptions)
{
	HRESULT hRes = S_OK;
	//создаем временную папку для формирования структуры epub
	CString sTempFolder = Utils::CreateTempDir( CString(bstrSrcPath) );
	FolderRemove oFolderRemove( sTempFolder );

	//создаем epub контейнер
	EpubContainer oEpubContainer;
	oEpubContainer.m_oEpubPackage.m_bEmbedFont = true;
	oEpubContainer.Create( sTempFolder );

	//Смотрим опции
	bool bEpubFromHtml = false;
	bool bSaveCover = true;
	bool bOnePageHtml = true;
	bool bLoadInCommandRenderer = false;
	CString sSourceHtmlPath = _T("");
	XmlUtils::CXmlReader oXmlOptionsReader;
	if( TRUE == oXmlOptionsReader.OpenFromXmlString( bstrXMLOptions ) )
	{
		XML::IXMLDOMNodePtr pXmlSubNode;
		if( TRUE ==oXmlOptionsReader.GetNode( pXmlSubNode ) )
		{
			XmlUtils::CXmlReader oXmlSubReader;
			if( TRUE == oXmlSubReader.OpenFromXmlNode( pXmlSubNode ) )
				if( TRUE == oXmlSubReader.ReadNode( _T("LoadInCommandRenderer") ) )
				{
					bLoadInCommandRenderer = true;
				}
		}
		if( TRUE == oXmlOptionsReader.ReadNode( _T("EpubOptions") ) )
		{
			CString sXml = oXmlOptionsReader.ReadNodeXml();

			CString sFontEncryption = oXmlOptionsReader.ReadNodeValue( _T("FontEncryption") );
			if( _T("") != sFontEncryption )
				oEpubContainer.m_oEpubPackage.m_nFontEncryption = (FontEncription)Strings::ToInteger( sFontEncryption );
		
			CString sFontEmbed = oXmlOptionsReader.ReadNodeValue( _T("FontEmbed") );sFontEmbed.MakeLower();
			if( _T("") != sFontEmbed )
			{
				if( _T("false") != sFontEmbed &&  _T("0") != sFontEmbed )
					oEpubContainer.m_oEpubPackage.m_bEmbedFont = false;
				else
					oEpubContainer.m_oEpubPackage.m_bEmbedFont = true;
			}

			CString sSaveCover = oXmlOptionsReader.ReadNodeValue( _T("SaveCover") );sSaveCover.MakeLower();
			if( _T("") != sSaveCover )
			{
				if( _T("false") != sSaveCover && _T("0") != sSaveCover )
					bSaveCover = false;
				else
					bSaveCover = true;
			}
			XML::IXMLDOMNodePtr pXmlSubNode;
			if( TRUE ==oXmlOptionsReader.GetNode( pXmlSubNode ) )
			{
				XmlUtils::CXmlReader oXmlSubReader;
				if( TRUE == oXmlSubReader.OpenFromXmlNode( pXmlSubNode ) )
					if( TRUE == oXmlSubReader.ReadNode( _T("OnePageHtml") ) )
					{
						CString sValue = oXmlSubReader.ReadNodeAttribute( _T("val") );
						sValue.MakeLower();
						if( _T("true") == sValue )
							bOnePageHtml = true;
						else if( _T("false") == sValue )
							bOnePageHtml = false;
					}
			}
			if( TRUE == oXmlOptionsReader.ReadNode( _T("NoStandartSave") ) )
				sSourceHtmlPath = oXmlOptionsReader.ReadNodeAttribute( _T("destinationpath") );
		}
	}
	CString sNewHtml = bstrSrcPath;
	//создаем IHtmlFile
	OfficeHtmlFile::IHtmlFilePtr piHtmlFile;
	piHtmlFile.CreateInstance( __uuidof(OfficeHtmlFile::CHtmlFile) );
	if( NULL == piHtmlFile )
		return AVS_ERROR_UNEXPECTED;

	//подцепляемся к событиям
	COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents, COfficeEpubFile >* oHtmlEvents = new COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents, COfficeEpubFile >( this );
	oHtmlEvents->AddRef();
	oHtmlEvents->Advise( piHtmlFile );
	COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents2, COfficeEpubFile >* oHtmlEvents2 = new COfficeHtmlEvent< OfficeHtmlFile::_IAVSOfficeFileTemplateEvents2, COfficeEpubFile >( this );
	oHtmlEvents2->AddRef();
	oHtmlEvents2->Advise( piHtmlFile );
	//заначения для прогресса
	m_nTasksAll = 2;
	m_nTasksComplete = 0;

	if( false == bEpubFromHtml )
	{
		if( true == bLoadInCommandRenderer && true == bOnePageHtml )
		{
			m_nTasksAll = 1;
			m_nTasksComplete = 0;

			long nPageCount = 0;
			m_piCommandsRenderer->get_PageCount( &nPageCount );
			DocxRenderer::IAVSDocxRendererPtr piDocxRenderer;
			piDocxRenderer.CreateInstance( __uuidof( DocxRenderer::CAVSDocxRenderer ) );
			if( NULL != piDocxRenderer )
			{
				for( int i = 0; i < nPageCount; i++ )
				{
					CString sCurTempDir = Utils::CreateTempDir( bstrSrcPath );

					CString sDocxRendererOptions = _T("<DocxRenderer");
					sDocxRendererOptions.AppendFormat( _T(" destinationpath=\"%s\""), sCurTempDir );
					sDocxRendererOptions.AppendFormat( _T(" textformatting=\"%d\""), TextFormatting::c_nWordProcessing );
					sDocxRendererOptions.Append( _T("/>") );
					//выставляем опции
					BSTR bstrDocxRendererOptions = sDocxRendererOptions.AllocSysString();
					piDocxRenderer->Initialize( bstrDocxRendererOptions );
					SysFreeString( bstrDocxRendererOptions );

					piDocxRenderer->NewPage();
					hRes = m_piCommandsRenderer->DrawPage( i, piDocxRenderer );
					if( FAILED( hRes ) )
						m_bCancel = true;

					if( false == m_bCancel )
					{
						hRes = piDocxRenderer->Save();
						if( FAILED( hRes ) )
							m_bCancel = true;
						if( false == m_bCancel )
						{
							OfficeHtmlFile::IHtmlFilePtr piCurHtmlFile;
							piCurHtmlFile.CreateInstance( __uuidof(OfficeHtmlFile::CHtmlFile) );
							//конвертируем docx в html
							sNewHtml = oEpubContainer.m_oEpubPackage.m_sPackegePath;
							sNewHtml.AppendFormat( _T("Chapter%d.html"), oEpubContainer.m_oEpubPackage.GetCount() + 1 );
							BSTR bstrDestHtml = sNewHtml.AllocSysString();
							CString sOptions;
							sOptions.AppendFormat( _T("<Options><FileType>%d</FileType><ConvertListOptions list2list=\"false\"/></Options>"), AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML );
							BSTR bstrDocxDir = sCurTempDir.AllocSysString();
							BSTR bstrOptions = sOptions.AllocSysString();
							hRes = piCurHtmlFile->SaveToFile( bstrDestHtml, bstrDocxDir, bstrOptions );
							SysFreeString( bstrDocxDir );
							SysFreeString( bstrOptions );
							SysFreeString( bstrDestHtml );
							if( FAILED( hRes ) )
								m_bCancel = true;
							//добавляем html
							oEpubContainer.m_oEpubPackage.AddHtml( sNewHtml );
						}
					}
					SHORT shCancel = c_shProgressContinue;
					OnProgressHtmlEx( 0, long( c_nMaxProgressPercent * ( 1.0 * ( i + 1 ) / nPageCount )), &shCancel );
					if( true == m_bCancel )
						break;					
					Utils::RemoveDirOrFile( sCurTempDir );
				}
			}
		}
		else
		{
			if( true == bLoadInCommandRenderer )
			{
				DocxRenderer::IAVSDocxRendererPtr piDocxRenderer;
				piDocxRenderer.CreateInstance( __uuidof( DocxRenderer::CAVSDocxRenderer ) );
				if( NULL != piDocxRenderer )
				{
					CString sDocxRendererOptions = _T("<DocxRenderer");
					sDocxRendererOptions.AppendFormat( _T(" destinationpath=\"%s\""), bstrSrcPath );
					sDocxRendererOptions.AppendFormat( _T(" textformatting=\"%d\""), TextFormatting::c_nWordProcessing );
					sDocxRendererOptions.Append( _T("/>") );
					//выставляем опции
					BSTR bstrDocxRendererOptions = sDocxRendererOptions.AllocSysString();
					piDocxRenderer->Initialize( bstrDocxRendererOptions );
					SysFreeString( bstrDocxRendererOptions );

					HRESULT hRes = S_OK;
					long nPageCount = 0;
					m_piCommandsRenderer->get_PageCount( &nPageCount );
					for( int i = 0; i < nPageCount; i++ )
					{
						piDocxRenderer->NewPage();
						hRes = m_piCommandsRenderer->DrawPage( i, piDocxRenderer );
						if( FAILED( hRes ) )
							m_bCancel = true;
						SHORT shCancel = c_shProgressContinue;
						OnProgressHtmlEx( 0, long( c_nMaxProgressPercent * ( 1.0 * ( i + 1 ) / nPageCount )), &shCancel );
						if( c_shProgressContinue != shCancel )
							m_bCancel = true;
						if( true == m_bCancel )
							break;
					}
					if( false == m_bCancel )
					{
						hRes = piDocxRenderer->Save();
						if( FAILED( hRes ) )
							m_bCancel = true;
					}
				}
			}
		m_nTasksComplete = 1;
		if( false == m_bCancel )
		{
		//конвертируем docx в html
		sNewHtml = oEpubContainer.m_oEpubPackage.m_sPackegePath;
		sNewHtml.AppendFormat( _T("Chapter%d.html"), oEpubContainer.m_oEpubPackage.GetCount() + 1 );
		BSTR bstrDestHtml = sNewHtml.AllocSysString();
		CString sOptions;
		sOptions.AppendFormat( _T("<Options><FileType>%d</FileType></Options>"), AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML );
		BSTR bstrOptions = sOptions.AllocSysString();
		hRes = piHtmlFile->SaveToFile( bstrDestHtml, bstrSrcPath, bstrOptions );
		SysFreeString( bstrOptions );
		SysFreeString( bstrDestHtml );

		//добавляем html
		oEpubContainer.m_oEpubPackage.AddHtml( sNewHtml );
		}
		}
	}
	else
	{
		//получаем имя файла.
		TCHAR tExt[256];
		TCHAR tFilename[256];
		TCHAR tFolder[256];
		TCHAR tDrive[256];
		_tsplitpath( CString(bstrSrcPath), tDrive, tFolder, tFilename, tExt );
		CString sExt = CString(tExt);
		CString sFilename = CString(tFilename);
		CString sFolder = CString(tFolder);
		CString sDrive = CString(tDrive);

		//конвертируем html в xhtml
		BSTR bstrSourceHtml = sSourceHtmlPath.AllocSysString();
		sNewHtml = oEpubContainer.m_oEpubPackage.m_sPackegePath;
		sNewHtml.AppendFormat( _T("Chapter%d.html"), oEpubContainer.m_oEpubPackage.GetCount() + 1 );
		BSTR bstrDestHtml = sNewHtml.AllocSysString();
		CString sOptions;sOptions.AppendFormat( _T("<Options><FileType>%d</FileType><NoStandartOpen destinationtype=\"%d\" destinationpath=\"%s\"/></Options>"), AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML, AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML, sNewHtml );
		BSTR bstrOptions = sOptions.AllocSysString();
		hRes = piHtmlFile->LoadFromFile( bstrSourceHtml, bstrDestHtml, bstrOptions );
		SysFreeString( bstrOptions );
		SysFreeString( bstrDestHtml );
		SysFreeString( bstrSourceHtml );

		//копируем сопутствующую папку
		CString sSourceHtmlDir  = sDrive + sFolder + sFilename + _T("_files/");
		CString sDestHtmlDir ; sDestHtmlDir.AppendFormat( oEpubContainer.m_oEpubPackage.m_sPackegePath + _T("Chapter%d_files"), oEpubContainer.m_oEpubPackage.GetCount() + 1 );
		Utils::CopyDirOrFile( sSourceHtmlDir, sDestHtmlDir );
	}
	if( FAILED( hRes ) )
		return hRes;
	//отцепляемся от событий
	oHtmlEvents->UnAdvise( piHtmlFile );
	RELEASEINTERFACE( oHtmlEvents );
	oHtmlEvents2->UnAdvise( piHtmlFile );
	RELEASEINTERFACE( oHtmlEvents2 );

	if( true == m_bCancel )
		return S_OK;
	//Title по имени
	TCHAR tFilename[256];
	_tsplitpath( CString(bstrDstFileName), NULL, NULL, tFilename, NULL );
	CString sFilename = CString(tFilename);
	oEpubContainer.m_oEpubPackage.m_sTitle = sFilename;
	oEpubContainer.m_oEpubPackage.m_sAuthor = _T("Unknown");
	//добавляем титульник
	if( true == bSaveCover && true == SaveCover( oEpubContainer.m_oEpubPackage.m_sPackegePath + _T("cover.jpeg"), oEpubContainer.m_oEpubPackage.m_nCoverWidth, oEpubContainer.m_oEpubPackage.m_nCoverHeight ) )
		oEpubContainer.m_oEpubPackage.m_sCover = _T("cover.jpeg");

	//сохраняем epub
	oEpubContainer.Save( sTempFolder );

	//запаковываем epub файл
	CompressEpubDir( sTempFolder, bstrDstFileName );
	return hRes;
}

STDMETHODIMP COfficeEpubFile::SetAdditionalParam( BSTR ParamName, VARIANT ParamValue)
{
	return S_OK;
}
STDMETHODIMP COfficeEpubFile::GetAdditionalParam( BSTR ParamName, VARIANT* ParamValue)
{
	CComBSTR ParamStr = ParamName;
	if (ParamStr == L"Metadata")
	{
		(*ParamValue).vt = VT_BSTR;
		(*ParamValue).bstrVal = m_sMetadata.AllocSysString();
	}
	return S_OK;
}
STDMETHODIMP COfficeEpubFile::get_CommandRenderer(IUnknown** ppunkRend)
{
	if(NULL == ppunkRend)
		return S_FALSE;
	(*ppunkRend) = NULL;
	if(NULL == m_piCommandsRenderer)
		return S_OK;
	return m_piCommandsRenderer->QueryInterface(__uuidof(AVSGraphics::IASCDocumentRenderer), (void**)&ppunkRend);
}


STDMETHODIMP COfficeEpubFile::put_CommandRenderer(IUnknown* ppunkRend)
{
	RELEASEINTERFACE(m_piCommandsRenderer);
	if(NULL != ppunkRend)
		ppunkRend->QueryInterface(__uuidof( AVSGraphics::IASCDocumentRenderer), (void**)&m_piCommandsRenderer);
	return S_OK;
}


BOOL COfficeEpubFile::IsEpubFile(CString sFilename, long& nError )
{
	nError = NOERROR;
	BOOL bResult = FALSE;

	OfficeUtils::IOfficeUtils2Ptr piOfficeUtils;
	piOfficeUtils.CreateInstance( __uuidof(OfficeUtils::COfficeUtils) );
	if( NULL == piOfficeUtils )
	{
		nError = AVS_ERROR_UNEXPECTED;
		return FALSE;
	}

	BYTE* pBuffer;
	const char *odtFormatLine = "application/epub+zip";			 

	BSTR bstrFilePath = sFilename.AllocSysString();
	HRESULT hRes = piOfficeUtils->LoadFileFromArchive(bstrFilePath, L"mimetype", &pBuffer);
	SysFreeString( bstrFilePath );
	if ( hRes == S_OK && pBuffer != NULL )
	{
		if (strstr((char*)pBuffer, odtFormatLine))
			bResult = TRUE;
	}
	RELEASEHEAP(pBuffer);	 

	return bResult;
}
bool COfficeEpubFile::SaveCover( CString sFilename, long& nWidth, long& nHeight )
{
	if( NULL != m_piCommandsRenderer )
	{
		long nPageCount = 0;
		m_piCommandsRenderer->get_PageCount( &nPageCount );
		if( nPageCount > 0 )
		{
			double dWidthMM = 0;
			double dHeightMM = 0;
			m_piCommandsRenderer->GetPageSize( 0, &dWidthMM, &dHeightMM );
			//
			if( 0 != dHeightMM )
			{
				nHeight = 1186;
				nWidth = nHeight * dWidthMM / dHeightMM;
			}
			AVSMediaCore3::IAVSUncompressedVideoFrame* piImage = NULL;
			AVSGraphics::IASCGraphicsRenderer* piGraphicsRenderer = NULL;
			UpdateGdiPlusRenderer( dWidthMM, dHeightMM, &piImage, &piGraphicsRenderer );
			if( NULL != piGraphicsRenderer )
			{
				piGraphicsRenderer->NewPage();
				m_piCommandsRenderer->DrawPage( 0, piGraphicsRenderer );
				//сохраняем файл
				ImageFile::IImageFile3Ptr piImageFile = NULL;
				piImageFile.CreateInstance( __uuidof(ImageFile::ImageFile3) );
				if( NULL != piImageFile )
				{
					VARIANT_BOOL vbSuccess = VARIANT_TRUE;
					BSTR bstrFilename = sFilename.AllocSysString();
					piImageFile->SaveImage2( (IUnknown**)&piImage, IMAGEFORMAT_JPE, bstrFilename, &vbSuccess );
					SysFreeString( bstrFilename );
					if( VARIANT_FALSE != vbSuccess )
						return true;
				}
			}
		}
	}
	return false;
}
void COfficeEpubFile::UpdateGdiPlusRenderer(double dWidthMm, double dHeightMm, AVSMediaCore3::IAVSUncompressedVideoFrame** piImage, AVSGraphics::IASCGraphicsRenderer** piRend)
{
	(*piImage) = NULL;
	(*piRend) = NULL;
	CoCreateInstance(__uuidof( AVSMediaCore3::CAVSUncompressedVideoFrame), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSMediaCore3::IAVSUncompressedVideoFrame), (void **)piImage);
	AVSGraphics::IAVSFontManagerPtr piFontManager;
	piFontManager.CreateInstance( __uuidof(AVSGraphics::CAVSFontManager) );
	piFontManager->Initialize( L"" );

	//задаем dpi без привязки к монитору
	int nLOGPIXELSX = 96;
	int nLOGPIXELSY = 96;

	//размер исходной картинки
	long nWidthPix = (long)(nLOGPIXELSX * dWidthMm / (10 * ONE_INCH));
	long nHeightPix = (long)(nLOGPIXELSY * dWidthMm * dHeightMm / (10 * dWidthMm * ONE_INCH));

	(*piImage)->Width = nWidthPix;
	(*piImage)->Height = nHeightPix;
	(*piImage)->put_Stride(0, nWidthPix * 4);

	(*piImage)->AspectRatioX = nWidthPix;
	(*piImage)->AspectRatioY = nHeightPix;

	(*piImage)->DataSize = 4 * nWidthPix * nHeightPix;

	(*piImage)->ColorSpace = 64 + (1 << 31);
	(*piImage)->AllocateBuffer(-1);

	memset((*piImage)->Buffer, 255, (*piImage)->BufferSize);

	CoCreateInstance(__uuidof( AVSGraphics::CAVSGraphicsRenderer), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IASCGraphicsRenderer), (void **)piRend);
	//ставим FontManager
	VARIANT vtVariant;
	vtVariant.vt = VT_UNKNOWN;
	vtVariant.punkVal = piFontManager;
	(*piRend)->SetAdditionalParam( L"FontManager", vtVariant );

	(*piRend)->put_Width(dWidthMm);
	(*piRend)->put_Height(dHeightMm);
	(*piRend)->CreateFromMediaData(*piImage, 0, 0, nWidthPix, nHeightPix );
}