/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include "OOXDocumentWriter.h"
#include "OOXRelsWriter.h"
#include "OOXDocumentWriter.h"
#include "OOXNumberingWriter.h"
#include "OOXFontTableWriter.h"
#include "OOXStylesWriter.h"
#include "OOXSettingsWriter.h"
#include "OOXThemeWriter.h"
#include "OOXFootnoteWriter.h"
#include "OOXStylesWriter.h"

OOXDocumentWriter::OOXDocumentWriter( OOXWriter& oWriter,RtfDocument& oDocument ): m_oWriter(oWriter), m_oDocument(oDocument)
{
	m_oFileWriter = NULL;
	m_bFirst = true;
}
OOXDocumentWriter::~OOXDocumentWriter()
{
	RELEASEOBJECT( m_oFileWriter );
}

std::wstring OOXDocumentWriter::CreateXmlStart()
{
	//пишем Footnotes
	RenderParameter oNewParam;
	oNewParam.poDocument	= &m_oDocument;
	oNewParam.poWriter		= &m_oWriter;
	oNewParam.poRels		= &m_oWriter.m_oDocRels;
	oNewParam.nType			= RENDER_TO_OOX_PARAM_UNKNOWN;

	//пишем document.xml
	std::wstring sResult = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n";
	sResult += L"<w:document \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\" \
xmlns:cx1=\"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
mc:Ignorable=\"w14 w15 w16se wp14\">";

	if (m_oDocument.m_pBackground)
	{
		if (PROP_DEF == m_oDocument.m_pBackground->m_nFillColor)
			m_oDocument.m_pBackground->m_nFillColor = 0xFFFFFF;
		
		RtfColor color(m_oDocument.m_pBackground->m_nFillColor);

		sResult += L"<w:background w:color=\"" + color.ToHexColor() + L"\">";
		{
			oNewParam.nType = RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2;
			sResult += m_oDocument.m_pBackground->RenderToOOX(oNewParam);
			oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		}
		sResult += L"</w:background>";
	}

	sResult += L"<w:body>";
	return sResult;
}
std::wstring OOXDocumentWriter::CreateXmlEnd( )
{
	std::wstring sResult ;

	//пишем все кроме document.xml
	RenderParameter oNewParam;
	oNewParam.poDocument	= &m_oDocument;
	oNewParam.poWriter		= &m_oWriter;
	oNewParam.poRels		= &m_oWriter.m_oDocRels;
	oNewParam.nType			= RENDER_TO_OOX_PARAM_UNKNOWN;

	OOXEndnoteWriter*	poEndnoteWriter		= static_cast<OOXEndnoteWriter*>( m_oWriter.m_poEndnoteWriter );
	OOXFootnoteWriter*	poFootnoteWriter	= static_cast<OOXFootnoteWriter*>( m_oWriter.m_poFootnoteWriter );
	
	if( NULL != m_oDocument.m_oFootnoteCon )
	{
		oNewParam.poRels = poFootnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poFootnoteWriter->AddFootnoteBegin( L"continuationSeparator", 1, m_oDocument.m_oFootnoteCon->RenderToOOX( oNewParam ) );
	}
	if( NULL != m_oDocument.m_oFootnoteSep )
	{
		oNewParam.poRels = poFootnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poFootnoteWriter->AddFootnoteBegin( L"separator", 0, m_oDocument.m_oFootnoteSep->RenderToOOX(oNewParam) );
	}
	if( NULL != m_oDocument.m_oEndnoteCon )
	{
		oNewParam.poRels = poEndnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poEndnoteWriter->AddEndnoteBegin( L"continuationSeparator", 1, m_oDocument.m_oEndnoteSep->RenderToOOX(oNewParam) );
	}
	if( NULL != m_oDocument.m_oEndnoteSep )
	{
		oNewParam.poRels = poEndnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poEndnoteWriter->AddEndnoteBegin( L"separator", 0, m_oDocument.m_oEndnoteSep->RenderToOOX(oNewParam) );
	}

	//fontTable.xml
	OOXFontTableWriter* poFontTableWriter = static_cast<OOXFontTableWriter*>( m_oWriter.m_poFontTableWriter );
	oNewParam.poRels = poFontTableWriter->m_oRelsWriter.get();
	oNewParam.nType = RENDER_TO_OOX_PARAM_FONTTABLE;
	poFontTableWriter->AddContent( m_oDocument.m_oFontTable.RenderToOOX( oNewParam ) );

	//settings.xml
	OOXSettingsWriter* poSettingsWriter = static_cast<OOXSettingsWriter*>( m_oWriter.m_poSettingsWriter );
	oNewParam.poRels = poSettingsWriter->m_oRelsWriter.get();
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	poSettingsWriter->AddContent( m_oDocument.m_oProperty.RenderToOOX(oNewParam) );
	//poSettingsWriter->AddContent( m_oDocument.m_oColorSchemeMapping.RenderToOOX(oNewParam) );
	if (m_oDocument.m_pMathProp)
		poSettingsWriter->AddContent( m_oDocument.m_pMathProp->RenderToOOX(oNewParam) );

	//numbering.xml
	OOXNumberingWriter* poNumberingWriter = static_cast<OOXNumberingWriter*>( m_oWriter.m_poNumberingWriter );
	oNewParam.poRels	= poNumberingWriter->m_oRelsWriter.get();
	oNewParam.nType		= RENDER_TO_OOX_PARAM_NUMBERING;
	
	poNumberingWriter->AddNumbering( m_oDocument.m_oListTable.RenderToOOX(oNewParam) );
	poNumberingWriter->AddNumbering( m_oDocument.m_oListOverrideTable.RenderToOOX(oNewParam) );

	//style.xml
	OOXStylesWriter* poStylesWriter = static_cast<OOXStylesWriter*>( m_oWriter.m_poStylesWriter );
	oNewParam.poRels	= poNumberingWriter->m_oRelsWriter.get();
	oNewParam.nType		= RENDER_TO_OOX_PARAM_STYLES;

	std::wstring sStyles;
	std::wstring sTempParaDef = m_oDocument.m_oDefaultParagraphProp.RenderToOOX(oNewParam);
	std::wstring sTempCharDef = m_oDocument.m_oDefaultCharProp.RenderToOOX(oNewParam);
	
	if( false == sTempParaDef.empty() || false == sTempCharDef.empty() )
	{
		sStyles += L"<w:docDefaults>";
		if( false == sTempCharDef.empty() )
		{
			sStyles += L"<w:rPrDefault><w:rPr>";
			sStyles += sTempCharDef ;
			sStyles += L"</w:rPr></w:rPrDefault>";
		}	
		if( false == sTempParaDef.empty() )
		{
			sStyles += L"<w:pPrDefault><w:pPr>";
			sStyles +=  sTempParaDef;
			sStyles += L"</w:pPr></w:pPrDefault>";
		}
		sStyles += L"</w:docDefaults>";
	}

	sStyles += m_oDocument.m_oStyleTable.RenderToOOX(oNewParam);
	poStylesWriter->AddContent(sStyles);

	//core.xml
	oNewParam.poRels	= NULL;
	oNewParam.nType		= RENDER_TO_OOX_PARAM_CORE;
	oNewParam.poWriter	= m_oWriter.m_poDocPropsCore;
	m_oDocument.m_oInformation.RenderToOOX(oNewParam);

	//app.xml
	oNewParam.poRels	= NULL;
	oNewParam.nType		= RENDER_TO_OOX_PARAM_APP;
	oNewParam.poWriter	= m_oWriter.m_poDocPropsApp;
	m_oDocument.m_oInformation.RenderToOOX(oNewParam);


	//пишем финальные свойства секции
	oNewParam.poDocument = &m_oDocument;
	oNewParam.poWriter = &m_oWriter;
	oNewParam.poRels = &m_oWriter.m_oDocRels;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	
	sResult += m_oDocument[0].props->m_oProperty.RenderToOOX(oNewParam);

	sResult += L"</w:body>";
	sResult += L"</w:document>";

	return sResult;
}

