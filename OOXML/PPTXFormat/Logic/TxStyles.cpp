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

#include "TxStyles.h"

namespace PPTX
{
	namespace Logic
	{
		void TxStyles::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t nCount = oNodes.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("titleStyle") == strName)
						titleStyle = oNode;
					else if (_T("bodyStyle") == strName)
						bodyStyle = oNode;
					else if (_T("otherStyle") == strName)
						otherStyle = oNode;
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring TxStyles::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(titleStyle);
			oValue.WriteNullable(bodyStyle);
			oValue.WriteNullable(otherStyle);

			return XmlUtils::CreateNode(_T("p:txStyles"), oValue);
		}
		void TxStyles::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, titleStyle);
			pWriter->WriteRecord2(1, bodyStyle);
			pWriter->WriteRecord2(2, otherStyle);
		}
		void TxStyles::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetLong() + 4;

			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						titleStyle = new TextListStyle();
						titleStyle->m_name = _T("p:titleStyle");
						titleStyle->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						bodyStyle = new TextListStyle();
						bodyStyle->m_name = _T("p:bodyStyle");
						bodyStyle->fromPPTY(pReader);
						break;
					}
					case 2:
					{
						otherStyle = new TextListStyle();
						otherStyle->m_name = _T("p:otherStyle");
						otherStyle->fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(end);
		}
		void TxStyles::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:txStyles"));
			pWriter->EndAttributes();

			pWriter->Write(titleStyle);
			pWriter->Write(bodyStyle);
			pWriter->Write(otherStyle);

			pWriter->EndNode(_T("p:txStyles"));
		}
		void TxStyles::FillParentPointersForChilds()
		{
			if(titleStyle.is_init())
				titleStyle->SetParentPointer(this);
			if(bodyStyle.is_init())
				bodyStyle->SetParentPointer(this);
			if(otherStyle.is_init())
				otherStyle->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
