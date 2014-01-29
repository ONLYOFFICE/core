#pragma once
#include <boost/regex.hpp>
#include "../Common/AtlDefine.h"
#include "../ASCOfficePDFWriter/Consts.h"
#include "../ASCOfficePDFWriter/Utils.h"
#include "ASCUniversalConverterUtils.h"
#include "EpubCrypt.h"
#include "../Common/DocxFormat/Source/SystemUtility/File.h"

using namespace boost::regex_constants;

CString LCID2String( LCID dwID )
{//http://msdn.microsoft.com/en-us/library/0h88fahh%28VS.85%29.aspx
	switch( dwID )
	{
		case 0x0436: return _T("af");break;
		case 0x041C: return _T("sq");break;
		case 0x3801: return _T("ar-ae");break;
		case 0x3C01: return _T("ar-bh");break;
		case 0x1401: return _T("ar-dz");break;
		case 0x0C01: return _T("ar-eg");break;
		case 0x0801: return _T("ar-iq");break;
		case 0x2C01: return _T("ar-jo");break;
		case 0x3401: return _T("ar-kw");break;
		case 0x3001: return _T("ar-lb");break;
		case 0x1001: return _T("ar-ly");break;
		case 0x1801: return _T("ar-ma");break;
		case 0x2001: return _T("ar-om");break;
		case 0x4001: return _T("ar-qa");break;
		case 0x0401: return _T("ar-sa");break;
		case 0x2801: return _T("ar-sy");break;
		case 0x1C01: return _T("ar-tn");break;
		case 0x2401: return _T("ar-ye");break;
		case 0x042B: return _T("hy");break;
		case 0x042C: return _T("az-az");break;
		case 0x082C: return _T("az-az");break;
		case 0x042D: return _T("eu");break;
		case 0x0423: return _T("be");break;
		case 0x0402: return _T("bg");break;
		case 0x0403: return _T("ca");break;
		case 0x0804: return _T("zh-cn");break;
		case 0x0C04: return _T("zh-hk");break;
		case 0x1404: return _T("zh-mo");break;
		case 0x1004: return _T("zh-sg");break;
		case 0x0404: return _T("zh-tw");break;
		case 0x041A: return _T("hr");break;
		case 0x0405: return _T("cs");break;
		case 0x0406: return _T("da");break;
		case 0x0413: return _T("nl-nl");break;
		case 0x0813: return _T("nl-be");break;
		case 0x0C09: return _T("en-au");break;
		case 0x2809: return _T("en-bz");break;
		case 0x1009: return _T("en-ca");break;
		case 0x2409: return _T("en-cb");break;
		case 0x1809: return _T("en-ie");break;
		case 0x2009: return _T("en-jm");break;
		case 0x1409: return _T("en-nz");break;
		case 0x3409: return _T("en-ph");break;
		case 0x1C09: return _T("en-za");break;
		case 0x2C09: return _T("en-tt");break;
		case 0x0809: return _T("en-gb");break;
		case 0x0409: return _T("en-us");break;
		case 0x0425: return _T("et");break;
		case 0x0429: return _T("fa");break;
		case 0x040B: return _T("fi");break;
		case 0x0438: return _T("fo");break;
		case 0x040C: return _T("fr-fr");break;
		case 0x080C: return _T("fr-be");break;
		case 0x0C0C: return _T("fr-ca");break;
		case 0x140C: return _T("fr-lu");break;
		case 0x100C: return _T("fr-ch");break;
		case 0x083C: return _T("gd-ie");break;
		case 0x043C: return _T("gd");break;
		case 0x0407: return _T("de-de");break;
		case 0x0C07: return _T("de-at");break;
		case 0x1407: return _T("de-li");break;
		case 0x1007: return _T("de-lu");break;
		case 0x0807: return _T("de-ch");break;
		case 0x0408: return _T("el");break;
		case 0x040D: return _T("he");break;
		case 0x0439: return _T("hi");break;
		case 0x040E: return _T("hu");break;
		case 0x040F: return _T("is");break;
		case 0x0421: return _T("id");break;
		case 0x0410: return _T("it-it");break;
		case 0x0810: return _T("it-ch");break;
		case 0x0411: return _T("ja");break;
		case 0x0412: return _T("ko");break;
		case 0x0426: return _T("lv");break;
		case 0x0427: return _T("lt");break;
		case 0x042F: return _T("mk");break;
		case 0x043E: return _T("ms-my");break;
		case 0x083E: return _T("ms-bn");break;
		case 0x043A: return _T("mt");break;
		case 0x044E: return _T("mr");break;
		case 0x0414: return _T("no-no");break;
		case 0x0814: return _T("no-no");break;
		case 0x0415: return _T("pl");break;
		case 0x0816: return _T("pt-pt");break;
		case 0x0416: return _T("pt-br");break;
		case 0x0417: return _T("rm");break;
		case 0x0418: return _T("ro");break;
		case 0x0818: return _T("ro-mo");break;
		case 0x0419: return _T("ru");break;
		case 0x0819: return _T("ru-mo");break;
		case 0x044F: return _T("sa");break;
		case 0x0C1A: return _T("sr-sp");break;
		case 0x0432: return _T("tn");break;
		case 0x0424: return _T("sl");break;
		case 0x041B: return _T("sk");break;
		case 0x042E: return _T("sb");break;
		case 0x040A: return _T("es-es");break;
		case 0x2C0A: return _T("es-ar");break;
		case 0x400A: return _T("es-bo");break;
		case 0x340A: return _T("es-cl");break;
		case 0x240A: return _T("es-co");break;
		case 0x140A: return _T("es-cr");break;
		case 0x1C0A: return _T("es-do");break;
		case 0x300A: return _T("es-ec");break;
		case 0x100A: return _T("es-gt");break;
		case 0x480A: return _T("es-hn");break;
		case 0x080A: return _T("es-mx");break;
		case 0x4C0A: return _T("es-ni");break;
		case 0x180A: return _T("es-pa");break;
		case 0x280A: return _T("es-pe");break;
		case 0x500A: return _T("es-pr");break;
		case 0x3C0A: return _T("es-py");break;
		case 0x440A: return _T("es-sv");break;
		case 0x380A: return _T("es-uy");break;
		case 0x200A: return _T("es-ve");break;
		case 0x0441: return _T("sw");break;
		case 0x041D: return _T("sv-se");break;
		case 0x081D: return _T("sv-fi");break;
		case 0x0449: return _T("ta");break;
		case 0x0444: return _T("tt");break;
		case 0x041E: return _T("th");break;
		case 0x041F: return _T("tr");break;
		case 0x0431: return _T("ts");break;
		case 0x0422: return _T("uk");break;
		case 0x0420: return _T("ur");break;
		case 0x0843: return _T("uz-uz");break;
		case 0x0443: return _T("uz-uz");break;
		case 0x042A: return _T("vi");break;
		case 0x0434: return _T("xh");break;
		case 0x043D: return _T("yi");break;
		case 0x0435: return _T("zu");break;
	}
	return _T("en-us");
}

