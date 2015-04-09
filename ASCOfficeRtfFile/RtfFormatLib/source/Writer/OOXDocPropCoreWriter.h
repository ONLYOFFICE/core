#pragma once
#include "OOXRelsWriter.h"

class OOXDocPropCoreWriter
{
private: 
	CString			m_sFileXml;
	OOXWriter&		m_oWriter;
	
	CString CreateXml()
	{
		CString sResult;
		sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
		sResult.Append( _T("\n") );
		sResult.Append( _T("<cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">") );
		sResult.Append( m_sFileXml );
		sResult.Append( _T("</cp:coreProperties>") );
		return sResult;
	}
public: 
	OOXRelsWriter* m_oRelsWriter;

	OOXDocPropCoreWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
	{
		m_oRelsWriter = &oWriter.m_oRels;
	}
	void AddContent( CString sText )
	{
		m_sFileXml.Append( sText );
	}
	bool Save( CString sFolder)
	{
		CString pathDocProps = sFolder + FILE_SEPARATOR_STR + _T("docProps");
		FileSystem::Directory::CreateDirectoryW(pathDocProps) ;

		if( false == m_sFileXml.IsEmpty() )
		{
			m_oWriter.m_oRels.AddRelationship( _T("http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties"), _T("docProps/core.xml") );
			m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-package.core-properties+xml"), _T("/docProps/core.xml") );

			CFile file;
			if (file.CreateFileW(pathDocProps + FILE_SEPARATOR_STR + _T("core.xml"))) return false;
	
			 CString sXml = CreateXml();
			 CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
			 file.WriteFile(sXmlUTF.GetBuffer(), sXmlUTF.GetLength());
			 file.CloseFile();
			 return true;
		}
		else
		{
			//todooo - default core !!!
		}
		return false;
	}
	bool IsEmpty()
	{
		return m_sFileXml.IsEmpty();
	}
};