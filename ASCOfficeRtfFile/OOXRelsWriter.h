#pragma once
#include "stdafx.h"
#include "../Common/XmlUtils.h"
#include "RtfDocument.h"

class OOXRelsWriter
{
private: CAtlArray< CString > m_aTargets;
private: CAtlArray< CString > m_aTypes;
private: CAtlArray< CString > m_aIDs;
private: CAtlArray< bool > m_aModes;
private: CString m_sFileName;
private: RtfDocument& m_oDocument;
public: OOXRelsWriter( CString sFileName, RtfDocument& oDocument ):m_oDocument(oDocument)
		{
			m_sFileName = sFileName;
		}

public: CString AddRelationship( CString sType, CString sTarget, bool bTargetModeInternal = true )
		{
			for( int i = 0 ;i < (int)m_aTargets.GetCount(); i++ )
				if( sTarget == m_aTargets[i] )
					return m_aIDs[i];
			m_aTargets.Add( sTarget );
			m_aTypes.Add( sType );
			CString sId = m_oDocument.m_oIdGenerator.Generate_rId();
			m_aIDs.Add( sId );
			m_aModes.Add( bTargetModeInternal );
			return sId;
		}
private: CString CreateXml()
		{
			CString sResult;
			sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>") );
			sResult.AppendChar('\n');
			sResult.Append( _T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">") );
			for( int i = 0; i < (int)m_aTargets.GetCount(); i++ )
			{
				sResult.AppendFormat( _T("<Relationship Id=\"%s\" Type=\"%s\" Target=\"%s\""), m_aIDs[i], m_aTypes[i], m_aTargets[i]);
				if( false == m_aModes[i] )
					sResult.Append( _T(" TargetMode=\"External\"") );
				sResult.Append( _T("/>") );
			}
			sResult.Append( _T("</Relationships>") );
			return sResult;
		}

public: bool Save( CString sFolder )
		{
			if( m_aTargets.GetCount() > 0 )
			{
				CreateDirectory(sFolder + _T("\\_rels"),NULL) ;

				HANDLE hFile = ::CreateFile(sFolder + _T("\\_rels\\") + m_sFileName +  _T(".rels"), GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
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
			return false;
		}
};
typedef boost::shared_ptr<OOXRelsWriter> OOXRelsWriterPtr;