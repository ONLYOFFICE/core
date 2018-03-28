/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

#include "BinWriters.h"

namespace BinDocxRW
{
	BinaryHeaderFooterTableWriter::BinaryHeaderFooterTableWriter(ParamsWriter& oParamsWriter, OOX::IFileContainer* oDocumentRels, std::map<int, bool>* mapIgnoreComments):
	m_oBcw(oParamsWriter), m_oParamsWriter(oParamsWriter), m_poTheme(oParamsWriter.m_poTheme), m_oFontProcessor(*oParamsWriter.m_pFontProcessor), m_oSettings(oParamsWriter.m_oSettings), m_pOfficeDrawingConverter(oParamsWriter.m_pOfficeDrawingConverter), m_oDocumentRels(oDocumentRels),m_mapIgnoreComments(mapIgnoreComments)
	{
	}
	void BinaryHeaderFooterTableWriter::Write()
	{
		int nStart = m_oBcw.WriteItemWithLengthStart();
		int nCurPos = 0;
		//Header
		if(m_aHeaders.size() > 0)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerHdrFtrTypes::Header);
			WriteHdrFtrContent(m_aHeaders, m_aHeaderTypes, m_aHeaderSectPrs, true);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		//Footer
		if(m_aFooters.size() > 0)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerHdrFtrTypes::Footer);
			WriteHdrFtrContent(m_aFooters, m_aFooterTypes, m_aFooterSectPrs, false);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		m_oBcw.WriteItemWithLengthEnd(nStart);
	}
	void BinaryHeaderFooterTableWriter::WriteHdrFtrContent(std::vector<OOX::CHdrFtr*>& aHdrFtrs, std::vector<SimpleTypes::EHdrFtr>& aHdrFtrTypes, std::vector<OOX::Logic::CSectionProperty*>& aHdrSectPrs, bool bHdr)
	{
		int nCurPos = 0;
		for(size_t i = 0; i < aHdrFtrs.size(); ++i)
		{
			OOX::CHdrFtr* pHdrFtr = aHdrFtrs[i];
			SimpleTypes::EHdrFtr eType = aHdrFtrTypes[i];
			OOX::Logic::CSectionProperty* pSectPr = aHdrSectPrs[i];
			BYTE byteHdrFtrType = c_oSerHdrFtrTypes::HdrFtr_Odd;
			switch(eType)
			{
				case SimpleTypes::hdrftrFirst:	byteHdrFtrType = c_oSerHdrFtrTypes::HdrFtr_First;	break;
				case SimpleTypes::hdrftrEven:	byteHdrFtrType = c_oSerHdrFtrTypes::HdrFtr_Even;	break;
				default:						byteHdrFtrType = c_oSerHdrFtrTypes::HdrFtr_Odd;		break;
			}
			nCurPos = m_oBcw.WriteItemStart(byteHdrFtrType);
			WriteHdrFtrItem(pSectPr, pHdrFtr, bHdr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryHeaderFooterTableWriter::WriteHdrFtrItem(OOX::Logic::CSectionProperty* pSectPr, OOX::CHdrFtr* pHdrFtr, bool bHdr)
	{
		int nCurPos = 0;
	//Content
		ParamsDocumentWriter oParamsDocumentWriter(pHdrFtr);
		m_oParamsWriter.m_pCurRels = oParamsDocumentWriter.m_pRels;

		BinaryDocumentTableWriter oBinaryDocumentTableWriter(m_oParamsWriter, oParamsDocumentWriter, m_mapIgnoreComments, NULL);
		
		smart_ptr<OOX::IFileContainer> oldRels = m_pOfficeDrawingConverter->GetRels();
		m_pOfficeDrawingConverter->SetRels(oParamsDocumentWriter.m_pRels);
		m_pOfficeDrawingConverter->ClearShapeTypes();

		nCurPos = m_oBcw.WriteItemStart(c_oSerHdrFtrTypes::HdrFtr_Content);
		oBinaryDocumentTableWriter.WriteDocumentContent(pHdrFtr->m_arrItems);
		m_oBcw.WriteItemEnd(nCurPos);

		m_pOfficeDrawingConverter->SetRels(oldRels);
	}
}
