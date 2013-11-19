#pragma once
#include "..\..\..\..\Common\XmlUtils.h"

#define USE_ATL_CSTRING
#include "..\..\..\..\Common\StringUtils.h"

#include "../Common/OfficeFileErrorDescription.h"

class SimpleDocxFormat
{
private: CAtlArray<CString> m_aRelsWalked;
private: CAtlArray<CString> m_aImagesToExtract;
public: void ExtractImages( CString sSourcePath, CString sDestPath )
		{
			m_aRelsWalked.RemoveAll();
			m_aImagesToExtract.RemoveAll();

			NormalizePath( sSourcePath, true );
			NormalizePath( sDestPath, true );
			ExtractImagesFromRels( sSourcePath + _T("_rels/.rels"), sSourcePath, sDestPath );
		}
private: void ExtractImagesFromRels( CString sSourceRels, CString sSourcePath, CString& sDestPath )
		{
			//нормализуем имя
			TCHAR  buffer[1024]; 
			GetFullPathName( sSourceRels, 1024, buffer, NULL );
			sSourceRels = CString( buffer );

			if( ::GetFileAttributes( sSourceRels ) == INVALID_FILE_ATTRIBUTES )
				return;

			//проверяем не обрабатывали ли ранее
			for( int i = 0; i < m_aRelsWalked.GetCount(); i++ )
				if( m_aRelsWalked[i] == sSourceRels )
					return;
			m_aRelsWalked.Add( sSourceRels );

			XmlUtils::CXmlReader oXmlReader;
			if( TRUE == oXmlReader.OpenFromFile( sSourceRels ) )
				if( TRUE == oXmlReader.ReadRootNode( _T("Relationships") ) )
					if( TRUE == oXmlReader.ReadNodeList( _T("Relationship") ) )
					{
						for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
						{
							CString sTargetMode = oXmlReader.ReadNodeAttribute( i, _T("TargetMode") );
							if( _T("External") != sTargetMode )
							{
								CString sType = oXmlReader.ReadNodeAttribute( i, _T("Type") );
								CString sTargetRel = oXmlReader.ReadNodeAttribute( i, _T("Target") );
								NormalizePath( sTargetRel, false );
								CString sTargetFile = sSourcePath + sTargetRel;

								TCHAR tDrive[256];
								TCHAR tDir[256];
								TCHAR tFilename[256];
								TCHAR tExt[256];
								_tsplitpath( sTargetFile, tDrive, tDir, tFilename, tExt );
								CString sDrive = CString( tDrive );
								CString sDir = CString( tDir );
								CString sFilename = CString( tFilename );
								CString sExt = CString( tExt );

								if( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image") == sType )
								{
									//нормализуем имя
									GetFullPathName( sTargetFile, 1024, buffer, NULL );
									sTargetFile = CString( buffer );

									//проверяем не копировали ли эту картинку
									bool bDuplicate = false;
									for( int j = 0; j < m_aImagesToExtract.GetCount(); j++ )
										if( m_aImagesToExtract[j] == sTargetFile )
										{
											bDuplicate = true;
											break;
										}
									if( false == bDuplicate )
									{
										m_aImagesToExtract.Add( sTargetFile );

										CString sNewTarget = sDestPath + sFilename + sExt;
										//если такой файл уже существует - добавляем счетчик
										if( INVALID_FILE_ATTRIBUTES != ::GetFileAttributes( sNewTarget ) )
										{
											DWORD dwFileAtrr = !INVALID_FILE_ATTRIBUTES;
											int nCounter = 1;
											while( INVALID_FILE_ATTRIBUTES != dwFileAtrr )
											{
												CString sCounter;
												sCounter.AppendFormat( _T("%d"), nCounter );
												while( sCounter.GetLength() < 3 )
													sCounter.Insert( 0, '0' );
												sNewTarget = sDestPath + sFilename + _T("_") + sCounter + sExt;
												nCounter++;
												dwFileAtrr = ::GetFileAttributes( sNewTarget );
											}
										}
										CopyFile( sTargetFile, sNewTarget, FALSE );
									}

								}
								else//пытаемся прочитать rels этого файла
									ExtractImagesFromRels( sDrive + sDir + _T("_rels/") + sFilename + sExt + _T(".rels"), sDrive + sDir, sDestPath );
							}	
						}
					}
		}

public: HRESULT AddObject( CString sPath, CString sOptions )
		{
			HRESULT hRes = S_FALSE;
			NormalizePath( sPath, true );
			
			bool bNeedFindRels = false; // нужно ли искать document.xml.rels( Нет ссылок не надо искать rels)
			CString sDocumentPath = _T("");//абсолютный путь к document.xml
			CString sDocumentRelsPath = _T("");//абсолютный путь к document.xml.rels
			int nStartRelsIndex = 1; //индекс с которого можно делать уникальные ссылки формата rIdN
			CString sDocumentAddition; //добавка к файлу document.xml
			CString sRelsAddition; //добавка к файлу document.xml.rels

			bool bEmptyOptions = true; //если ничего не надо добавлять - true;
			//первый раз пробигаем чтобы узнать есть ли там ссылки (Нет ссылок не надо искать rels)
			XmlUtils::CXmlReader oXmlOptionsReader;
			if( TRUE == oXmlOptionsReader.OpenFromXmlString( sOptions ) )
				if( TRUE == oXmlOptionsReader.ReadRootNode( _T("AddObjects") ) )
					if( TRUE == oXmlOptionsReader.ReadNodeList( _T("Paragraph") ) )
					{
						bEmptyOptions = false;
						for( int i = 0; i < oXmlOptionsReader.GetLengthList(); i++ )
						{
							bool bContinue = true;
							XML::IXMLDOMNodePtr pXmlNode;
							if( TRUE == oXmlOptionsReader.GetNode( i, pXmlNode ) )
							{
								XmlUtils::CXmlReader oXmlSubReader;
								if( TRUE == oXmlSubReader.OpenFromXmlNode( pXmlNode ) )
									if( TRUE == oXmlSubReader.ReadNodeList( _T("*") ) )
									{
										for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
										{
											CString sNodeName = oXmlSubReader.ReadNodeName( j );
											if( _T("Link") == sNodeName )
											{
												bNeedFindRels = true;
												bContinue = false;
												break;
											}
										}
									}
							}
							if( false == bContinue )
								break;
						}
					}
			if( true == bEmptyOptions )
				return S_OK;

			//Ищем документ по главным rels
			XmlUtils::CXmlReader oXmlRelsReader;
			if( ::GetFileAttributes( sPath + _T("_rels/.rels") ) != INVALID_FILE_ATTRIBUTES )
			if( TRUE == oXmlRelsReader.OpenFromFile( sPath + _T("_rels/.rels") ) )
				if( TRUE == oXmlRelsReader.ReadRootNode( _T("Relationships") ) )
					if( TRUE == oXmlRelsReader.ReadNodeList( _T("Relationship") ) )
					{
						for( int i = 0; i < oXmlRelsReader.GetLengthList(); i++ )
						{
							CString sType = oXmlRelsReader.ReadNodeAttribute( i, _T("Type") );
							if( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument") == sType )
							{
								CString sTargetRel = oXmlRelsReader.ReadNodeAttribute( i, _T("Target") );
								NormalizePath( sTargetRel, false );
								//формируем абсолютный путь к document.xml
								sDocumentPath = sPath + sTargetRel;
								if( true == bNeedFindRels )//нужно искать rels документа
								{
									TCHAR tDrive[256];
									TCHAR tDir[256];
									TCHAR tFilename[256];
									TCHAR tExtension[256];
									_tsplitpath( sDocumentPath, tDrive, tDir, tFilename, tExtension );

									sDocumentRelsPath = CString(tDrive) + CString(tDir) + _T("_rels/") + tFilename + tExtension + _T(".rels");
								}
								break;
							}
						}
					}
			if( _T("") == sDocumentPath )
				return AVS_ERROR_UNEXPECTED;

			//пробегаемся по document.xml.rels чтобы вычислить nStartRelsIndex
			if( true == bNeedFindRels )
			{
				XmlUtils::CXmlReader oXmlReader;
				if( ::GetFileAttributes( sDocumentRelsPath ) != INVALID_FILE_ATTRIBUTES )
				if( TRUE == oXmlReader.OpenFromFile( sDocumentRelsPath ) )
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
			}
			//читаем sOptions и формируем добавки к document.xml.rels и document.xml
			XmlUtils::CXmlReader oXmlOptionsReader2;
			if( TRUE == oXmlOptionsReader2.OpenFromXmlString( sOptions ) )
				if( TRUE == oXmlOptionsReader2.ReadRootNode( _T("AddObjects") ) )
					if( TRUE == oXmlOptionsReader2.ReadNodeList( _T("Paragraph") ) )
					{
						XML::IXMLDOMDocumentPtr	pXmlDocument;
						pXmlDocument.CreateInstance(CLSID_DOMDocument);
						if( NULL != pXmlDocument )
						{
							for( int i = 0; i < oXmlOptionsReader2.GetLengthList(); i++ )
							{
								CString sCurParagraph = _T("");
								bool bContinue = true;
								XML::IXMLDOMNodePtr pXmlNode;
								if( TRUE == oXmlOptionsReader2.GetNode( i, pXmlNode ) )
								{
									XmlUtils::CXmlReader oXmlSubReader;
									if( TRUE == oXmlSubReader.OpenFromXmlNode( pXmlNode ) )
									{	
										if( TRUE == oXmlSubReader.ReadNodeList( _T("*") ) )
										{
											int nNodeListLen = oXmlSubReader.GetLengthList();
											if( 0 == i )
												sCurParagraph.Append( _T("<w:p><w:pPr><w:spacing w:after=\"200\" w:before=\"200\" w:line=\"276\" w:lineRule=\"auto\"/></w:pPr>") );
											else
												sCurParagraph.Append( _T("<w:p><w:pPr><w:spacing w:after=\"200\" w:line=\"276\" w:lineRule=\"auto\"/></w:pPr>") );
											if( nNodeListLen > 0 )
											{
												for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
												{
													CString sNodeName = oXmlSubReader.ReadNodeName( j );
													CString sValue = oXmlSubReader.ReadNodeAttribute( j, _T("value") );
													CString sHref = oXmlSubReader.ReadNodeAttribute( j, _T("href") );

													sHref = sHref.Trim();
													//заменяем пробелы на %20
													sHref.Replace( _T(" "), _T("%20") );

													if( _T("Text") == sNodeName )
														sCurParagraph.AppendFormat( _T("<w:r><w:rPr><w:sz w:val=\"24\"/></w:rPr><w:t xml:space=\"preserve\">%s</w:t></w:r>"), sValue );
													else if( _T("Link") == sNodeName )
													{
														CString sCurRID = GenerateRID( nStartRelsIndex );
														if( _T("") != sCurRID )
														{
															//добавляем <w:hyperlink> в document.xml
															sCurParagraph.AppendFormat( _T("<w:hyperlink r:id=\"%s\"><w:r><w:rPr><w:sz w:val=\"24\"/><w:u w:val=\"single\"/><w:color w:val=\"0000ff\"/></w:rPr><w:t xml:space=\"preserve\" >%s</w:t></w:r></w:hyperlink>"), sCurRID, sValue );
															sRelsAddition.AppendFormat( _T("<Relationship Id=\"%s\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"%s\" TargetMode=\"External\"/>"), sCurRID, sHref );

														}
													}
												}
											}
											sCurParagraph.Append( _T("</w:p>") );
										}
									}
								}
								sDocumentAddition.Append( sCurParagraph );
							}
						}
					}
			//добавляем в document.xml.rels
			bool bRelsSucceeded = false;
			if( _T("") != sRelsAddition )
			{
				//оборачиваем дополнительно чтобы можно было читать как xml
				sRelsAddition = _T("<root xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">") + sRelsAddition +_T("</root>"); 
				XML::IXMLDOMDocumentPtr	m_pXmlDocument;
				m_pXmlDocument.CreateInstance(CLSID_DOMDocument);
				if( NULL != m_pXmlDocument )
				{
					VARIANT vtLoad;
					vtLoad.vt = VT_BSTR;
					vtLoad.bstrVal = sDocumentRelsPath.AllocSysString();

					try{
						if( VARIANT_TRUE == m_pXmlDocument->load( vtLoad ) )
						{
							XML::IXMLDOMNodePtr oRootNode = m_pXmlDocument->selectSingleNode( _T("Relationships") );
							if( NULL != oRootNode )
							{
								XmlUtils::CXmlReader oRelationReader;
								if( TRUE == oRelationReader.OpenFromXmlString( sRelsAddition ) )
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
								vtSave.bstrVal = sDocumentRelsPath.AllocSysString();
								m_pXmlDocument->save( vtSave );
								SysFreeString( vtSave.bstrVal );

								bRelsSucceeded = true;
							}
						}
					}
					catch( ... )
					{
					}
					SysFreeString( vtLoad.bstrVal );
				}
			}
			//добавляем в document.xml
			if( _T("") != sDocumentAddition )
			{
				//оборачиваем дополнительно чтобы можно было читать как xml
				sDocumentAddition = _T("<root xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">") + sDocumentAddition +_T("</root>");

				if( false == bNeedFindRels || true == bRelsSucceeded )
				{
					XML::IXMLDOMDocumentPtr	m_pXmlDocument;
					m_pXmlDocument.CreateInstance(CLSID_DOMDocument);
					if( NULL != m_pXmlDocument )
					{
						VARIANT vtLoad;
						vtLoad.vt = VT_BSTR;
						vtLoad.bstrVal = sDocumentPath.AllocSysString();

						try{
							if( VARIANT_TRUE == m_pXmlDocument->load( vtLoad ) )
							{
								XML::IXMLDOMNodePtr oRootNode = m_pXmlDocument->selectSingleNode( _T("w:document") );
								if( NULL == oRootNode )
									XML::IXMLDOMNodePtr oRootNode = m_pXmlDocument->selectSingleNode( _T("w:wordDocument") );
								if( NULL != oRootNode )
								{
									XML::IXMLDOMNodePtr oBodyNode = oRootNode->selectSingleNode( _T("w:body") );
									if( NULL != oBodyNode )
									{
										XML::IXMLDOMNodePtr oLastNode = NULL;
										oBodyNode->get_lastChild( &oLastNode );
										if( NULL != oLastNode )
										{
											VARIANT vtInsert;
											vtInsert.vt = VT_UNKNOWN;
											vtInsert.punkVal = oLastNode;

											XmlUtils::CXmlReader oRelationReader;
											if( TRUE == oRelationReader.OpenFromXmlString( sDocumentAddition ) )
												if( TRUE == oRelationReader.ReadNodeList( _T("*") ) )
												{
													int nLengthList = oRelationReader.GetLengthList();
													for( int i = 0; i < nLengthList ; i++ )
													{
														XML::IXMLDOMNodePtr oCurNode = NULL;
														oRelationReader.GetNode( i, oCurNode );
														if( NULL != oCurNode )
															XML::IXMLDOMNodePtr pBuf = oBodyNode->insertBefore( oCurNode, vtInsert );
													}
												}

											VARIANT vtSave;
											vtSave.vt = VT_BSTR;
											vtSave.bstrVal = sDocumentPath.AllocSysString();
											m_pXmlDocument->save( vtSave );
											SysFreeString( vtSave.bstrVal );

											hRes = S_OK;
										}
									}
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
			return hRes;
		}

private: CString GenerateRID( int& nRID )
		 {
			 CString sResult;
			 sResult.AppendFormat( _T("rId%d"), nRID );
			 nRID++;
			 return sResult;
		 }

// bEnd == true (делает чтобы в конце был '/' или '\'), bEnd == false (делает чтобы в начале не было '/' или '\')
private: void NormalizePath( CString& sPath, bool bEnd = true )
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
};