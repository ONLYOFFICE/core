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
#include "Si.h"

#include "../../../../../ASCOfficePPTXFile/Editor/BinaryFileReaderWriter.h"

namespace OOX
{
	namespace Spreadsheet
	{

		void CSi::fromXLSBExt (NSBinPptxRW::CBinaryFileReader& oStream)
		{
			_UINT32 nCount = oStream.GetULong();
			while(nCount-- > 0)
			{
				BYTE nTypeRun = oStream.GetUChar();
				if(0x1 == nTypeRun)
				{
					OOX::Spreadsheet::CRun* pRun = new OOX::Spreadsheet::CRun();
					if(oStream.GetBool())
					{
						pRun->m_oRPr.Init();
						pRun->m_oRPr->fromXLSB(oStream, XLSB::rt_FONT);
					}
					_UINT32 nTextCount = oStream.GetULong();
					while(nTextCount-- > 0)
					{
						OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
						pText->m_sText.append(oStream.GetString2());
						pRun->m_arrItems.push_back(pText);
					}
					m_arrItems.push_back(pRun);
				}
				else if(0x2 == nTypeRun)
				{
					OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
					pText->m_sText.append(oStream.GetString2());
					m_arrItems.push_back(pText);
				}
			}
		}

		void CSi::toXLSBExt (NSBinPptxRW::CXlsbBinaryWriter& oStream)
		{
			//it's not by XLSB format
			oStream.WriteULONG(m_arrItems.size());
			for(size_t i = 0; i < m_arrItems.size(); ++i)
			{
				OOX::Spreadsheet::WritingElement* we = m_arrItems[i];
				if(OOX::et_x_r == we->getType())
				{
					oStream.WriteBYTE(0x1);
					OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(we);
					oStream.WriteBOOL(pRun->m_oRPr.IsInit());
					if(pRun->m_oRPr.IsInit())
					{
						pRun->m_oRPr->toXLSB(oStream);
					}
					oStream.WriteULONG(pRun->m_arrItems.size());
					for(size_t j = 0, length = pRun->m_arrItems.size(); j < length; ++j)
					{
						OOX::Spreadsheet::WritingElement* we = pRun->m_arrItems[j];
						if(OOX::et_x_t == we->getType())
						{
							OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(we);
							oStream.WriteString(pText->m_sText);
						}
						else
						{
							oStream.WriteString(L"");
						}
					}
				}
				else if(OOX::et_x_t == we->getType())
				{
					OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(we);
					oStream.WriteBYTE(0x2);
					oStream.WriteString(pText->m_sText);
				}
				else
				{
					oStream.WriteBYTE(0);
				}
			}
		}
		_UINT32 CSi::getXLSBSize() const
		{
			_UINT32 nLen = 4;
			for(size_t i = 0; i < m_arrItems.size(); ++i)
			{
				nLen += 1;
				OOX::Spreadsheet::WritingElement* we = m_arrItems[i];
				if(OOX::et_x_r == we->getType())
				{
					nLen += 1;
					OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(we);
					if(pRun->m_oRPr.IsInit())
					{
						nLen += pRun->m_oRPr->getXLSBSize();
					}
					nLen += 4;
					for(size_t j = 0, length = pRun->m_arrItems.size(); j < length; ++j)
					{
						OOX::Spreadsheet::WritingElement* we = pRun->m_arrItems[j];
						if(OOX::et_x_t == we->getType())
						{
							OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(we);
							nLen += 4 + 2 * pText->m_sText.length();
						}
						else
						{
							nLen += 4;
						}
					}
				}
				else if(OOX::et_x_t == we->getType())
				{
					OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(we);
					nLen += 4 + 2 * pText->m_sText.length();
				}
			}
			return nLen;
		}

	} //Spreadsheet
} // OOX