#define BUF_SIZE 2048

typedef enum { FE_NONE = 0, FE_ADOBE = 1, FE_IDPF = 2 } FontEncription;

class IdGenerator
{
public: IdGenerator()
		{
			m_nId = 1;
		}
private: int m_nId;
public: CString GenerateId()
		{
			CString sId;
			sId.AppendFormat( _T("id%d"), m_nId++ );
			return sId;
		}
};

class EpubPackage
{
public: EpubPackage()
		{
			TCHAR tBuffer[256];
			GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_ILANGUAGE, tBuffer, 256 );
			m_sLanguage = LCID2String( _tcstol( tBuffer, NULL, 16 ) );
			m_nFontEncryption = FE_NONE;
			m_bEmbedFont = false;
			m_piFontManager = NULL;
			m_sMetadata = _T("");
		}
public: ~EpubPackage()
		{
			RELEASEINTERFACE( m_piFontManager );
		}
public: CString m_sPackegePath;
public: CString m_sPackegePathRel;
public: FontEncription m_nFontEncryption;
public: bool m_bEmbedFont;
public: CString m_sMetadata;
private: void CorrectString(CString& strValue)
{
	strValue.Replace(_T("&"),	_T("&amp;"));			
	strValue.Replace(_T("'"),	_T("&apos;"));
	strValue.Replace(_T("<"),	_T("&lt;"));
	strValue.Replace(_T(">"),	_T("&gt;"));
	strValue.Replace(_T("\""),	_T("&quot;"));
}
private: CString CorrectString2(CString& str)
{
	CString strValue = str;
	CorrectString(strValue);
	return strValue;
}
const CString getAttr(XML::IXMLDOMElementPtr element, const CString& attrName)
{
	XML::IXMLDOMNamedNodeMapPtr attrs = element->Getattributes();
	while( XML::IXMLDOMAttributePtr attr = attrs->nextNode() )
		if (attr->GetbaseName().GetBSTR() == attrName)
			return attr->GetnodeValue().bstrVal;
	return _T("");
}
public: int Load( CString sPackageFilename )
		{
			TCHAR tDrive[256];
			TCHAR tDir[256];
			_tsplitpath( sPackageFilename, tDrive, tDir, NULL, NULL );
			m_sPackegePath = CString( tDrive );
			m_sPackegePath.Append( tDir );

			XmlUtils::CXmlReader oXmlReader;
			if( TRUE == oXmlReader.OpenFromFile( sPackageFilename ) )
				if( TRUE == oXmlReader.ReadRootNode( _T("package") ) )
				{
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode( oNode );
					XmlUtils::CXmlReader oXmlSubReader;
					//Читаем metadata
					m_sMetadata = _T("");
					try
					{
					if( TRUE == oXmlSubReader.OpenFromXmlNode( oNode ) )
						if( TRUE == oXmlSubReader.ReadNode( _T("metadata") ) )
						{
							CString sTitle = oXmlSubReader.ReadNodeValue(_T("dc:title"));
							CString sCreator = oXmlSubReader.ReadNodeValue(_T("dc:creator"));
							CString sPublisher = oXmlSubReader.ReadNodeValue(_T("dc:publisher"));
							CString sLanguage = oXmlSubReader.ReadNodeValue(_T("dc:language"));
							CString sContributor = oXmlSubReader.ReadNodeValue(_T("dc:contributor"));
							CString sDescription = oXmlSubReader.ReadNodeValue(_T("dc:description"));
							CString sCoverage = oXmlSubReader.ReadNodeValue(_T("dc:coverage"));
							oXmlSubReader.ReadNodeList(_T("meta"));
							for(int i = 0; i < oXmlSubReader.GetLengthList(); i++)
							{
								XML::IXMLDOMNodePtr pMeta;
								oXmlSubReader.GetNode(i, pMeta);
	
								CString sName = getAttr(pMeta, _T("name"));
								if(_T("cover") == sName)
									sCoverage = "1";
							}

							if(_T("") != sTitle)
								m_sMetadata += _T("<name>") + CorrectString2(sTitle) + _T("</name>");
							if(_T("") != sCreator)
							{
								m_sMetadata += _T("<author>") + CorrectString2(sCreator) + _T("</author>");
								m_sMetadata += _T("<creator>") + CorrectString2(sCreator) + _T("</creator>");
							}
							if(_T("") != sPublisher)
								m_sMetadata += _T("<publisher>") + CorrectString2(sPublisher) + _T("</publisher>");
							if(_T("") != sLanguage)
								m_sMetadata += _T("<language>") + CorrectString2(sLanguage) + _T("</language>");
							if(_T("") != sContributor)
								m_sMetadata += _T("<creator>") + CorrectString2(sContributor) + _T("</creator>");
							if(_T("") != sDescription)
								m_sMetadata += _T("<annotation>") + CorrectString2(sDescription) + _T("</annotation>");
							if(_T("") != sCoverage)
								m_sMetadata += _T("<coverpage>1</coverpage>");

							if(_T("") != m_sMetadata)
								m_sMetadata = _T("<meta>") + m_sMetadata + _T("</meta>");
						}
					}
					catch(...)
					{
						m_sMetadata = _T("");
					}

					CAtlArray<CString> aSequence;
					if( TRUE == oXmlSubReader.OpenFromXmlNode( oNode ) )
						if( TRUE == oXmlSubReader.ReadNode( _T("spine") ) )
							if( TRUE == oXmlSubReader.ReadNodeList( _T("itemref") ) )
							{ 
								for( int i = 0; i < oXmlSubReader.GetLengthList(); i++ )
								{
									CString sId = oXmlSubReader.ReadNodeAttribute( i, _T("idref") );
									if( _T("") != sId )
										aSequence.Add( sId );
								}
							}
					if( aSequence.GetCount() > 0 )
					{
						if( TRUE == oXmlSubReader.OpenFromXmlNode( oNode ) )
							if( TRUE == oXmlSubReader.ReadNode( _T("manifest") ) )
								if( TRUE == oXmlSubReader.ReadNodeList( _T("item") ) )
								{ 
									for( int i = 0; i < (int)aSequence.GetCount(); i++ )
									{
										CString sTargetId = aSequence[i];
										for( int j = 0; j < oXmlSubReader.GetLengthList(); j++ )
										{
											CString sMime = oXmlSubReader.ReadNodeAttribute( j, _T("media-type") );
											CString sHRef = oXmlSubReader.ReadNodeAttribute( j, _T("href") );
											//Decode URL
											sHRef.Replace(_T("%20"), _T(" "));
											sHRef.Replace(_T("%3B"), _T(";"));
											sHRef.Replace(_T("%2C"), _T(","));
											sHRef.Replace(_T("%26"), _T("&"));
											sHRef.Replace(_T("%3D"), _T("="));
											sHRef.Replace(_T("%2B"), _T("+"));
											sHRef.Replace(_T("%24"), _T("$"));
											CString sId = oXmlSubReader.ReadNodeAttribute( j, _T("id") );
											if( sTargetId == sId && _T("") != sMime && _T("") != sHRef )
												m_aHtmls.Add( m_sPackegePath + sHRef );
										}
									}
								}
					}
					if( m_aHtmls.GetCount() > 0 )
						return 0;
				}
			return -1;
		}
