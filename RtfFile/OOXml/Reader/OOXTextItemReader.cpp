/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once
#include "OOXTextItemReader.h"

#include "../../Format/RtfDocument.h"
#include "../../Format/RtfTable.h"

#include "OOXTableReader.h"
#include "OOXParagraphReader.h"

#include "../../../OOXML/DocxFormat/Logic/Sdt.h"
#include "../../../OOXML/DocxFormat/Logic/Table.h"
#include "../../../OOXML/DocxFormat/Logic/Paragraph.h"
#include "../../../OOXML/DocxFormat/Logic/ParagraphProperty.h"

OOXTextItemReader::OOXTextItemReader()
{
	m_oTextItems = TextItemContainerPtr(new TextItemContainer());
}
bool OOXTextItemReader::Parse(OOX::WritingElement* ooxElement, ReaderParameter oParam)
{
	switch (ooxElement->getType())
	{
		case OOX::et_a_p:
		{
			PPTX::Logic::Paragraph * pParagraph = dynamic_cast<PPTX::Logic::Paragraph*>(ooxElement);

			OOXParagraphReader	oParagraphReader(pParagraph);
			RtfParagraphPtr oNewParagraph(new RtfParagraph());
			//apply default property to new paragraph
			oNewParagraph->m_oProperty = oParam.oRtf->m_oDefaultParagraphProp;
			oNewParagraph->m_oProperty.m_oCharProperty = oParam.oRtf->m_oDefaultCharProp;
			oNewParagraph->m_oProperty.m_nItap = 0;

			if (true == oParagraphReader.Parse(oParam, (*oNewParagraph), CcnfStyle()))
			{
				m_oTextItems->AddItem(oNewParagraph);
			}
		}break;
		case OOX::et_w_p:
		{
			OOX::Logic::CParagraph * pParagraph = dynamic_cast<OOX::Logic::CParagraph*>(ooxElement);

			OOXParagraphReader	oParagraphReader(pParagraph);
			RtfParagraphPtr oNewParagraph(new RtfParagraph());
		//apply default property to new paragraph
			oNewParagraph->m_oProperty = oParam.oRtf->m_oDefaultParagraphProp;
			oNewParagraph->m_oProperty.m_oCharProperty = oParam.oRtf->m_oDefaultCharProp;
			
			if (oParam.oReader->m_bInTable)
			{
				if (NULL != oParam.poTableStyle)
					oNewParagraph->m_oProperty.m_nTableStyle = oParam.poTableStyle->m_nID;
				oNewParagraph->m_oProperty.m_bInTable = 1;
				oNewParagraph->m_oProperty.m_nItap = oParam.oReader->m_nCurItap;
			}
			else
			{
				oNewParagraph->m_oProperty.m_nItap = 0;
			}

			if (true == oParagraphReader.Parse(oParam, (*oNewParagraph), CcnfStyle()))
			{
				m_oTextItems->AddItem(oNewParagraph);
			}


		}break;
		case OOX::et_w_tbl:
		{
			OOX::Logic::CTbl * pTbl = dynamic_cast<OOX::Logic::CTbl*>(ooxElement);
			RtfTablePtr oNewTable(new RtfTable());

			OOXTableReader oTableReader(pTbl);
			oParam.oReader->m_nCurItap = 1;
			if (true == oTableReader.Parse(oParam, (*oNewTable)))
			{
				m_oTextItems->AddItem(oNewTable);
			}
			oParam.oReader->m_nCurItap = 0;
		}break;
		case OOX::et_w_sdt:
		{
			OOX::Logic::CSdt * pSdt = dynamic_cast<OOX::Logic::CSdt*>(ooxElement);
			if (pSdt->m_oSdtEndPr.IsInit())
			{
				//todo
			}
			if (pSdt->m_oSdtContent.IsInit())
			{
				Parse(pSdt->m_oSdtContent.GetPointer(), oParam);
			}
		}break;
		case OOX::et_w_sdtContent:
		{
			OOX::Logic::CSdtContent * pSdt = dynamic_cast<OOX::Logic::CSdtContent*>(ooxElement);

			for (std::vector<OOX::WritingElement*>::iterator it = pSdt->m_arrItems.begin(); it != pSdt->m_arrItems.end(); ++it)
			{
				Parse(*it, oParam);
			}

		}break;
		case OOX::et_w_commentRangeStart:
		case OOX::et_w_commentReference:
		case OOX::et_w_commentRangeEnd:
		{
			OOX::Logic::CParagraph oParagraph;
			oParagraph.m_oParagraphProperty = new OOX::Logic::CParagraphProperty();
			oParagraph.m_arrItems.push_back(ooxElement);

			OOXParagraphReader	oParagraphReader(&oParagraph);
			RtfParagraphPtr oNewParagraph(new RtfParagraph());

			oParagraph.m_oParagraphProperty->m_oKeepNext.Init();
			oParagraph.m_oParagraphProperty->m_oKeepNext->m_oVal.SetValue(SimpleTypes::EOnOff::onoffTrue);

			oParagraph.m_oParagraphProperty->m_oKeepLines.Init();
			oParagraph.m_oParagraphProperty->m_oKeepLines->m_oVal.SetValue(SimpleTypes::EOnOff::onoffTrue);

			//apply default property to new paragraph
			oNewParagraph->m_oProperty = oParam.oRtf->m_oDefaultParagraphProp;
			oNewParagraph->m_oProperty.m_oCharProperty = oParam.oRtf->m_oDefaultCharProp;
			oNewParagraph->m_oProperty.m_nItap = 0;

			if (true == oParagraphReader.Parse(oParam, (*oNewParagraph), CcnfStyle()))
			{
				m_oTextItems->AddItem(oNewParagraph);
			}

			oParagraph.m_arrItems.clear();
			delete oParagraph.m_oParagraphProperty; oParagraph.m_oParagraphProperty = NULL;
		}break;
		default:
		{

		}break;
	}
	return true;
}
