#pragma once
#include "OOXRelsWriter.h"
#include "Resource.h"

class OOXSettingsWriter
{
private: CString m_sFileXml;
private: OOXWriter& m_oWriter;
public: OOXRelsWriterPtr m_oRelsWriter;
public: OOXSettingsWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
		{
			m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("settings.xml"), oDocument ) );
			oWriter.m_oCustomRelsWriter.Add( m_oRelsWriter );
		}
private: CString CreateXml()
		{
			CString sResult;
			sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
			sResult.Append( _T("\n") );
			sResult.Append( _T("<w:settings xmlns:w = \"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m = \"http://schemas.openxmlformats.org/officeDocument/2006/math\">") );
			sResult.Append( m_sFileXml );
			sResult.Append( _T("</w:settings>") );
			return sResult;
		}
public: void AddContent( CString sText )
		{
			m_sFileXml.Append( sText );
		}
public: bool Save( CString sFolder )
		{
			if( false == m_sFileXml.IsEmpty() )
			{
				//генерируем свою xml
				CString sXml = CreateXml();

				//берем xml из шаблона
				CString sFilename = sFolder +  _T("\\word\\settings.xml");
				if( true == RtfUtility:: SaveResourceToFile( IDR_SETTINGS, L"XML", sFilename ) )
				{
					XmlUtils::CXmlReader oXmlReader1;
					XmlUtils::CXmlReader oXmlReader2;

					if( TRUE == oXmlReader1.OpenFromFile( sFilename ) && TRUE == oXmlReader2.OpenFromXmlString( sXml ) &&
						TRUE == oXmlReader1.ReadRootNode( _T("w:settings") ) && TRUE == oXmlReader2.ReadRootNode( _T("w:settings") ) )
					{
						sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");
						sXml.Append( RtfUtility::MergeXml( oXmlReader1, oXmlReader2 ) );
					}
				}

				HANDLE hFile = ::CreateFile( sFilename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0 );

				m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings"), _T("settings.xml") );
				m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml"), _T("/word/settings.xml") );

				DWORD dwBytesWritten;
				CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
				::WriteFile(hFile, sXmlUTF, sXmlUTF.GetLength(), &dwBytesWritten, NULL);
				CloseHandle( hFile );
				return true;
			}
			else
			{
				if( true == RtfUtility:: SaveResourceToFile( IDR_SETTINGS, L"XML", sFolder +  _T("\\word\\settings.xml") ) )
				{
					m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings"), _T("settings.xml") );
					m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml"), _T("/word/settings.xml") );
				}
			}
			return false;
		}

 public: bool IsEmpty()
		{
			return m_sFileXml.IsEmpty();
		}
};