public: int GetCount()
		{
			return (int)m_aHtmls.GetCount();
		}
public: CString GetItem( int nIndex )
		{
			if( nIndex >= 0 && nIndex < (int)m_aHtmls.GetCount() )
				return m_aHtmls[nIndex];
			return _T("");
		}
public: int Create( CString sFolderPath, CString sFolderPathRelative )
		{
			m_sPackegePath = sFolderPath;
			m_sPackegePathRel = sFolderPathRelative;
			m_sUUID = CreateUUID();
			return 0;
		}
public: void AddHtml( CString sPath )
		 {
			//проверяем размер он не должен превышать 300Kb на xml, иначе читалки Sony такие файлы не проиграет
			// делаем с запасом - 260Kb (как calibre)
			CFile oHtmlFile;
			oHtmlFile.OpenFile( sPath );
			ULONG64 lFileSize =  oHtmlFile.GetFileSize();
			oHtmlFile.CloseFile();
			long nCriticalSize = 260 * 1024;
			long nPartSize = 150 * 1024; // чтобы количество частей было побольше
			if( lFileSize > nCriticalSize )
			{
				//считаем приблизительное число частей
				int nPartsCount = (lFileSize / nPartSize) + 1;
				//разбиваем html
				_SplitHtml( sPath, nPartsCount );
				DeleteFile( sPath );
			}
			else
				_AddHtml(sPath);
		 }
