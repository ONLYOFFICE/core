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

            if (file.CreateFile(sFolder + FILE_SEPARATOR_STR + _T("word") + FILE_SEPARATOR_STR + _T("numbering.xml")) != S_OK) return false;
				
			m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering"), _T("numbering.xml") );
			m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml"), _T("/word/numbering.xml") );

             std::wstring sXml = CreateXml();
             std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

             file.WriteFile((void*)sXmlUTF.c_str(), sXmlUTF.length());
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

    std::wstring CreateXml()
	{
        std::wstring sResult;
        sResult.append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
        sResult.append( _T("\n") );
        sResult.append( _T("<w:numbering xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\">") );
        sResult.append( m_sFileXml.GetBuffer() );
        sResult.append( _T("</w:numbering>") );
		return sResult;
	}
};
