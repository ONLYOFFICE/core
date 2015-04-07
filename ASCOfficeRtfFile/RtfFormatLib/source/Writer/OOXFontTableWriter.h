#pragma once

#include "OOXRelsWriter.h"

class OOXFontTableWriter
{
public: 
	OOXRelsWriterPtr m_oRelsWriter;
	OOXFontTableWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
	{
		m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter(  _T("fontTable.xml"), oDocument ) );
		oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
	}

	void AddContent( CString sText )
	{
		m_sFileXml.Append( sText );
	}
	bool Save(  CString sFolder )
	{
		if( false == m_sFileXml.IsEmpty() )
		{
			HANDLE hFile = ::CreateFile(sFolder +  _T("\\word\\fontTable.xml"),
					GENERIC_WRITE,
					0,
					0,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					0);
			//ATLASSERT( INVALID_HANDLE_VALUE != hFile );

			if( INVALID_HANDLE_VALUE != hFile )
			{	
				m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable"), _T("fontTable.xml") );
				m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml"), _T("/word/fontTable.xml") );

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
			//if( true == RtfUtility:: SaveResourceToFile( IDR_FONT_TABLE, L"XML", sFolder +  _T("\\word\\fontTable.xml") ) )
			//{
			//	m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable"), _T("fontTable.xml") );
			//	m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml"), _T("/word/fontTable.xml") );
			//}
		}
		return false;
	}
	bool IsEmpty()
	{
		return m_sFileXml.IsEmpty();
	}
private: 
	CString m_sFileXml;
	OOXWriter& m_oWriter;
	CString CreateXml()
	{
		CString sResult;
		sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
		sResult.Append( _T("<w:fonts xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">") );
		sResult.Append( m_sFileXml );
		sResult.Append( _T("</w:fonts>") );
		return  sResult;
	}
};