/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "../RtfDocument.h"
#include "OOXWriter.h"
#include "../../../../Common/FileWriter.h"

class OOXDocumentWriter
{
private: 
	OOXWriter& m_oWriter;
	RtfDocument& m_oDocument;
	NFileWriter::CBufferedFileWriter* m_oFileWriter;
	bool m_bFirst; //один параграф пишем другой храним в памяти
public: 
	OOXDocumentWriter( OOXWriter& oWriter,RtfDocument& oDocument ): m_oWriter(oWriter), m_oDocument(oDocument)
	{
		m_oFileWriter = NULL;
		m_bFirst = true;
	}
	~OOXDocumentWriter()
	{
		RELEASEOBJECT( m_oFileWriter );
	}
	CString CreateXmlStart();

	CString CreateXmlEnd( );

	bool SaveByItemStart( CString sFolder )
	{
		CString pathWord = sFolder + FILE_SEPARATOR_STR + _T("word");
        FileSystem::Directory::CreateDirectory(pathWord) ;
	
		try
		{
			CString sFilename = pathWord + FILE_SEPARATOR_STR + _T("document.xml");
			m_oFileWriter = new NFileWriter::CBufferedFileWriter( sFilename );
		}
		catch(...)
		{
			return false;
		}
		m_oWriter.m_oRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument"), _T("word/document.xml") );
		m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml"), _T("/word/document.xml") );

		CString sXml = CreateXmlStart( );
        std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.GetBuffer());

        m_oFileWriter->Write((BYTE*)sXmlUTF.c_str(), sXmlUTF.length());
        return true;
	}
	bool SaveByItem()
	{
		if( true == m_bFirst )
			m_bFirst = false;
		else
		{
			RenderParameter oNewParam;
			oNewParam.poDocument	= &m_oDocument;
			oNewParam.poWriter		= &m_oWriter;
			oNewParam.poRels		= &m_oWriter.m_oDocRels;
			oNewParam.nType			= RENDER_TO_OOX_PARAM_UNKNOWN;

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
                    std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.GetBuffer());

                    m_oFileWriter->Write((BYTE*)sXmlUTF.c_str(), sXmlUTF.length());
				}
				//удаляем секцию
				m_oDocument.RemoveItem( 0 );
			}
			else if( m_oDocument.GetCount() > 0 && m_oDocument[0]->GetCount() > 0 )//пишем параграф
			{
				CString sXml = m_oDocument[0]->operator[](0)->RenderToOOX(oNewParam);
				//удаляем первый параграф
				m_oDocument[0]->RemoveItem( 0 );
                std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.GetBuffer());

				if (m_oFileWriter)
				{
					m_oFileWriter->Write((BYTE*)sXmlUTF.c_str(), sXmlUTF.length());
				}
				else
				{
					//!!!!
					m_oFileWriter = NULL;
				}
            }
		}
		return true;
	}
	bool SaveByItemEnd()
	{
		RenderParameter oNewParam;
		oNewParam.poDocument	= &m_oDocument;
		oNewParam.poWriter		= &m_oWriter;
		oNewParam.poRels		= &m_oWriter.m_oDocRels;
		oNewParam.nType			= RENDER_TO_OOX_PARAM_UNKNOWN;

		if( m_oDocument.GetCount() > 0 && m_oDocument[0]->GetCount() > 0 )//дописываем последний параграф
		{
			CString sXml = m_oDocument[0]->operator[](0)->RenderToOOX(oNewParam);
			//удаляем первый параграф
			m_oDocument[0]->RemoveItem( 0 );
            std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.GetBuffer());

            m_oFileWriter->Write((BYTE*)sXmlUTF.c_str(), sXmlUTF.length());
        }
		CString sXml = CreateXmlEnd( );
        std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.GetBuffer());

		if (m_oFileWriter)
		{
			m_oFileWriter->Write((BYTE*)sXmlUTF.c_str(), sXmlUTF.length());
		}
        RELEASEOBJECT( m_oFileWriter );
		return true;
	}
	int GetCount()
	{
		int nCount = 0;
		for( int i = 0; i < m_oDocument.GetCount(); i++ )
			nCount += m_oDocument[i]->GetCount();
		return nCount;
	}
};
