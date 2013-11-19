#include "stdafx.h"
#include "OOXReader.h"
#include "OOXSettingsReader.h"
#include "OOXFontTableReader.h"
#include "OOXStyleTableReader.h"
#include "OOXNumberingReader.h"
#include "OOXDocumentReader.h"
#include "OOXAppReader.h"
#include "OOXCoreReader.h"
#include "OOXFootnotesReader.h"
#include "OOXThemeReader.h"

OOXReader::OOXReader( RtfDocument& oDocument,CString path ): m_oDocument(oDocument)
				{
					m_sPath = path;
					m_nCurItap = 0;
				}
bool OOXReader::Parse()
				{
					CString sRelsPath = m_sPath + _T("\\_rels\\.rels");
					m_oRelsReader.Init( sRelsPath );
					CAtlArray<CString> m_aPath;
					m_oRelsReader.GetByType( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument"), m_aPath);
					CString sDocFullPath = m_sPath + _T("\\") + m_aPath[0];

					m_sDocumentPath = GetFolder( sDocFullPath );

					//выделяем имя document.xml
					OOXRelsReaderPtr oDocumentRelsReader;
					int nSlashIndex = sDocFullPath.ReverseFind( '\\' );
					CString sName = sDocFullPath.Right( sDocFullPath.GetLength() - nSlashIndex  - 1) ;

					//проверяем наличие document.xml rels
					CString sDocumentRelsPath = m_sDocumentPath + _T("\\_rels\\") + sName + _T(".rels");
					if( ::GetFileAttributes( sDocumentRelsPath ) != DWORD(-1) )
					{
						oDocumentRelsReader = OOXRelsReaderPtr( new OOXRelsReader() );
						oDocumentRelsReader->Init( sDocumentRelsPath );
					}

					ReaderParameter oReaderParameter;
					oReaderParameter.oReader = this;
					oReaderParameter.oDocument = &m_oDocument;
					oReaderParameter.oRels = oDocumentRelsReader;

					if( true == oDocumentRelsReader->GetByType( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings"), m_aPath) )
					{ //важно чтобы первыми читались именно settings (например для footnote)
						OOXSettingsReader oSettingsReader( m_sDocumentPath + _T("\\") + m_aPath[0] );
						oSettingsReader.Parse( oReaderParameter );
					}
					if( true == oDocumentRelsReader->GetByType( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable"), m_aPath) )
					{
						OOXFontTableReader oFontTableReader( m_sDocumentPath + _T("\\") + m_aPath[0] );
						oFontTableReader.Parse( oReaderParameter );
					}
					if( true == oDocumentRelsReader->GetByType( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme"), m_aPath) )
					{
						OOXThemeReader oThemeReader( m_sDocumentPath + _T("\\") + m_aPath[0] );
						oThemeReader.Parse( oReaderParameter );
					}
					if( true == oDocumentRelsReader->GetByType( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering"), m_aPath) )
					{
						OOXNumberingReader oNumberingReader;
						oNumberingReader.Parse(oReaderParameter, m_sDocumentPath + _T("\\") + m_aPath[0] );
					}
					if( true == oDocumentRelsReader->GetByType( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles"), m_aPath) )
					{
						OOXStyleTableReader oStyleTableReader( m_sDocumentPath + _T("\\") + m_aPath[0] );
						oStyleTableReader.Parse( oReaderParameter );
					}
					if( true == m_oRelsReader.GetByType( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties"), m_aPath)  )
					{
						OOXAppReader oDocPropAppReader( m_sPath + _T("\\") + m_aPath[0]);
						oDocPropAppReader.Parse(oReaderParameter  );
					}
					if( true == m_oRelsReader.GetByType( _T("http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties"), m_aPath)  )
					{
						OOXCoreReader oDocPropCoreReader( m_sPath + _T("\\") + m_aPath[0]);
						oDocPropCoreReader.Parse(oReaderParameter  );
					}
					if( true == oDocumentRelsReader->GetByType( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes"), m_aPath)  )
					{
						OOXFootnotesReader oFootnotesReader;
						oFootnotesReader.Parse( oReaderParameter, m_sDocumentPath + _T("\\") + m_aPath[0], true );
					}
					if( true == oDocumentRelsReader->GetByType( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes"), m_aPath)  )
					{
						OOXFootnotesReader oEndnotesReader;
						oEndnotesReader.Parse( oReaderParameter, m_sDocumentPath + _T("\\") + m_aPath[0], false );  
					}

					if( true == m_oRelsReader.GetByType( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument"), m_aPath) )
					{
						OOXDocumentReader oDocReader( m_sPath + _T("\\") + m_aPath[0]);
						//читаем документ
						oDocReader.Parse( oReaderParameter );
					}

					return true;
				}
CString OOXReader::GetFolder( CString sDocPath )
				{
					int nLastSlash = sDocPath.ReverseFind('\\');
					CString sLeft = sDocPath.Left(nLastSlash  );
					return sLeft;
				}