private: void _SplitHtml( CString sPath, long nPartsCount )
		{
			try
			{
				XML::IXMLDOMDocumentPtr	pXmlDocument;
				pXmlDocument.CreateInstance(CLSID_DOMDocument);
				if( NULL != pXmlDocument )
				{
					if( VARIANT_TRUE == pXmlDocument->load( CComVariant(sPath) ) )
					{
						XML::IXMLDOMNodePtr piHtmlNode = NULL;
						XML::IXMLDOMNodePtr piHeadNode = NULL;
						XML::IXMLDOMNodePtr piBodyNode = NULL;
						XML::IXMLDOMNodePtr piBodyDiv = NULL;

						piHtmlNode = pXmlDocument->selectSingleNode( _T("html") );
						piHeadNode = pXmlDocument->selectSingleNode( _T("html/head") );
						piBodyNode = pXmlDocument->selectSingleNode( _T("html/body") );
						piBodyDiv = pXmlDocument->selectSingleNode( _T("html/body/div") );

						if( NULL != piHtmlNode && NULL != piHeadNode && NULL != piBodyNode && NULL != piBodyDiv )
						{
							XML::IXMLDOMNodeListPtr piHtmlAttr = NULL;
							XML::IXMLDOMNodeListPtr piBodyAttr = NULL;
							XML::IXMLDOMNodeListPtr piBodyDivAttr = NULL;

							//запоминаем атрибуты тегов
							piHtmlAttr = piHtmlNode->selectNodes( L"@*" );
							piBodyAttr = piBodyNode->selectNodes( L"@*" );
							piBodyDivAttr = piBodyDiv->selectNodes( L"@*" );
							XML::IXMLDOMNodeListPtr piBodyChilds = NULL;
							piBodyChilds = piBodyDiv->GetchildNodes();
							//если тело пустое или в нем всего один нод, то дальнейшее разбиение невозможно
							//пропускаем эту html
							if( NULL != piBodyChilds )
							{
								long nChildCount = piBodyChilds->Getlength();
								long nPartSize = nChildCount / nPartsCount;
								if( nChildCount > 1 )
								{
									CString sNameWithoutExt = sPath.Left( sPath.GetLength() - (CString(_T(".html"))).GetLength() );
									for( int i = 0; i < nPartsCount ; i++ )
									{
										//копируем стили в новый документ
										XML::IXMLDOMDocumentPtr	pXmlDocumentPart;
										pXmlDocumentPart.CreateInstance(CLSID_DOMDocument);
										//копируем шаблонный html
										XML::IXMLDOMElementPtr pHtmlPart = pXmlDocumentPart->createElement( L"html" );
										if( NULL != piHtmlAttr )
										{
											for( int j = 0 ; j < piHtmlAttr->Getlength(); j++ )
											{
												XML::IXMLDOMNodePtr pCurAttr = piHtmlAttr->Getitem(j);
												pHtmlPart->setAttribute( pCurAttr->GetnodeName(), pCurAttr->GetnodeValue() );
											}
										}
										//копируем шаблонный body
										XML::IXMLDOMElementPtr pBodyPart = pXmlDocumentPart->createElement( L"body" );
										if( NULL != piBodyAttr )
										{
											for( int j = 0 ; j < piBodyAttr->Getlength(); j++ )
											{
												XML::IXMLDOMNodePtr pCurAttr = piBodyAttr->Getitem(j);
												pBodyPart->setAttribute( pCurAttr->GetnodeName(), pCurAttr->GetnodeValue() );
											}
										}
										//копируем шаблонный div
										XML::IXMLDOMElementPtr pDivPart = pXmlDocumentPart->createElement( L"div" );
										if( NULL != piBodyDivAttr )
										{
											for( int j = 0 ; j < piBodyDivAttr->Getlength(); j++ )
											{
												XML::IXMLDOMNodePtr pCurAttr = piBodyDivAttr->Getitem(j);
												pDivPart->setAttribute( pCurAttr->GetnodeName(), pCurAttr->GetnodeValue() );
											}
										}

										//формируем новое имя
										CString sNewPath = sNameWithoutExt;
										sNewPath.AppendFormat( _T("%d.html"), i + 1 );

										int nInterStart = 0;
										int nInterEnd = 0;
										if( 0 == i )
											nInterStart = 0;
										else
											nInterStart = i * nPartSize;
										if( nPartsCount - 1 == i )
											nInterEnd = nChildCount;
										else
											nInterEnd = (i + 1) * nPartSize;
										for( int k = nInterStart; k < nInterEnd; k++ )
											pDivPart->appendChild( piBodyChilds->Getitem(k)->cloneNode(VARIANT_TRUE) );

										//соединяем вместе
										pBodyPart->appendChild( pDivPart );
										if( NULL != piHeadNode )
											pHtmlPart->appendChild( piHeadNode );
										pHtmlPart->appendChild( pBodyPart );
										pXmlDocumentPart->appendChild( pHtmlPart );

										//сохраняем в файл
										if( SUCCEEDED(pXmlDocumentPart->save(CComVariant(sNewPath))) )
											AddHtml( sNewPath );
									}
								}
							}
						}
					}
				}
			}
			catch(...)
			{
			}
		}

private: int _AddHtml( CString sPath )
		{
			//добавляем только в манифест
			TCHAR tExt[256];
			TCHAR tFilename[256];
			TCHAR tFolder[256];
			TCHAR tDrive[256];
			_tsplitpath( sPath, tDrive, tFolder, tFilename, tExt );
			CString sExt = CString(tExt);
			CString sFilename = CString(tFilename);
			CString sFolder = CString(tFolder);
			CString sDrive = CString(tDrive);

			CString sNewItem;
			CString sId = m_oIdGenerator.GenerateId();
			sNewItem.AppendFormat( _T("<item href=\"%s\" id=\"%s\" media-type=\"application/xhtml+xml\"/>"), sFilename + sExt, sId );
			m_aHtmlsManifest.Add( sNewItem );
			m_aHtmlsSpine.Add( _T("<itemref idref=\"") + sId + _T("\"/>") );
			m_aHtmls.Add( sFilename + sExt );
			//добавляем картинки из связанной с html папки

			AddAdditional( sPath, tDrive + sFolder , sFilename );
			ModifyHtml( sPath );
			if( true == m_bEmbedFont )
				AddFonts( sPath, tDrive + sFolder , sFilename );
			return 0;
		}
private: void ModifyHtml( CString sPath )
		 {
			std::string sStdFullHtml; 
			ReadFileInString( sPath, sStdFullHtml );

			//c margin неудобно смотреть текст на читалке
			boost::regex xRegEx1( "margin-(top|left|right|bottom)([^/>]*?)(;|(\"))" );
			sStdFullHtml = boost::regex_replace( sStdFullHtml, xRegEx1, "\\4" );
			//div.basic - определяет размер страницы
			boost::regex xRegEx2( "div\\.basic(.*?)\\}" );
			sStdFullHtml = boost::regex_replace( sStdFullHtml, xRegEx2, "" );


			CStringA sFullHtml = CStringA( sStdFullHtml.c_str() );
						//сохраняем измененную html
						HANDLE hFile = ::CreateFile( sPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);
						DWORD dwBytesWritten;
						::WriteFile(hFile, sFullHtml.GetBuffer() , sFullHtml.GetLength(), &dwBytesWritten, NULL);	
						sFullHtml.ReleaseBuffer();
						CloseHandle( hFile );
		 }
