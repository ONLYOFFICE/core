#pragma once
#include "XmlUtils.h"
#include "OOXRelsWriter.h"
#include "Resource.h"

class OOXDocPropAppWriter
{
private: CString m_sContent;
private: OOXWriter& m_oWriter;
public: OOXRelsWriter* m_oRelsWriter;
public: OOXDocPropAppWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter( oWriter )
		{
			m_oRelsWriter = &oWriter.m_oRels;
		}
private: CString CreateXml()
		{
			CString sResult;
			sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
			sResult.Append( _T("\n") );
			sResult.Append( _T("<Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" >") );
			sResult.Append( m_sContent );
			sResult.Append( _T("</Properties>") );
			return  sResult;
		}
public: void AddContent( CString sText )
		{
			m_sContent.Append( sText );
		}
public: bool Save( CString sFolder)
		{
			if( false == m_sContent.IsEmpty() )
			{
				m_oWriter.m_oRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties"), _T("docProps/app.xml") );
				m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.extended-properties+xml"), _T("/docProps/app.xml") );

				CreateDirectory(sFolder + _T("\\docProps"), NULL) ;
				HANDLE hFile = ::CreateFile(sFolder + _T("\\docProps\\app.xml"),
						GENERIC_WRITE,
						0,
						0,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						0);
				//ATLASSERT( INVALID_HANDLE_VALUE != hFile );

				if( INVALID_HANDLE_VALUE != hFile )
				{	
					 DWORD dwBytesWritten;
					 CString sXml = CreateXml();
					 CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
					 ::WriteFile(hFile, sXmlUTF, sXmlUTF.GetLength(), &dwBytesWritten, NULL);
					 CloseHandle( hFile );
					 return true;
				}
			}
			else
			{
				CreateDirectory(sFolder + _T("\\docProps"), NULL) ;
				if( true == RtfUtility:: SaveResourceToFile( IDR_APP, L"XML", sFolder + _T("\\docProps\\app.xml") ) )
				{
					m_oWriter.m_oRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties"), _T("docProps/app.xml") );
					m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.extended-properties+xml"), _T("/docProps/app.xml") );
				}
				else
				{
					RemoveDirectory( sFolder + _T("\\docProps") );
				}
			}
			return false;
		}
 public: bool IsEmpty()
		{
			return m_sContent.IsEmpty();
		}
};