#pragma once

#include "../IdGenerator.h"

class OOXStylesWriter
{
public: 
	OOXRelsWriterPtr m_oRelsWriter;
	OOXStylesWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
	{
		m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("styles.xml"), oDocument ) );
		oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
	}
	~OOXStylesWriter()
	{
		int i = 0;
	}
	void AddContent( CString sText )
	{
		m_sFileXml.Append( sText );
	}
	bool Save( CString sFolder )
	{
		CString pathWord = sFolder + FILE_SEPARATOR_STR + _T("word");
		
		//if( false == m_sFileXml.IsEmpty() ) 
		{		
			CFile file;
            if (file.CreateFile(pathWord + FILE_SEPARATOR_STR + _T("styles.xml"))) return false;

			m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles"), _T("styles.xml") );
			m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml"), _T("/word/styles.xml") );

            std::wstring sXml = CreateXml();
            std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

            file.WriteFile((void*)sXmlUTF.c_str(), sXmlUTF.length());

			file.CloseFile();
			return true;
		}
		//else
		//{
		//	//todooo default style !!
		//}
		return false;
	}
private: 
	CString			m_sFileXml;
	OOXWriter&		m_oWriter;
 
    std::wstring CreateXml()
	{
        std::wstring sResult;
        sResult.append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
        sResult.append( _T("\n") );
        sResult.append( _T("<w:styles xmlns:w = \"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">") );
        sResult.append( m_sFileXml.GetBuffer() );
        sResult.append( _T("</w:styles>") );
		return sResult;
	}
};
