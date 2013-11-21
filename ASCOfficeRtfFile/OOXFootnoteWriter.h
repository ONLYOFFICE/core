#pragma once
#include "stdafx.h"
#include "../Common/XmlUtils.h"
#include "OOXWriter.h"

class OOXFootnoteWriter
{
private: RtfDocument& m_oDocument;
private: OOXWriter& m_oWriter;
public: OOXRelsWriterPtr m_oRelsWriter;
public: OOXFootnoteWriter( OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter),m_oDocument(oDocument)
		{
			m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("footnotes.xml"), oDocument ) );
			oWriter.m_oCustomRelsWriter.Add( m_oRelsWriter );
		}
private: CString m_sFootnotes;
public: void AddFootnoteBegin( CString sType, int nID, CString sText )
		{
			CString sFootnote;
			sFootnote.Append( _T("<w:footnote") );
			if( false == sType.IsEmpty() )
				sFootnote.AppendFormat( _T(" w:type=\"%s\""), sType );
			if( PROP_DEF != nID )
				sFootnote.AppendFormat( _T(" w:id=\"%d\""), nID );
			sFootnote.Append( _T(">") );
			sFootnote.Append( sText );
			sFootnote.Append(_T("</w:footnote>"));

			m_sFootnotes.Insert( 0 , sFootnote );
		}
public: void AddFootnote( CString sType, int nID, CString sText )
		{
			m_sFootnotes.Append( _T("<w:footnote") );
			if( false == sType.IsEmpty() )
				m_sFootnotes.AppendFormat( _T(" w:type=\"%s\""), sType );
			if( PROP_DEF != nID )
				m_sFootnotes.AppendFormat( _T(" w:id=\"%d\""), nID );
			m_sFootnotes.Append( _T(">") );
			m_sFootnotes.Append( sText );
			m_sFootnotes.Append(_T("</w:footnote>"));
		}
private: CString CreateXml()
		{
			CString sResult;
			sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>") );
			sResult.AppendChar('\n');
			sResult.Append( _T("<w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2008/9/16/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2009/2/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingShape\" >") );
			sResult.Append( m_sFootnotes );
			sResult.Append( _T("</w:footnotes>") );
			return sResult;
		}

public: bool Save( CString sFolder )
		{
			if( false == m_sFootnotes.IsEmpty() )
			{
				HANDLE hFile = ::CreateFile(sFolder + _T("\\footnotes.xml"), GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
				//ATLASSERT( INVALID_HANDLE_VALUE != hFile );

				if( INVALID_HANDLE_VALUE != hFile )
				{
					m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes"), _T("footnotes.xml") );
					m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml"), _T("/word/footnotes.xml") );

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
class OOXEndnoteWriter
{
private: RtfDocument& m_oDocument;
private: OOXWriter& m_oWriter;
public: OOXRelsWriterPtr m_oRelsWriter;
public: OOXEndnoteWriter( OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter),m_oDocument(oDocument)
		{
			m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter( _T("endnotes.xml"), oDocument ) );
			oWriter.m_oCustomRelsWriter.Add( m_oRelsWriter );
		}
private: CString m_sEndnotes;
public: void AddEndnoteBegin( CString sType, int nID, CString sText )
		{
			CString sEndnote;
			sEndnote.Append( _T("<w:endnote") );
			if( false == sType.IsEmpty() )
				sEndnote.AppendFormat( _T(" w:type=\"%s\""), sType );
			if( -2 != nID )
				sEndnote.AppendFormat( _T(" w:id=\"%d\""), nID );
			sEndnote.Append( _T(">") );
			sEndnote.Append( sText );
			sEndnote.Append(_T("</w:endnote>") );

			m_sEndnotes.Insert( 0 , sEndnote );
		}
public: void AddEndnote( CString sType, int nID, CString sText )
		{
			m_sEndnotes.Append( _T("<w:endnote") );
			if( false == sType.IsEmpty() )
				m_sEndnotes.AppendFormat( _T(" w:type=\"%s\""), sType );
			if( -2 != nID )
				m_sEndnotes.AppendFormat( _T(" w:id=\"%d\""), nID );
			m_sEndnotes.Append( _T(">") );
			m_sEndnotes.Append( sText );
			m_sEndnotes.Append(_T("</w:endnote>") );
		}
private: CStringA CreateXml()
		{
			CString sResult;
			sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>") );
			sResult.AppendChar('\n');
			sResult.Append( _T("<w:endnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2008/9/16/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2009/2/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingShape\" >") );
			sResult.Append( m_sEndnotes );
			sResult.Append( _T("</w:endnotes>") );
			return  Convert::UnicodeToUtf8( sResult);
		}

public: bool Save( CString sFolder )
		{
			if( false == m_sEndnotes.IsEmpty() )
			{
				HANDLE hFile = ::CreateFile(sFolder + _T("\\endnotes.xml"), GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
				//ATLASSERT( INVALID_HANDLE_VALUE != hFile );

				if( INVALID_HANDLE_VALUE != hFile )
				{
					m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes"), _T("endnotes.xml") );
					m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml"), _T("/word/endnotes.xml") );

					 DWORD dwBytesWritten;
					 CStringA sXml = CreateXml();
					 ::WriteFile(hFile,sXml ,sXml.GetLength(), &dwBytesWritten, NULL);
					 CloseHandle(hFile);
					 return true;
				}
			}
			return false;
		}
};