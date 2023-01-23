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
#pragma once

#include "OOXDocumentReader.h"
#include "OOXSettingsReader.h"
#include "OOXSectionPropertyReader.h"
#include "OOXTextItemReader.h"
#include "OOXShapeReader.h"
#include "OOXMathReader.h"
#include "OOXFontReader.h"

#include "../../../OOXML/DocxFormat/Document.h"
#include "../../../OOXML/DocxFormat/Math/oMathContent.h"
#include "../../../OOXML/DocxFormat/Logic/Paragraph.h"
#include "../../../OOXML/DocxFormat/Logic/ParagraphProperty.h"
#include "../../../OOXML/DocxFormat/Logic/SectionProperty.h"

#include "../../Format/RtfDocument.h"

OOXDocumentReader::OOXDocumentReader(OOX::CDocument* ooxDocument)
{
	m_ooxDocument = ooxDocument;
}
OOXDocumentReader::~OOXDocumentReader()
{
}
bool OOXDocumentReader::Parse(ReaderParameter oParam)
{
	if (m_ooxDocument == NULL) return false;

	m_poReader = oParam.oReader;
	m_poDocument = oParam.oRtf;

	if (m_ooxDocument->m_oBackground.IsInit())
	{
		m_poDocument->m_pBackground = RtfShapePtr(new RtfShape());
		OOXBackgroundReader oBackgroundReader(m_ooxDocument->m_oBackground.GetPointer());
		oBackgroundReader.Parse(oParam, m_poDocument->m_pBackground);
	}

	if (false == m_ooxDocument->m_arrSections.empty())
	{
		for (size_t i = 0; i < m_ooxDocument->m_arrSections.size(); ++i)
		{
			_section section;

			section.props = RtfSectionPtr(new RtfSection());

			section.props->m_oProperty.SetDefaultOOX();
			section.start_para = m_ooxDocument->m_arrSections[i].start_elm;
			section.end_para = m_ooxDocument->m_arrSections[i].end_elm;

			OOXSectionPropertyReader oSectReader(dynamic_cast<OOX::Logic::CSectionProperty*>(m_ooxDocument->m_arrSections[i].sect));
			if (true == oSectReader.Parse(oParam, section.props->m_oProperty))
			{
				m_poDocument->AddItem(section);
			}
		}
	}
	else
	{
		size_t last_section_start = 0;

		//считаем количесво секций и заполняем их свойства .. 
		for (size_t i = 0; i < m_ooxDocument->m_arrItems.size(); ++i)
		{
			if (m_ooxDocument->m_arrItems[i] == NULL) continue;

			if (m_ooxDocument->m_arrItems[i]->getType() == OOX::et_w_p)
			{
				OOX::Logic::CParagraph * para = dynamic_cast<OOX::Logic::CParagraph *>(m_ooxDocument->m_arrItems[i]);

				if ((para) && (para->m_oParagraphProperty))
				{
					if (para->m_oParagraphProperty->m_oSectPr.IsInit())
					{
						_section section;
						section.props = RtfSectionPtr(new RtfSection());
						section.start_para = last_section_start;
						section.end_para = i;
						section.end_para++;

						last_section_start = i; last_section_start++;

						section.props->m_oProperty.SetDefaultOOX();

						OOXSectionPropertyReader oSectReader(para->m_oParagraphProperty->m_oSectPr.GetPointer());
						if (true == oSectReader.Parse(oParam, section.props->m_oProperty))
						{
							m_poDocument->AddItem(section);
						}
					}
				}
			}
		}
		//----------------------------------------------------------------------------------------------------------
		_section section;

		section.props = RtfSectionPtr(new RtfSection());
		section.start_para = last_section_start;
		section.end_para = m_ooxDocument->m_arrItems.size();

		section.props->m_oProperty.SetDefaultOOX();
		if (m_ooxDocument->m_oSectPr.IsInit())// свойства последней секции
		{
			OOXSectionPropertyReader oSectReader(m_ooxDocument->m_oSectPr.GetPointer());
			if (oSectReader.Parse(oParam, section.props->m_oProperty))
			{
				m_poDocument->AddItem(section);
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------------
	m_poDocument->RemoveItem(0); //бланковый при инициализации

	for (int sect = 0; sect < m_poDocument->GetCount(); sect++)
	{
		m_oTextItemReader.m_oTextItems = m_poDocument->m_aArray[sect].props;

		for (size_t i = m_poDocument->m_aArray[sect].start_para; i < m_poDocument->m_aArray[sect].end_para; ++i)
		{
			m_oTextItemReader.Parse(m_ooxDocument->m_arrItems[i], oParam);
		}
	}
	return true;
}

OOXSettingsReader::OOXSettingsReader(OOX::CSettings* ooxSettings)
{
	m_ooxSettings = ooxSettings;
}
bool OOXSettingsReader::Parse(ReaderParameter oParam)
{
	//oParam.oRtf->m_oColorSchemeMapping.SetDefaultOOX();

	if (m_ooxSettings == NULL) return false;

	if (m_ooxSettings->m_oMathPr.IsInit())
	{
		oParam.oRtf->m_pMathProp = RtfMathPtr(new RtfMath());

		OOXMathReader oMathReader(NULL);
		oMathReader.ParseElement(oParam, m_ooxSettings->m_oMathPr.GetPointer(), oParam.oRtf->m_pMathProp);
	}

	if (m_ooxSettings->m_oDoNotHyphenateCaps.IsInit())
	{
		oParam.oRtf->m_oProperty.m_bHypCaps = !m_ooxSettings->m_oDoNotHyphenateCaps->m_oVal.ToBool() ? 1 : 0;
	}
	if (m_ooxSettings->m_oAutoHyphenation.IsInit())
	{
		oParam.oRtf->m_oProperty.m_bAutoHyp = m_ooxSettings->m_oAutoHyphenation->m_oVal.ToBool() ? 1 : 0;
	}
	if (m_ooxSettings->m_oConsecutiveHyphenLimit.IsInit() && m_ooxSettings->m_oConsecutiveHyphenLimit->m_oVal.IsInit())
	{
		oParam.oRtf->m_oProperty.m_nMaxHypen = *m_ooxSettings->m_oConsecutiveHyphenLimit->m_oVal;
	}
	if (m_ooxSettings->m_oHyphenationZone.IsInit() && m_ooxSettings->m_oHyphenationZone->m_oVal.IsInit())
	{
		oParam.oRtf->m_oProperty.m_nHyphenationRight = m_ooxSettings->m_oHyphenationZone->m_oVal->ToTwips(); //todooo проверить размерность
	}
	if (m_ooxSettings->m_oDefaultTabStop.IsInit() && m_ooxSettings->m_oDefaultTabStop->m_oVal.IsInit())
	{
		oParam.oRtf->m_oProperty.m_nTabWidth = m_ooxSettings->m_oDefaultTabStop->m_oVal->ToTwips();//todooo
	}
	if (m_ooxSettings->m_oBordersDoNotSurroundHeader.IsInit())
	{
		oParam.oRtf->m_oProperty.m_bDorderSurroundHeader = !m_ooxSettings->m_oBordersDoNotSurroundHeader->m_oVal.ToBool() ? 1 : 0;
	}
	if (m_ooxSettings->m_oBordersDoNotSurroundFooter.IsInit())
	{
		oParam.oRtf->m_oProperty.m_bDorderSurroundFotter = !m_ooxSettings->m_oBordersDoNotSurroundFooter->m_oVal.ToBool() ? 1 : 0;
	}
	if (m_ooxSettings->m_oAlignBordersAndEdges.IsInit())
	{
		oParam.oRtf->m_oProperty.m_bAlignBordersAndEdges = m_ooxSettings->m_oAlignBordersAndEdges->m_oVal.ToBool() ? 1 : 0;
	}
	if (m_ooxSettings->m_oGutterAtTop.IsInit())
	{
		oParam.oRtf->m_oProperty.m_bGutterAtTop = m_ooxSettings->m_oGutterAtTop->m_oVal.ToBool() ? 1 : 0;
	}
	if (m_ooxSettings->m_oMirrorMargins.IsInit())
	{
		oParam.oRtf->m_oProperty.m_bSwitchMargins = m_ooxSettings->m_oMirrorMargins->m_oVal.ToBool() ? 1 : 0;
	}
	if (m_ooxSettings->m_oEvenAndOddHeaders.IsInit())
	{
		oParam.oRtf->m_oProperty.m_bFacingPage = m_ooxSettings->m_oEvenAndOddHeaders->m_oVal.ToBool() ? 1 : 0;
	}
	if (m_ooxSettings->m_oDisplayBackgroundShape.IsInit())
	{
		oParam.oRtf->m_oProperty.m_nDisplayBackground = m_ooxSettings->m_oDisplayBackgroundShape->m_oVal.ToBool() ? 1 : 0;
	}
	if (m_ooxSettings->m_oCompat.IsInit())
	{
		// todooo - реализовать в DocxFormat

		//if( L"w:doNotUseHTMLParagraphAutoSpacing" == sNodeName )
		//	oParam.oRtf->m_oProperty.m_bHtmlAutoSpace = 0;
		//else if( L"w:forgetLastTabAlignment" == sNodeName )
		//	oParam.oRtf->m_oProperty.m_bUseTabAlignment = 0;
	}
	if (m_ooxSettings->m_oFootnotePr.IsInit())
	{
		for (size_t i = 0; i < m_ooxSettings->m_oFootnotePr->m_arrFootnote.size(); i++)
		{
			if (m_ooxSettings->m_oFootnotePr->m_arrFootnote[i]->m_oId.IsInit())
			{
				oParam.oRtf->m_oProperty.m_aSpecialFootnotes.push_back(m_ooxSettings->m_oFootnotePr->m_arrFootnote[i]->m_oId->GetValue());
			}
		}
		if (m_ooxSettings->m_oFootnotePr->m_oNumFmt.IsInit() && m_ooxSettings->m_oFootnotePr->m_oNumFmt->m_oVal.IsInit())
		{
			oParam.oRtf->m_oProperty.m_nFootnoteNumberingFormat = RtfListLevelProperty::GetFormat(m_ooxSettings->m_oFootnotePr->m_oNumFmt->m_oVal->ToString());
		}
		if (m_ooxSettings->m_oFootnotePr->m_oNumRestart.IsInit() && m_ooxSettings->m_oFootnotePr->m_oNumRestart->m_oVal.IsInit())
		{
			switch (m_ooxSettings->m_oFootnotePr->m_oNumRestart->m_oVal->GetValue())
			{
			case SimpleTypes::restartnumberEachPage:
				oParam.oRtf->m_oProperty.m_eFootnoteRestart = RtfDocumentProperty::fr_EachPage; break;
			case SimpleTypes::restartnumberEachSect:
				oParam.oRtf->m_oProperty.m_eFootnoteRestart = RtfDocumentProperty::fr_EachSection; break;
			case SimpleTypes::restartnumberContinious:
			default:
				oParam.oRtf->m_oProperty.m_eFootnoteRestart = RtfDocumentProperty::fr_Continuous; break;
			}
		}
		if (m_ooxSettings->m_oFootnotePr->m_oNumStart.IsInit() && m_ooxSettings->m_oFootnotePr->m_oNumStart->m_oVal.IsInit())
		{
			oParam.oRtf->m_oProperty.m_nFootnoteStart = *m_ooxSettings->m_oFootnotePr->m_oNumStart->m_oVal;
		}
		if (m_ooxSettings->m_oFootnotePr->m_oPos.IsInit() && m_ooxSettings->m_oFootnotePr->m_oPos->m_oVal.IsInit())
		{
			switch (m_ooxSettings->m_oFootnotePr->m_oPos->m_oVal->GetValue())
			{
			case SimpleTypes::ftnposBeneathText:
				oParam.oRtf->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_BeneathText; break;
			case SimpleTypes::ftnposDocEnd:
				oParam.oRtf->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_EndDocument; break;
			case SimpleTypes::ftnposPageBottom:
				oParam.oRtf->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_BottomPage; break;
			case SimpleTypes::ftnposSectEnd:
			default:
				oParam.oRtf->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_EndSection; break;
			}
		}
	}

	if (m_ooxSettings->m_oEndnotePr.IsInit())
	{
		for (size_t i = 0; i < m_ooxSettings->m_oEndnotePr->m_arrEndnote.size(); i++)
		{
			if (m_ooxSettings->m_oEndnotePr->m_arrEndnote[i]->m_oId.IsInit())
			{
				oParam.oRtf->m_oProperty.m_aSpecialEndnotes.push_back(m_ooxSettings->m_oEndnotePr->m_arrEndnote[i]->m_oId->GetValue());
			}
		}
		if (m_ooxSettings->m_oEndnotePr->m_oNumFmt.IsInit() && m_ooxSettings->m_oEndnotePr->m_oNumFmt->m_oVal.IsInit())
		{
			oParam.oRtf->m_oProperty.m_nEndnoteNumberingFormat = RtfListLevelProperty::GetFormat(m_ooxSettings->m_oEndnotePr->m_oNumFmt->m_oVal->ToString());
		}
		if (m_ooxSettings->m_oEndnotePr->m_oNumRestart.IsInit() && m_ooxSettings->m_oEndnotePr->m_oNumRestart->m_oVal.IsInit())
		{
			switch (m_ooxSettings->m_oEndnotePr->m_oNumRestart->m_oVal->GetValue())
			{
			case SimpleTypes::restartnumberEachSect:
				oParam.oRtf->m_oProperty.m_eEndnoteRestart = RtfDocumentProperty::er_EachSection; break;
			case SimpleTypes::restartnumberContinious:
			default:
				oParam.oRtf->m_oProperty.m_eEndnoteRestart = RtfDocumentProperty::er_Continuous; break;
			}
		}
		if (m_ooxSettings->m_oEndnotePr->m_oNumStart.IsInit() && m_ooxSettings->m_oEndnotePr->m_oNumStart->m_oVal.IsInit())
		{
			oParam.oRtf->m_oProperty.m_nEndnoteStart = *m_ooxSettings->m_oEndnotePr->m_oNumStart->m_oVal;
		}
		if (m_ooxSettings->m_oEndnotePr->m_oPos.IsInit() && m_ooxSettings->m_oEndnotePr->m_oPos->m_oVal.IsInit())
		{
			switch (m_ooxSettings->m_oEndnotePr->m_oPos->m_oVal->GetValue())
			{
			case SimpleTypes::ftnposBeneathText:
				oParam.oRtf->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_BeneathText; break;
			case SimpleTypes::ftnposDocEnd:
				oParam.oRtf->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_EndDocument; break;
			case SimpleTypes::ftnposPageBottom:
				oParam.oRtf->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_BottomPage; break;
			case SimpleTypes::ftnposSectEnd:
			default:
				oParam.oRtf->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_EndSection; break;
			}
		}
	}
	return true;
}