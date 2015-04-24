#pragma once
#include "OOXRelsWriter.h"

class OOXSettingsWriter
{
private:
	CString m_sFileXml; 
	OOXWriter& m_oWriter;

	std::wstring CreateXml()
	{
		std::wstring sResult;
		sResult.append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
		sResult.append( _T("\n") );
		sResult.append( _T("<w:settings xmlns:w = \"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m = \"http://schemas.openxmlformats.org/officeDocument/2006/math\">") );
		sResult.append( m_sFileXml );
		sResult.append( _T("</w:settings>") );
		return sResult;
	}
public: 
	OOXRelsWriterPtr m_oRelsWriter;

	OOXSettingsWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
	{
		m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("settings.xml"), oDocument ) );
		oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
	}
	void AddContent( CString sText )
	{
		m_sFileXml.Append( sText );
	}
	bool Save( CString sFolder )
	{
		CString pathWord = sFolder + FILE_SEPARATOR_STR + _T("word");

		if( false == m_sFileXml.IsEmpty() )
		{
			//генерируем свою xml
			 std::wstring sXml = CreateXml();

			//todoooo ПЕРЕПИСАТЬ

			////берем xml из шаблона
			//CString sFilename = sFolder +  _T("\\word\\settings.xml");
			//if( true == RtfUtility:: SaveResourceToFile( IDR_SETTINGS, L"XML", sFilename ) )
			//{
			//	XmlUtils::CXmlLiteReader oXmlReader1;
			//	XmlUtils::CXmlLiteReader oXmlReader2;

			//	if( TRUE == oXmlReader1.OpenFromFile( sFilename ) && TRUE == oXmlReader2.OpenFromXmlString( sXml ) &&
			//		TRUE == oXmlReader1.ReadRootNode( _T("w:settings") ) && TRUE == oXmlReader2.ReadRootNode( _T("w:settings") ) )
			//	{
			//		sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");
			//		//sXml.Append( RtfUtility::MergeXml( oXmlReader1, oXmlReader2 ) );
			//	}
			//}

			CFile file;
            if (file.CreateFile(pathWord + FILE_SEPARATOR_STR + _T("settings.xml"))) return false;

			m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings"), _T("settings.xml") );
			m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml"), _T("/word/settings.xml") );

            std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

            file.WriteFile((void*)sXmlUTF.c_str(), sXmlUTF.length());

			file.CloseFile();
;
			return true;
		}
		else
		{
			//if( true == RtfUtility:: SaveResourceToFile( IDR_SETTINGS, L"XML", sFolder +  _T("\\word\\settings.xml") ) )
			//{
			//	m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings"), _T("settings.xml") );
			//	m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml"), _T("/word/settings.xml") );
			//}
		}
		return false;
	}

	bool IsEmpty()
	{
		return m_sFileXml.IsEmpty();
	}
};