#pragma once
#include "stdafx.h"
#include "XmlUtils.h"

class OOXContentTypesWriter
{
private: CAtlArray< CString > m_aTargets;
private: CAtlArray< CString > m_aTypes;

private: CAtlArray< CString > m_aExtensions;
private: CAtlArray< CString > m_aExtTypes;

public: void AddWriter()
		{
		}
public: void AddContent( CString sType, CString sTarget )
		{
			for( int i = 0 ;i < (int)m_aTargets.GetCount(); i++ )
				if( sTarget == m_aTargets[i] )
					return;
			m_aTargets.Add( sTarget );
			m_aTypes.Add( sType );
		}
public: void AddExtension( CString sType, CString sTarget )
		{
			for( int i = 0 ;i < (int)m_aExtensions.GetCount(); i++ )
				if( sTarget == m_aExtensions[i] )
					return;
			m_aExtensions.Add( sTarget );
			m_aExtTypes.Add( sType );
		}
private: CString CreateXml()
		{
			CString sResult;
			sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
			sResult.AppendChar('\n');
			sResult.Append( _T("<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">") );

			sResult.Append( _T("<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>") );
			sResult.Append( _T("<Default Extension=\"xml\" ContentType=\"application/xml\"/>") );

			for( int i = 0; i < (int)m_aExtensions.GetCount(); i++ )
				sResult.AppendFormat( _T("<Default Extension=\"%s\" ContentType=\"%s\"/>"), m_aExtensions[i], m_aExtTypes[i]);

			for( int i = 0; i < (int)m_aTargets.GetCount(); i++ )
				sResult.AppendFormat( _T("<Override PartName=\"%s\" ContentType=\"%s\"/>"), m_aTargets[i], m_aTypes[i]);
			sResult.Append( _T("</Types>") );
			return sResult;
		}
public: bool Save(CString sFolder)
		{
			HANDLE hFile = ::CreateFile(sFolder + _T("\\[Content_Types].xml"),
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
				 ::WriteFile( hFile, sXmlUTF, sXmlUTF.GetLength(), &dwBytesWritten, NULL );
				 CloseHandle( hFile );
				 return true;
			}
			return false;
		}
};