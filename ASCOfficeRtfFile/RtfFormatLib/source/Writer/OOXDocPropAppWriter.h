#pragma once

#include "OOXRelsWriter.h"

class OOXDocPropAppWriter
{
private: 
	CString m_sContent;
	OOXWriter& m_oWriter;

	CString CreateXml()
	{
		CString sResult;
		sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
		sResult.Append( _T("\n") );
		sResult.Append( _T("<Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" >") );
		sResult.Append( m_sContent );
		sResult.Append( _T("</Properties>") );
		return  sResult;
	}

public: 
	OOXRelsWriter* m_oRelsWriter;
	
	OOXDocPropAppWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter( oWriter )
	{
		m_oRelsWriter = &oWriter.m_oRels;
	}
	void AddContent( CString sText )
	{
		m_sContent.Append( sText );
	}
	bool Save( CString sFolder)
	{
		CString pathDocProps = sFolder + FILE_SEPARATOR_STR + _T("docProps");
		FileSystem::Directory::CreateDirectoryW(pathDocProps) ;
		
		if( false == m_sContent.IsEmpty() )
		{
			m_oWriter.m_oRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties"), _T("docProps/app.xml") );
			m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.extended-properties+xml"), _T("/docProps/app.xml") );

			CFile file;
			if (file.CreateFileW(pathDocProps + FILE_SEPARATOR_STR + _T("app.xml"))) return false;

			 CString sXml = CreateXml();
			 CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
			 file.WriteFile(sXmlUTF.GetBuffer(), sXmlUTF.GetLength());
			 file.CloseFile();
			 return true;
		}
		else
		{
			//todooo - default app !!!
		}
		return false;
	}
	bool IsEmpty()
	{
		return m_sContent.IsEmpty();
	}
};