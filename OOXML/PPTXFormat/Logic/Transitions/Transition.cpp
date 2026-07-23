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

#include "Transition.h"

namespace PPTX
{
	namespace Logic
	{
			void Transition::fromXML(XmlUtils::CXmlNode& node)
			{
                XmlMacroReadAttributeBase(node, L"advClick", advClick);
                XmlMacroReadAttributeBase(node, L"advTm",	advTm);
                XmlMacroReadAttributeBase(node, L"p14:dur",	dur);
                XmlMacroReadAttributeBase(node, L"spd",		spd);

				std::vector<XmlUtils::CXmlNode> oNodes;
				
				if (node.GetNodes(_T("*"), oNodes))
				{
					size_t count = oNodes.size();
					
					for (size_t i = 0; i < count; ++i)
					{
						XmlUtils::CXmlNode& oNode = oNodes[i];

						std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (strName == L"sndAc")
							sndAc = oNode;
						else
						{
							trSerialize.fromXML(oNode);
							base.fromXML(oNode);
						}
					}
				}
				
				FillParentPointersForChilds();
			}
			std::wstring Transition::toXML() const
			{
				XmlUtils::CAttribute oAttr;
                oAttr.WriteLimitNullable(_T("spd"), spd);
				oAttr.Write(_T("advClick"), advClick);
				oAttr.Write(_T("advTm"), advTm);
				oAttr.Write(_T("dur"), dur);

				XmlUtils::CNodeValue oValue;
				oValue.Write(base);
				oValue.WriteNullable(sndAc);

				return XmlUtils::CreateNode(_T("p:transition"), oAttr, oValue);
			}
			void Transition::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

				pWriter->WriteBool2(0, advClick);
				pWriter->WriteInt2(1, advTm);
				pWriter->WriteInt2(2, dur);
				pWriter->WriteLimit2(3, spd);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				if (_T("") != trSerialize.m_strNodeName)
				{
					pWriter->WriteRecord1(0, trSerialize);
				}
			}
			void Transition::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG end = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // attribute start
				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
						advClick = pReader->GetBool();
					else if (1 == _at)
						advTm = pReader->GetLong();
					else if (2 == _at)
						dur = pReader->GetLong();
					else if (3 == _at)
						spd = pReader->GetUChar();
				}

				while (pReader->GetPos() < end)
				{
					BYTE _rec = pReader->GetUChar();

					switch (_rec)
					{
						case 0:
						{
							trSerialize.fromPPTY(pReader);
							break;
						}						
						default:
						{
							pReader->SkipRecord();
							break;
						}
					}
				}
				pReader->Seek(end);
			}
			void Transition::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(_T("<mc:AlternateContent xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\">\
<mc:Choice \
xmlns:p159=\"http://schemas.microsoft.com/office/powerpoint/2015/09/main\" \
xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" \
Requires=\"p159\">"));

				pWriter->StartNode(_T("p:transition"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("spd"), spd);
				pWriter->WriteAttribute(_T("p14:dur"), dur);
				pWriter->WriteAttribute(_T("advClick"), advClick);
				pWriter->WriteAttribute(_T("advTm"), advTm);
				pWriter->EndAttributes();

				if (trSerialize.m_strNodeName != _T(""))
					trSerialize.toXmlWriter(pWriter);

				pWriter->EndNode(_T("p:transition"));

				pWriter->WriteString(_T("</mc:Choice><mc:Fallback>"));

				pWriter->StartNode(_T("p:transition"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("spd"), spd);
				pWriter->WriteAttribute(_T("advClick"), advClick);
				pWriter->WriteAttribute(_T("advTm"), advTm);
				pWriter->EndAttributes();

				if (trSerialize.m_strNodeName != _T(""))
					trSerialize.toXmlWriterOld(pWriter);

				pWriter->EndNode(_T("p:transition"));

				pWriter->WriteString(_T("</mc:Fallback></mc:AlternateContent>"));
			}			
			void Transition::FillParentPointersForChilds()
			{
				if(base.is_init())
					base.SetParentPointer(this);
				if(sndAc.IsInit())
					sndAc->SetParentPointer(this);
			}
			void Transition::Normalize()
			{
				advTm.normalize_positive();
			}		
	} // namespace Logic
} // namespace PPTX
