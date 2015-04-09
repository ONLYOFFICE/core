#pragma once

#include "../IdGenerator.h"

class OOXThemeWriter
{
private: 
	OOXWriter& m_oWriter;
public: 
	OOXThemeWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
	{
	}
	bool Save( CString sFolder ) //todo доделать очистку поле ошибок
	{
		CString pathWord = sFolder + FILE_SEPARATOR_STR + _T("word");
		
		FileSystem::Directory::CreateDirectoryW(pathWord) ;
		FileSystem::Directory::CreateDirectoryW(pathWord + FILE_SEPARATOR_STR + _T("theme")) ;

		//todooo default theme !!!!!
		//
		//if( true == RtfUtility:: SaveResourceToFile( IDR_THEME, L"XML", sFolder + _T("\\word\\theme\\theme1.xml") ) )
		//{
		//	m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme"), _T("theme/theme1.xml") );
		//	m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.theme+xml"), _T("/word/theme/theme1.xml") );
		//}
		//else
		//{
		//	RemoveDirectory( sFolder + _T("\\word\\theme") );
		//}
		return true;
	}
};