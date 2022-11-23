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

#include "Converter.h"

#include "TextboxMapping.h"
#include "TableMapping.h"
#include "FooterMapping.h"
#include "HeaderMapping.h"
#include "MainDocumentMapping.h"

namespace DocFileFormat
{
	int TextboxMapping::TextboxCount = 0;

	TextboxMapping::TextboxMapping (ConversionContext* ctx, int nTBIndex, XMLTools::CStringXmlWriter* writer, IMapping* caller ): DocumentMapping( ctx, writer, caller ), m_nTBIndex(0)
	{
		TextboxCount++;
		m_nTBIndex	=	nTBIndex;
		SetInset(-1,-1,-1,-1);
	}


	TextboxMapping::TextboxMapping (ConversionContext* ctx, XMLTools::CStringXmlWriter* writer, IMapping* caller) : DocumentMapping( ctx, writer, caller ), m_nTBIndex(0)
	{
		TextboxCount++;
		m_nTBIndex	=	TextboxCount - 1;
		SetInset(-1,-1,-1,-1);
	}

	TextboxMapping::~TextboxMapping()
	{
		//TextboxCount--;  
	}
}

namespace DocFileFormat
{	
	void TextboxMapping::SetTextboxStyle(const std::wstring & style)
	{
		m_sTextBoxStyle = style;
	}

	void TextboxMapping::SetInset (int nLeft, int nTop, int nRight, int nBottom)
	{
		//emu to pt (1 pt = 12700)
		m_dxTextLeft	=	nLeft	/ 12700.;
		m_dyTextTop		=	nTop	/ 12700.;
		m_dxTextRight	=	nRight	/ 12700.;
		m_dyTextBottom	=	nBottom	/ 12700.;
	}

	void TextboxMapping::Apply (IVisitable* visited)
	{
		m_document = static_cast<WordDocument*>(visited);

		if (!m_document) return;

		m_pXmlWriter->WriteNodeBegin(L"v:textbox", true);
		if (m_dxTextLeft >= 0 && m_dyTextTop >= 0 && m_dxTextRight >= 0 && m_dyTextBottom >= 0)
		{
			m_pXmlWriter->WriteAttribute(L"inset", FormatUtils::DoubleToWideString(m_dxTextLeft) + L"pt," +
				FormatUtils::DoubleToWideString(m_dyTextTop) + L"pt," +
				FormatUtils::DoubleToWideString(m_dxTextRight) + L"pt," +
				FormatUtils::DoubleToWideString(m_dyTextBottom) + L"pt");
		}
		if (!m_sTextBoxStyle.empty())
		{
			m_pXmlWriter->WriteAttribute(L"style", m_sTextBoxStyle);
		}
		m_pXmlWriter->WriteNodeEnd(L"", true, false);

		m_pXmlWriter->WriteNodeBegin(L"w:txbxContent");

		int cpStart = 0;
		int cpEnd = 0;
		Tbkd* bkd = NULL;

		int txtbxSubdocStart =
			m_document->FIB->m_RgLw97.ccpText +
			m_document->FIB->m_RgLw97.ccpFtn +
			m_document->FIB->m_RgLw97.ccpHdr +
			m_document->FIB->m_RgLw97.ccpAtn +
			m_document->FIB->m_RgLw97.ccpEdn;

		if (typeid(*_caller) == typeid(MainDocumentMapping))
		{
			if (m_nTBIndex < m_document->TextboxBreakPlex->Elements.size())//file(21).doc
			{
				bkd = static_cast<Tbkd*>(m_document->TextboxBreakPlex->Elements[m_nTBIndex]);
			}
			
			if (m_nTBIndex + 1 < m_document->TextboxIndividualPlex->CharacterPositions.size())
			{//Jukivka.doc
				cpStart = txtbxSubdocStart + m_document->TextboxIndividualPlex->CharacterPositions[m_nTBIndex];
				cpEnd = txtbxSubdocStart + m_document->TextboxIndividualPlex->CharacterPositions[m_nTBIndex + 1];
			}		
			else if (m_nTBIndex + 1 < m_document->TextboxBreakPlex->CharacterPositions.size())
			{
				cpStart = txtbxSubdocStart + m_document->TextboxBreakPlex->CharacterPositions[m_nTBIndex];
				cpEnd = txtbxSubdocStart + m_document->TextboxBreakPlex->CharacterPositions[m_nTBIndex + 1];
			}

		}
		else if ((typeid(*_caller) == typeid(HeaderMapping)) || (typeid(*_caller) == typeid(FooterMapping)))
		{
			txtbxSubdocStart += m_document->FIB->m_RgLw97.ccpTxbx;

			bkd = static_cast<Tbkd*>(m_document->TextboxBreakPlexHeader->Elements[m_nTBIndex]);

			cpStart = txtbxSubdocStart + m_document->TextboxBreakPlexHeader->CharacterPositions[m_nTBIndex];
			cpEnd = txtbxSubdocStart + m_document->TextboxBreakPlexHeader->CharacterPositions[m_nTBIndex + 1];
		}
		bool bUsed = bkd ? bkd->bUsed : false;
		_isTextBoxContent = true;

		//convert the textbox text
		_lastValidPapx = (*(m_document->AllPapxFkps->begin()))->grppapx[0];

		ParagraphPropertyExceptions* papx_prev = NULL;
		int cp = cpStart;
		while (cp < cpEnd && cp >= cpStart && !bUsed)
		{
			int fc = m_document->FindFileCharPos(cp);
			if (fc < 0) break;

			ParagraphPropertyExceptions* papx = findValidPapx(fc);
			if (papx_prev && papx_prev == papx)//file(21).doc
				break;
			papx_prev = papx;

			TableInfo tai(papx, m_document->nWordVersion);

			if (tai.fInTable)
			{
				//this PAPX is for a table
				Table table(this, cp, ((tai.iTap > 0) ? (1) : (0)));
				table.Convert(this);
				cp = table.GetCPEnd();
			}
			else
			{
				//this PAPX is for a normal paragraph
				cp = writeParagraph(cp, 0x7fffffff);
			}
		}
		_isTextBoxContent = false;

		if (bkd)
			bkd->bUsed = true;

		m_pXmlWriter->WriteNodeEnd(L"w:txbxContent");
		m_pXmlWriter->WriteNodeEnd(L"v:textbox");
	}
}