bool OOXDocumentWriter::SaveByItemStart( std::wstring sFolder )
{
	std::wstring pathWord = sFolder + FILE_SEPARATOR_STR + L"word";
    NSDirectory::CreateDirectory(pathWord) ;

	try
	{
		std::wstring sFilename = pathWord + FILE_SEPARATOR_STR + L"document.xml";
		m_oFileWriter = new NFileWriter::CBufferedFileWriter( sFilename );
	}
	catch(...)
	{
		return false;
	}
	m_oWriter.m_oRels.AddRelationship( L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument", L"word/document.xml" );
	m_oWriter.m_oContentTypes.AddContent( L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml", L"/word/document.xml" );

	std::wstring sXml = CreateXmlStart( );
    std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

    m_oFileWriter->Write((BYTE*)sXmlUTF.c_str(), sXmlUTF.length());
    return true;
}
bool OOXDocumentWriter::SaveBySection()
{
	if( m_oDocument.GetCount() < 1) 
		return false;

	RenderParameter oNewParam;

	oNewParam.poDocument	= &m_oDocument;
	oNewParam.poWriter		= &m_oWriter;
	oNewParam.poRels		= &m_oWriter.m_oDocRels;
	oNewParam.nType			= RENDER_TO_OOX_PARAM_UNKNOWN;

	std::wstring sXml, sXmlSectProp;	
	
	bool bParaPrevEmpty = false, bParaCurrEmpty = false;

	for (int i = 0; i < m_oDocument[0].props->GetCount(); i++)
	{
		if (i == m_oDocument[0].props->GetCount() - 1 && m_oDocument.GetCount() > 1)
		{
			_section section;
			if (m_oDocument.GetItem(section, 0))
			{
				sXmlSectProp = section.props->RenderToOOX(oNewParam);
			}	
		}
		RtfParagraph *para = dynamic_cast<RtfParagraph *>(m_oDocument[0].props->operator[](i).get());
		bParaCurrEmpty = (para) ? (para->GetCount() < 1) : true;

		sXml = m_oDocument[0].props->operator[](i)->RenderToOOX(oNewParam);

		if (!sXml.empty() || !sXmlSectProp.empty())
		{
			if (sXml.empty())
			{
				sXml = L"<w:p><w:pPr>" + sXmlSectProp + L"</w:pPr></w:p>";
			}
			else
			{
				if (!sXmlSectProp.empty())
				{
					if (bParaPrevEmpty && bParaCurrEmpty)
					{
						sXml += L"<w:p><w:pPr>" + sXmlSectProp + L"</w:pPr></w:p>";
					}
					else
					{
						size_t nFind, nFindPict, pos = sXml.size();

						do
						{
							nFindPict	= sXml.rfind(L"<w:pict>", pos);
							nFind		= sXml.rfind(L"</w:pPr>", pos);
							pos = nFindPict - 1;
						}while(nFind != std::wstring::npos && nFindPict != std::wstring::npos && nFind > nFindPict);

						if( nFind != std::wstring::npos)
						{
							sXml.insert( nFind, sXmlSectProp );
						}
						else
						{
							nFind = sXml.rfind( L"<w:p>" );
							if( std::wstring::npos != nFind )
								sXml.insert( nFind + 5, L"<w:pPr>" + sXmlSectProp + L"</w:pPr>" );
						}
					}
				}	
			}
			
			std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);
			if (m_oFileWriter)
			{
				m_oFileWriter->Write((BYTE*)sXmlUTF.c_str(), sXmlUTF.length());
			}
		}
		bParaPrevEmpty = bParaCurrEmpty;
	}

	if (m_oDocument.GetCount() > 1 )
	{
		m_oDocument.RemoveItem( 0 ); //удаляем секцию кроме последней
	}
	else
	{
		m_oDocument[0].props->m_aArray.clear();
	}

	return true;
}
bool OOXDocumentWriter::SaveByItem()
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

		if( m_oDocument.GetCount() > 1)//если что-то есть в следующей секции значит предыдущая закончилась
		{
			std::wstring sXml, sectPr;

			_section section;
			if (m_oDocument.GetItem(section, 0))
			{
                sectPr = section.props->RenderToOOX(oNewParam);
			}

			if( m_oDocument[0].props->GetCount() > 0 )
			{
                sXml = m_oDocument[0].props->operator[](0)->RenderToOOX(oNewParam);
				
				size_t nFind, nFindPict, pos = sXml.size();

				do
				{
					nFindPict	= sXml.rfind(L"<w:pict>", pos);
					nFind		= sXml.rfind(L"</w:pPr>", pos);
					pos = nFindPict - 1;
				}while(std::wstring::npos != nFind && std::wstring::npos != nFindPict && nFind > nFindPict);


				if( std::wstring::npos != nFind )
				{
					sXml.insert( nFind, sectPr );
				}
				else
				{
					size_t Find = sXml.rfind( L"<w:p>" );
					if( std::wstring::npos != nFind )
						sXml.insert( nFind + 5, L"<w:pPr>" + sectPr + L"</w:pPr>" );
				}
			}
			else
			{
				//генерация ???
				sXml = L"<w:p><w:pPr>" + sectPr + L"</w:pPr></w:p>";
			}
	
			std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);
			m_oFileWriter->Write((BYTE*)sXmlUTF.c_str(), sXmlUTF.length());
			
			m_oDocument.RemoveItem( 0 ); //удаляем секцию
		}
		else if( m_oDocument.GetCount() > 0 && m_oDocument[0].props->GetCount() > 1 )//пишем параграф - один всегда  "прозапас для секций"
		{
			std::wstring sXml = m_oDocument[0].props->operator[](0)->RenderToOOX(oNewParam);
            std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

			if (m_oFileWriter)
			{
				m_oFileWriter->Write((BYTE*)sXmlUTF.c_str(), sXmlUTF.length());
			}
			m_oDocument[0].props->RemoveItem( 0 );//удаляем первый параграф
        }
	}
	return true;
}
bool OOXDocumentWriter::SaveByItemEnd()
{
	RenderParameter oNewParam;
	oNewParam.poDocument	= &m_oDocument;
	oNewParam.poWriter		= &m_oWriter;
	oNewParam.poRels		= &m_oWriter.m_oDocRels;
	oNewParam.nType			= RENDER_TO_OOX_PARAM_UNKNOWN;

	if( m_oDocument.GetCount() > 0 && m_oDocument[0].props->GetCount() > 0 )//дописываем последний параграф
	{
		std::wstring sXml = m_oDocument[0].props->operator[](0)->RenderToOOX(oNewParam);
		//удаляем первый параграф
		m_oDocument[0].props->RemoveItem( 0 );
        std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

        m_oFileWriter->Write((BYTE*)sXmlUTF.c_str(), sXmlUTF.length());
    }
	std::wstring sXml = CreateXmlEnd( );
    std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

	if (m_oFileWriter)
	{
		m_oFileWriter->Write((BYTE*)sXmlUTF.c_str(), sXmlUTF.length());
	}
    RELEASEOBJECT( m_oFileWriter );
	return true;
}

int OOXDocumentWriter::GetCountSections()
{
	return m_oDocument.GetCount();
}
int OOXDocumentWriter::GetCount()
{
	int nCount = 0;
	for( int i = 0; i < m_oDocument.GetCount(); i++ )
	{
		nCount += m_oDocument[i].props->GetCount();
	}
	return nCount;
}