private: void AddAdditional( CString sSourceHtml , CString sFolder, CString sFilename )
		 {//добавляем в манифест файлы вставленные в html ( картинки, ... )
			CString sTargetFolder = sFilename + _T("_files/");
			CString sPath = sFolder + sFilename + _T("_files");
			HANDLE Handle;
			WIN32_FIND_DATA FindData;
			DWORDLONG Result=0;

			Handle = FindFirstFile( (sPath+"\\*.*"), &FindData );
			if ( Handle == INVALID_HANDLE_VALUE )
				return;
			do
			{
				if(( CString( FindData.cFileName ) != _T(".") )&&( CString( FindData.cFileName ) != _T("..") ) )
					if( 0 == ( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
					{
						CString sFullFilename = sPath + _T("\\") + FindData.cFileName;
						AddToManifest( sTargetFolder, sFullFilename );
					}
			}
			while(FindNextFile(Handle, &FindData) != 0);
			FindClose(Handle);
		 }
private: void AddFonts( CString sSourceHtml , CString sFolder, CString sFilename )
		{
			CString sTargetFolder = sFilename + _T("_files/");
			CString sPath = sFolder + sFilename + _T("_files");

			//названия всех шрифтов
			CAtlArray<CString> aFonts;

			//встраиваим шрифты из css
			CString sTargetFontFolder = _T("fonts/");
			CString sFontPath = sFolder + _T("fonts");
			//ищем в исходном html файле
			std::wstring sStdFullHtml; 
			boost::regex xRegEx( "font-family\\:'(.*?)';" );
			XmlUtils::CXmlReader oXmlReader;
			if(TRUE == oXmlReader.OpenFromFile(sSourceHtml))
				if(TRUE == oXmlReader.ReadRootNode(_T("html")))
				{
					XML::IXMLDOMNodePtr pFullXmlNode;
					if( SUCCEEDED( oXmlReader.GetNode( pFullXmlNode ) ) )
					{
						sStdFullHtml = pFullXmlNode->Getxml();
						FindFontInString( sStdFullHtml, xRegEx, aFonts );

						////ищем шрифты во внешних css
						//if(TRUE == oXmlReader.ReadNode(_T("head")))
						//	if(TRUE == oXmlReader.ReadNodeList(_T("link")))
						//	{
						//		for( int i = 0 ;i < oXmlReader.GetLengthList(); i++ )
						//		{
						//			CString sRel = oXmlReader.ReadNodeAttribute( i, _T("rel") );
						//			if( _T("stylesheet") == sRel )
						//			{
						//				CString sHref = oXmlReader.ReadNodeAttribute( i, _T("href") );
						//				TCHAR tFolder[256];
						//				TCHAR tDrive[256];
						//				_tsplitpath( sSourceHtml, tDrive, tFolder, NULL, NULL );
						//				CString sFolder = CString(tFolder);
						//				CString sDrive = CString(tDrive);

						//				FindFontInFile( sDrive + sFolder + sHref, aFonts );
						//			}
						//		}
						//	}

						CStringA sFullHtml =  Utils::UnicodeToCodepage( sStdFullHtml.c_str(), CP_UTF8 );
						//формируем добавок к html css
						CString sCSSEmbedFont;
						NSStructures::CFont oCurFont;
						wchar_t wsDisplayName[1000];
						wchar_t wsFontName[1000];
						//ищем файлы шрифтов
						for( int i = 0; i < (int)aFonts.GetCount(); i++ )
						{
							CString sFontName = (CString)aFonts[i];
							CAtlMap<CString, bool>::CPair *pPair =  m_mapFonts.Lookup( sFontName );
							if( NULL == pPair )
							{
								m_mapFonts.SetAt( sFontName, true );
								AddFont( &oCurFont, sFontName, (LPTSTR)wsDisplayName, 1000, (LPTSTR)wsFontName, 1000, sFontPath, sTargetFontFolder, sCSSEmbedFont );
								//AddFont( &oCurFont, sFontName + _T(" Bold"), (LPTSTR)wsDisplayName, 1000, (LPTSTR)wsFontName, 1000, sFontPath, sTargetFontFolder, sCSSEmbedFont );
								//AddFont( &oCurFont, sFontName + _T(" Italic"), (LPTSTR)wsDisplayName, 1000, (LPTSTR)wsFontName, 1000, sFontPath, sTargetFontFolder, sCSSEmbedFont );
								//AddFont( &oCurFont, sFontName + _T(" Bold Italic"), (LPTSTR)wsDisplayName, 1000, (LPTSTR)wsFontName, 1000, sFontPath, sTargetFontFolder, sCSSEmbedFont );
							}						
						}
						//добавляем к css
						if( -1 != sFullHtml.Find( "</style>" ) )
							sFullHtml.Replace( "</style>", sCSSEmbedFont + "\n</style>" );
						else
						{
							CStringA sAddStyle = "<style>";
							sAddStyle.Append( sCSSEmbedFont + "\n</style>\n</head>" );
							sFullHtml.Replace( "</head>", sAddStyle );
						}
						//сохраняем измененную html
						HANDLE hFile = ::CreateFile( sSourceHtml, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);
						DWORD dwBytesWritten;
						::WriteFile(hFile, "п»ї" , 3, &dwBytesWritten, NULL);//заголовок utf-8
						::WriteFile(hFile, sFullHtml.GetBuffer() , sFullHtml.GetLength(), &dwBytesWritten, NULL);	
						sFullHtml.ReleaseBuffer();
						CloseHandle( hFile );
					}
				}
		 }
private: void ReadFileInString( CString sFilepath, std::string& sResult )
		 {
			//читаем файл в строку
			std::string sStdFullHtml;
			CFile oXmlFile;
			oXmlFile.OpenFile( sFilepath );
			ULONG64 lBufSize =  oXmlFile.GetFileSize();
			BYTE* pTempBuf = new BYTE[ lBufSize + 1 ];
			oXmlFile.ReadFile( pTempBuf, lBufSize );
			pTempBuf [lBufSize] = '\0';
			sResult = (char*)pTempBuf;
			RELEASEARRAYOBJECTS( pTempBuf );
			oXmlFile.CloseFile();
		 }
private: void FindFontInString( std::wstring sStdFullHtml, boost::regex xRegEx, CAtlArray<CString>& aResult )
		 {
			boost::wsmatch oFonts;
			std::wstring::const_iterator xItStart = sStdFullHtml.begin();
			std::wstring::const_iterator xItEnd = sStdFullHtml.end();
			while( boost::regex_search( xItStart, xItEnd, oFonts, xRegEx ) )
			{
				CString sNewFonts = ((std::wstring)oFonts[1]).c_str();
				CString sCurFont;
				int curPos= 0;
				sCurFont= sNewFonts.Tokenize (_T(","), curPos);
				while (sCurFont != "")
				{ 
					sCurFont.Trim();

					bool bExist = false;
					for( int j = 0; j < (int)aResult.GetCount(); j++ )
						if( sCurFont == aResult[j] )
						{
							bExist = true;
							break;
						}
					if( false == bExist )
						aResult.Add( sCurFont );

					sCurFont= sNewFonts.Tokenize (_T(","), curPos);
				}
				xItStart = oFonts[1].second;
			}
		 }
private: void FindFontInFile( CString sFilepath, CAtlArray<CString>& aResult )
		 {
			std::string sStdFullHtml; 
			boost::regex xRegEx( "font-family\\:(.*?);" );
			ReadFileInString( sFilepath, sStdFullHtml );

			//FindFontInString( sStdFullHtml, xRegEx, aResult );
		 }
private: void AddFont( NSStructures::CFont *pFont, CString sFontName, LPTSTR wsDisplayName, int nDisplayNameSize, LPTSTR wsFontName, int nFontFileSize, CString sFontPath, CString sTargetFontFolder, CString& sCSSEmbedFont )
		 {
 			BOOL bBold   = FALSE;
			BOOL bItalic = FALSE;
			CString sFontFilePath;
			// получаем фонт по его названию
			if ( GetFontFile( pFont, sFontName, wsDisplayName, 1000, wsFontName, 1000, &bBold, &bItalic ) )
			{
				sFontFilePath = wsFontName;
				//проверяем существуем ли файл шрифта
				if( ::GetFileAttributes( sFontFilePath ) == INVALID_FILE_ATTRIBUTES )
				{//ищем в системной папке
					// Скорее всего фонт не загрузился из-за того, что sFontFile содержит не полный путь
					char *pValue;
					size_t len;
					errno_t err = _dupenv_s( &pValue, &len, "windir" );
					sFontFilePath = CString(pValue);
					sFontFilePath += "\\Fonts\\";
					sFontFilePath += wsFontName;
					if( ::GetFileAttributes( sFontFilePath ) == INVALID_FILE_ATTRIBUTES )
						return;//ничего не поделаешь, файл найти не удалось
				}
				CString sFontFileName = wsFontName;
				//CharLower( sFontFileName.GetBuffer() );
				//sFontFileName.ReleaseBuffer();
				CString sFontAddress = sFontPath + _T("\\") + sFontFileName;
				CreateDirectory( sFontPath, NULL );

				//шифруем файл фонта в папку
				bool bFontEmbedResult = true;
				if( FE_IDPF == m_nFontEncryption )
					bFontEmbedResult = CryptFile( sFontFilePath, sFontAddress, (CStringA)m_sUUID, true, true );
				else if( FE_ADOBE == m_nFontEncryption )
					bFontEmbedResult = CryptFile( sFontFilePath, sFontAddress, (CStringA)m_sUUID, true, false );
				else
				{
					//копируем font в папку
					int nRes = Utils::CopyDirOrFile( sFontFilePath, sFontAddress );
					if( NULL != nRes )
						bFontEmbedResult = false;
				}
				if( true == bFontEmbedResult )
				{
					CString sStyle;
					if( TRUE == bBold )
						sStyle.Append( _T("font-weight:bold;") );
					else
						sStyle.Append( _T("font-weight:normal;") );
					if( TRUE == bItalic )
						sStyle.Append( _T("font-style:italic;") );
					else
						sStyle.Append( _T("font-style:normal;") );
					//прибавка к css
					//sCSSEmbedFont.AppendFormat( _T("@font-face {font-family:'%s';%ssrc:url(%s);}"), sFontName, sStyle,  sTargetFontFolder + sFontFileName );
					sCSSEmbedFont.AppendFormat( _T("@font-face {font-family:'%s';font-weight:normal;font-style:normal;src:url('%s');}\n"), sFontName, sTargetFontFolder + sFontFileName );
					sCSSEmbedFont.AppendFormat( _T("@font-face {font-family:'%s';font-weight:bold;font-style:normal;src:url('%s');}\n"), sFontName, sTargetFontFolder + sFontFileName );
					sCSSEmbedFont.AppendFormat( _T("@font-face {font-family:'%s';font-weight:normal;font-style:italic;src:url('%s');}\n"), sFontName, sTargetFontFolder + sFontFileName );
					sCSSEmbedFont.AppendFormat( _T("@font-face {font-family:'%s';font-weight:bold;font-style:italic;src:url('%s');}\n"), sFontName, sTargetFontFolder + sFontFileName );

					//прибавка к manifest
					AddToManifest( sTargetFontFolder, sFontFileName, _T("application/octet-stream") );
					if( FE_NONE != m_nFontEncryption )
					{
						AddToCrypt( sTargetFontFolder, sFontFileName );
					}
				}

			}
		 }
private: void AddToCrypt( CString sTargetFolder, CString sFullFilename )
		{
			TCHAR tExt[256];
			TCHAR tFilename[256];
			TCHAR tFolder[256];
			TCHAR tDrive[256];
			_tsplitpath( sFullFilename, tDrive, tFolder, tFilename, tExt );
			CString sExt = CString(tExt);
			CString sFilename = CString(tFilename);
			CString sFolder = CString(tFolder);
			CString sDrive = CString(tDrive);

			CString sNewItem;
			sNewItem.AppendFormat( _T("<EncryptedData xmlns=\"http://www.w3.org/2001/04/xmlenc#\"><EncryptionMethod Algorithm=\"http://ns.adobe.com/pdf/enc#RC\"/><CipherData><CipherReference URI=\"%s\"/></CipherData></EncryptedData>"),m_sPackegePathRel + sTargetFolder + sFilename + sExt );
			m_aCrypted.Add( sNewItem );
		}
private: void AddToManifest( CString sTargetFolder, CString sFullFilename, CString sMime = _T("") )
		{
			if( _T("") == sMime )
			{
				TCHAR *pBuffer ;
				FindMimeFromData( NULL, sFullFilename, NULL, NULL, NULL, FMFD_DEFAULT, &pBuffer, 0 );
				sMime = CString( pBuffer );
				RELEASEHEAP(pBuffer);	
			}
			if( _T("") != sMime )
			{
				TCHAR tExt[256];
				TCHAR tFilename[256];
				TCHAR tFolder[256];
				TCHAR tDrive[256];
				_tsplitpath( sFullFilename, tDrive, tFolder, tFilename, tExt );
				CString sExt = CString(tExt);
				CString sFilename = CString(tFilename);
				CString sFolder = CString(tFolder);
				CString sDrive = CString(tDrive);

				CString sNewItem;
				sNewItem.AppendFormat( _T("<item href=\"%s\" id=\"%s\" media-type=\"%s\"/>"),sTargetFolder + sFilename + sExt, m_oIdGenerator.GenerateId(), sMime );
				m_aHtmlsManifest.Add( sNewItem );
			}
		}
public: int Save()
		{
			if( _T("") !=  m_sCover )
				SaveTitle();//сохраняем titlepage.xhtml

			//сохраняем toc.ncx
			SaveToc();

			//сохраняем content.opf
			SaveContent();

			return 0;
		}
public: int SaveTitle()
		{
			HANDLE hFile = NULL;
			DWORD dwBetysWritten = 0;
			hFile = ::CreateFile( m_sPackegePath + _T("\\titlepage.xhtml"), GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);
			if( INVALID_HANDLE_VALUE == hFile )
				return -1;
			CString sTitle = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
			sTitle.Append( _T("<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">") );
			sTitle.Append( _T("<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>") );
			sTitle.Append( _T("<title>Cover</title>") );
			sTitle.Append( _T("<style type=\"text/css\" title=\"override_css\">") );
			sTitle.Append( _T("@page {padding: 0pt; margin:0pt}") );
			sTitle.Append( _T("body { text-align: center; padding:0pt; margin: 0pt; }") );
			sTitle.Append( _T("</style></head>") );
			sTitle.Append( _T("<body><div>") );
			sTitle.AppendFormat( _T("<img src=\"%s\"/>"), m_sCover );
			sTitle.Append( _T("</div></body></html>") );

			CStringA sTitleUtf;
			int nBufSize = WideCharToMultiByte(CP_UTF8, 0, sTitle, -1, NULL, NULL, NULL, NULL);
			LPSTR p = sTitleUtf.GetBuffer(nBufSize);
		    WideCharToMultiByte(CP_UTF8, 0, sTitle, -1, p, nBufSize, NULL, NULL);
			sTitleUtf.ReleaseBuffer();

			::WriteFile( hFile, sTitleUtf, sTitleUtf.GetLength(), &dwBetysWritten, NULL );	
			RELEASEHANDLE( hFile );
			return 0;
		}
public: int SaveToc()
		{
			HANDLE hFile = NULL;
			DWORD dwBetysWritten = 0;
			hFile = ::CreateFile( m_sPackegePath + _T("\\toc.ncx"), GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);
			if( INVALID_HANDLE_VALUE == hFile )
				return -1;
			CString sToc = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
			sToc.Append( _T("<ncx version=\"2005-1\" xmlns=\"http://www.daisy.org/z3986/2005/ncx/\">") );
			sToc.AppendFormat( _T("<head><meta name=\"dtb:uid\" content=\"%s\"/><meta name=\"dtb:depth\" content=\"1\"/></head>"), m_sUUID );
			sToc.AppendFormat( _T("<docTitle><text>%s</text></docTitle>"), m_sTitle );
			sToc.AppendFormat( _T("<docAuthor><text>%s</text></docAuthor>"), m_sAuthor );
			int nHtmlCount = (int)m_aHtmls.GetCount();
			if( nHtmlCount > 0 )
			{
				sToc.Append( _T("<navMap>"));
				for( int i = 0; i < nHtmlCount; i++ )
					sToc.AppendFormat( _T("<navPoint id=\"part%d\" playOrder=\"%d\"><navLabel><text>Part %d</text></navLabel><content src=\"%s\"/></navPoint>"), (i+1), (i+1), (i+1), m_aHtmls[i] );
				sToc.Append( _T("</navMap>"));
			}
			sToc.Append( _T("</ncx>") );
			
			CStringA sTocUtf;
			int nBufSize = WideCharToMultiByte(CP_UTF8, 0, sToc, -1, NULL, NULL, NULL, NULL);
			LPSTR p = sTocUtf.GetBuffer(nBufSize);
		    WideCharToMultiByte(CP_UTF8, 0, sToc, -1, p, nBufSize, NULL, NULL);
			sTocUtf.ReleaseBuffer();

			::WriteFile( hFile, sTocUtf, sTocUtf.GetLength(), &dwBetysWritten, NULL );	
			RELEASEHANDLE( hFile );
			return 0;
		}
public: int SaveContent()
		{
			HANDLE hFile = NULL;
			DWORD dwBetysWritten = 0;
			//создаем файл
			hFile = ::CreateFile( m_sPackegePath + _T("\\content.opf"), GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);
			if( INVALID_HANDLE_VALUE == hFile )
				return -1;
			CString sContent = _T("<?xml version='1.0' encoding='utf-8'?>\n");
			sContent.Append( _T("<package xmlns=\"http://www.idpf.org/2007/opf\" version=\"2.0\" unique-identifier=\"bookid\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:dcterms=\"http://purl.org/dc/terms/\">") );
			sContent.Append( _T("<metadata>") );
			if( false == m_sTitle.IsEmpty() )
				sContent.AppendFormat( _T("<dc:title>%s</dc:title>"), m_sTitle );
			if( false == m_sAuthor.IsEmpty() )
				sContent.AppendFormat( _T("<dc:creator>%s</dc:creator>"), m_sAuthor );
			if( false == m_sLanguage.IsEmpty() )
				sContent.AppendFormat( _T("<dc:language>%s</dc:language>"), m_sLanguage );
			sContent.AppendFormat( _T("<dc:identifier id=\"bookid\">%s</dc:identifier>"), m_sUUID );
			sContent.Append( _T("</metadata>") );
			//sContent.Append( _T("<package xmlns=\"http://www.idpf.org/2007/opf\" version=\"2.0\" >") );

			sContent.Append( _T("<manifest>") );
			if( _T("") != m_sCover )
			{
				sContent.AppendFormat( _T("<item href=\"%s\" id=\"cover\" media-type=\"image/jpeg\"/>"), m_sCover );
				sContent.Append( _T("<item href=\"titlepage.xhtml\" id=\"titlepage\" media-type=\"application/xhtml+xml\"/>") );
			}
			sContent.Append( _T("<opf:item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\"/>") );
			for( int i = 0; i < (int)m_aHtmlsManifest.GetCount(); i++ )
				sContent.Append( m_aHtmlsManifest[i] );
			sContent.Append( _T("</manifest>") );
			sContent.Append( _T("<spine toc=\"ncx\">") );
			if( _T("") != m_sCover )
				sContent.Append( _T("<itemref idref=\"titlepage\"/>") );
			for( int i = 0; i < (int)m_aHtmlsSpine.GetCount(); i++ )
				sContent.Append( m_aHtmlsSpine[i] );
			sContent.Append( _T("</spine>") );
			sContent.Append( _T("</package>") );

			CStringA sContentUtf;
			int nBufSize = WideCharToMultiByte(CP_UTF8, 0, sContent, -1, NULL, NULL, NULL, NULL);
			LPSTR p = sContentUtf.GetBuffer(nBufSize);
		    WideCharToMultiByte(CP_UTF8, 0, sContent, -1, p, nBufSize, NULL, NULL);
			sContentUtf.ReleaseBuffer();

			::WriteFile( hFile, sContentUtf, sContentUtf.GetLength(), &dwBetysWritten, NULL );	
			RELEASEHANDLE( hFile );
			return 0;
		}
public: CString CreateUUID()
		 {
			CString sUUID = _T("urn:uuid:");
			srand ( time(NULL) );
			addRandomHexDigit(sUUID, 8, 0xF, 0);
			sUUID.AppendChar('-');
			addRandomHexDigit(sUUID, 4, 0xF, 0);
			sUUID.AppendChar('-');
			sUUID.AppendChar('4');
			addRandomHexDigit(sUUID, 3, 0xF, 0);
			sUUID.AppendChar('-');
			addRandomHexDigit(sUUID, 1, 0x3, 0x8);
			addRandomHexDigit(sUUID, 3, 0xF, 0);
			sUUID.AppendChar('-');
			addRandomHexDigit(sUUID, 12, 0xF, 0);
			return sUUID;
		 }
private: void addRandomHexDigit( CString& sb, int count, int mask, int value )
{
	for (int i = 0; i < count; i++)
	{
		int v = ( ( rand() % 16 ) & mask ) | value;
		if (v < 10)
			sb.AppendChar((char) ('0' + v));
		else
			sb.AppendChar((char) (('a' - 10) + v));
	}
}
private: IdGenerator m_oIdGenerator;
private: CAtlArray<CString> m_aHtmls;
private: CAtlArray<CString> m_aHtmlsManifest;
private: CAtlArray<CString> m_aHtmlsSpine;
public: CAtlArray<CString> m_aCrypted;
private: CString m_sUUID;
private: CString m_sLanguage;
public: CString m_sTitle;
public: CString m_sAuthor;
public: CString m_sCover;
public: long m_nCoverWidth;
public: long m_nCoverHeight;
public: AVSGraphics::IASCFontManager* m_piFontManager;
private: CAtlMap<CString, bool> m_mapFonts;
};
