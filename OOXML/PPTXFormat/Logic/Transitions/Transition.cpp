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

				XmlUtils::CXmlNodes oNodes;
				
				if (node.GetNodes(_T("*"), oNodes))
				{
					int count = oNodes.GetCount();
					
					for (int i = 0; i < count; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

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
<mc:Choice xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" Requires=\"p14\">"));

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
