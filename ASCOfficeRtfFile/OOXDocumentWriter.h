#pragma once
#include "stdafx.h"
#include "RtfDocument.h"
#include "OOXWriter.h"
#include "FileWriter.h"

class OOXDocumentWriter
{
private: OOXWriter& m_oWriter;
private: RtfDocument& m_oDocument;
private: NFileWriter::CBufferedFileWriter* m_oFileWriter;
private: bool m_bFirst; //один параграф пишем другой храним в памяти
public: OOXDocumentWriter( OOXWriter& oWriter,RtfDocument& oDocument ): m_oWriter(oWriter), m_oDocument(oDocument)
		{
			m_oFileWriter = NULL;
			m_bFirst = true;
		}
public: ~OOXDocumentWriter()
		{
			RELEASEOBJECT( m_oFileWriter );
		}
public: CString CreateXmlStart();

public: CString CreateXmlEnd( );

public: bool SaveByItemStart( CString sFolder )
{
	CreateDirectory( sFolder + _T("\\word"), NULL);
	try
	{
		CString sFilename = sFolder + _T("\\word\\document.xml");
		BSTR bstrFilename = sFilename.AllocSysString();
		m_oFileWriter = new NFileWriter::CBufferedFileWriter( bstrFilename );
		SysFreeString( bstrFilename );
	}
	catch(...)
	{
		return false;
	}
	m_oWriter.m_oRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument"), _T("word/document.xml") );
	m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml"), _T("/word/document.xml") );

	CString sXml = CreateXmlStart( );
	CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
	m_oFileWriter->Write( (LPBYTE)(LPCSTR)sXmlUTF, sXmlUTF.GetLength() );
	return true;
}
public: bool SaveByItem()
		{
			if( true == m_bFirst )
				m_bFirst = false;
			else
			{
				RenderParameter oNewParam;
				oNewParam.poDocument = &m_oDocument;
				oNewParam.poWriter = &m_oWriter;
				oNewParam.poRels = &m_oWriter.m_oDocRels;
				oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

				if( m_oDocument.GetCount() > 1 && m_oDocument[1]->GetCount() > 0 )//если что-то есть в следующей секции значит предудущая закончилась
				{
					if( m_oDocument[0]->GetCount() > 0 )
					{
						CString sXml = m_oDocument[0]->operator[](0)->RenderToOOX(oNewParam);
						//пишем вставляем свойства секции
						CString sSectPr = m_oDocument[0]->m_oProperty.RenderToOOX(oNewParam);
						CString sFindStr = _T("</w:pPr>");
						int nIndexpPr = sXml.Find( sFindStr );
						if( -1 != nIndexpPr )
						{
							sXml.Insert( nIndexpPr, sSectPr );
						}
						else
						{
							sFindStr = _T("<w:p>");
							int nIndexP = sXml.Find( _T("<w:p>") );
							if( -1 != nIndexP )
								sXml.Insert( nIndexP + sFindStr.GetLength(), _T("<w:pPr>") + sSectPr + _T("</w:pPr>") );
						}
						//удаляем первый параграф
						m_oDocument[0]->RemoveItem( 0 );
						CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
						m_oFileWriter->Write( (LPBYTE)(LPCSTR)sXmlUTF, sXmlUTF.GetLength() );
					}
					//удаляем секцию
					m_oDocument.RemoveItem( 0 );
				}
				else if( m_oDocument.GetCount() > 0 && m_oDocument[0]->GetCount() > 0 )//пишем параграф
				{
					CString sXml = m_oDocument[0]->operator[](0)->RenderToOOX(oNewParam);
					//удаляем первый параграф
					m_oDocument[0]->RemoveItem( 0 );
					CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
					m_oFileWriter->Write( (LPBYTE)(LPCSTR)sXmlUTF, sXmlUTF.GetLength() );
				}
			}
			return true;
		}
public: bool SaveByItemEnd()
		{
			RenderParameter oNewParam;
			oNewParam.poDocument = &m_oDocument;
			oNewParam.poWriter = &m_oWriter;
			oNewParam.poRels = &m_oWriter.m_oDocRels;
			oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

			if( m_oDocument.GetCount() > 0 && m_oDocument[0]->GetCount() > 0 )//дописываем последний параграф
			{
				CString sXml = m_oDocument[0]->operator[](0)->RenderToOOX(oNewParam);
				//удаляем первый параграф
				m_oDocument[0]->RemoveItem( 0 );
				CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
				m_oFileWriter->Write( (LPBYTE)(LPCSTR)sXmlUTF, sXmlUTF.GetLength() );
			}
			CString sXml = CreateXmlEnd( );
			CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
			m_oFileWriter->Write( (LPBYTE)(LPCSTR)sXmlUTF, sXmlUTF.GetLength() );
			RELEASEOBJECT( m_oFileWriter );
			return true;
		}
public: int GetCount()
		{
			int nCount = 0;
			for( int i = 0; i < m_oDocument.GetCount(); i++ )
				nCount += m_oDocument[i]->GetCount();
			return nCount;
		}
};