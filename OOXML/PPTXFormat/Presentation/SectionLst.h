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

#include "../Logic/XmlId.h"

namespace PPTX
{
	namespace Logic
	{
		class Section : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Section)

			nullable_string				id;
			nullable_string				name;
			std::vector<Logic::XmlId>	arSldIdLst;

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlMacroReadAttributeBase(node, L"id", id);
				XmlMacroReadAttributeBase(node, L"name", name);

				XmlUtils::CXmlNode oNodeSldIdLst;
				if (node.GetNode(L"p14:sldIdLst", oNodeSldIdLst))
				{
					XmlMacroLoadArray(oNodeSldIdLst, L"p14:sldId", arSldIdLst, Logic::XmlId);
				}
			}

			virtual std::wstring toXML() const
			{
				return L"";
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(L"p14:section");

				pWriter->StartAttributes();
                pWriter->WriteAttribute2(L"name", name);
				pWriter->WriteAttribute(L"id", id);

				pWriter->EndAttributes();

				pWriter->StartNode(L"p14:sldIdLst");
				pWriter->EndAttributes();
					pWriter->WriteArray2(arSldIdLst);
				pWriter->EndNode(L"p14:sldIdLst");

				pWriter->EndNode(L"p14:section");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, id);
				pWriter->WriteString2(1, name);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);	

				pWriter->WriteRecordArray(0, 0, arSldIdLst);

			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
					case 0:
						id = pReader->GetString2();
						break;
					case 1:
						name = pReader->GetString2();
						break;
					default:
						break;
					}
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _rec = pReader->GetUChar();

					switch (_rec)
					{
						case 0:
						{
							pReader->Skip(4); // len
							ULONG lCount = pReader->GetULong();

							for (ULONG i = 0; i < lCount; ++i)
							{
								pReader->Skip(1);

								Logic::XmlId sldId(L"p14:sldId");

								arSldIdLst.push_back(sldId);
								arSldIdLst.back().fromPPTY(pReader);
							}
							
							break;
						}
						default:
						{
							pReader->SkipRecord();
							break;
						}
					}
				}

				pReader->Seek(_end_rec);
			}

			virtual void FillParentPointersForChilds()
			{
			}
		};
	}

	namespace nsPresentation
	{
		class SectionLst : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SectionLst)

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						Logic::Section sect;

						arSectionLst.push_back(sect);				
						arSectionLst.back().fromXML(oNode);
					}		
				}
			}
			virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

				while (pReader->GetPos() < end)
				{
					BYTE _rec = pReader->GetUChar();

					switch (_rec)
					{
						case 0:
						{
							pReader->Skip(4); // len
							ULONG lCount = pReader->GetULong();

							for (ULONG i = 0; i < lCount; ++i)
							{
								pReader->Skip(1);

								Logic::Section sect;

								arSectionLst.push_back(sect);
								arSectionLst.back().fromPPTY(pReader);
							}
							
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
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecordArray(0, 0, arSectionLst);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(L"p14:sectionLst");
				
				pWriter->StartAttributes();
				pWriter->WriteAttribute(L"xmlns:p14", PPTX::g_Namespaces.p14.m_strLink);
				pWriter->EndAttributes();

				pWriter->WriteArray2(arSectionLst);

				pWriter->EndNode(L"p14:sectionLst");
			}

			std::vector<Logic::Section> arSectionLst;

		protected:
			virtual void FillParentPointersForChilds(){};
		};
	}
} // namespace PPTX
