#pragma once
#include "OOXWriter.h"
#include "../RtfDocument.h"

class OOXNumberingWriter
{
public:
	OOXRelsWriterPtr m_oRelsWriter;
 
	OOXNumberingWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
	{
		m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("numbering.xml"), oDocument ) );
		oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
		//m_sFileXml.Append( oDocument.RenderToOOX(&oWriter,&oDocument,"numbering.xml","") );
	}
	void AddNumbering( CString sText )
	{
		m_sFileXml.Append( sText );
	}
	bool Save( CString sFolder )
	{
		if( false == m_sFileXml.IsEmpty() )
		{
			CFile file;

			if (file.CreateFileW(sFolder + FILE_SEPARATOR_STR + _T("word") + FILE_SEPARATOR_STR + _T("numbering.xml")) != S_OK) return false;
				
			m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering"), _T("numbering.xml") );
			m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml"), _T("/word/numbering.xml") );

			 DWORD dwBytesWritten;
			 CString sXml = CreateXml();
			 CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
			
			 file.WriteFile(sXmlUTF.GetBuffer(), sXmlUTF.GetLength());
			 file.CloseFile();
			 return true;
			
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
		sResult.Append( _T("\n") );
		sResult.Append( _T("<w:numbering xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\">") );
		sResult.Append( m_sFileXml );
		sResult.Append( _T("</w:numbering>") );
		return sResult;
	}
};