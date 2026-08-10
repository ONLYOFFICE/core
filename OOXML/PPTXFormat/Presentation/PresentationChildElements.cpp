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

#include "PresentationChildElements.h"

namespace PPTX
{
	namespace nsPresentation
	{
		void EmbeddedFont::fromXML(XmlUtils::CXmlNode& node)
		{
			Bold = node.ReadNode(_T("p:bold"));
			BoldItalic = node.ReadNode(_T("p:boldItalic"));
			Italic = node.ReadNode(_T("p:italic"));
			Regular = node.ReadNode(_T("p:regular"));
			Font = node.ReadNode(_T("p:font"));

			FillParentPointersForChilds();
		}
		std::wstring EmbeddedFont::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(Bold);
			oValue.WriteNullable(BoldItalic);
			oValue.WriteNullable(Italic);
			oValue.WriteNullable(Regular);
			oValue.Write(Font);

			return XmlUtils::CreateNode(_T("p:embeddedFont"), oValue);
		}
		void EmbeddedFont::FillParentPointersForChilds()
		{
			if (Bold.is_init())
				Bold->SetParentPointer(this);
			if (BoldItalic.is_init())
				BoldItalic->SetParentPointer(this);
			if (Italic.is_init())
				Italic->SetParentPointer(this);
			if (Regular.is_init())
				Regular->SetParentPointer(this);
			Font.SetParentPointer(this);
		}
		void EmbeddedFontDataId::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = XmlUtils::GetNameNoNS(node.GetName());

			rid = node.GetAttribute(L"r:id");
		}
		std::wstring EmbeddedFontDataId::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"r:id", rid);

			return XmlUtils::CreateNode(L"p:" + m_name, oAttr);
		}
		void EmbeddedFontDataId::FillParentPointersForChilds()
		{
		}
		void NotesSz::fromXML(XmlUtils::CXmlNode& node)
		{
			cx = node.ReadAttributeInt(L"cx");
			cy = node.ReadAttributeInt(L"cy");

			Normalize();
		}
		std::wstring NotesSz::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("cx"), cx);
			oAttr.Write(_T("cy"), cy);

			return XmlUtils::CreateNode(_T("p:notesSz"), oAttr);
		}
		void NotesSz::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

			pWriter->WriteInt1(0, cx);
			pWriter->WriteInt1(1, cy);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void NotesSz::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:notesSz"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("cx"), cx);
			pWriter->WriteAttribute(_T("cy"), cy);
			pWriter->EndAttributes();
			pWriter->EndNode(_T("p:notesSz"));
		}
		void NotesSz::FillParentPointersForChilds()
		{
		}
		void NotesSz::Normalize()
		{
			if (cx < 0)
				cx = 0;
			if (cy < 0)
				cy = 0;
		}
		void Kinsoku::fromXML(XmlUtils::CXmlNode& node)
		{
			invalEndChars = node.GetAttribute(_T("invalEndChars"));
			invalStChars = node.GetAttribute(_T("invalStChars"));

			XmlMacroReadAttributeBase(node, L"lang", lang);
		}
		std::wstring Kinsoku::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("invalEndChars"), invalEndChars);
			oAttr.Write(_T("invalStChars"), invalStChars);
			oAttr.Write(_T("lang"), lang);

			return XmlUtils::CreateNode(_T("p:kinsoku"), oAttr);
		}
		void Kinsoku::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			// TODO:
		}
		void Kinsoku::FillParentPointersForChilds()
		{
		}
		void PhotoAlbum::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"bw", bw);
			XmlMacroReadAttributeBase(node, L"frame", frame);
			XmlMacroReadAttributeBase(node, L"layout", layout);
			XmlMacroReadAttributeBase(node, L"showCaptions", showCaptions);
		}
		std::wstring PhotoAlbum::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("bw"), bw);
			oAttr.WriteLimitNullable(_T("frame"), frame);
			oAttr.WriteLimitNullable(_T("layout"), layout);
			oAttr.Write(_T("showCaptions"), showCaptions);

			return XmlUtils::CreateNode(_T("p:photoAlbum"), oAttr);
		}
		void PhotoAlbum::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
		}
		void PhotoAlbum::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
		}
		void PhotoAlbum::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
		}
		void PhotoAlbum::FillParentPointersForChilds()
		{
		}
		void SldSz::fromXML(XmlUtils::CXmlNode& node)
		{
			cx = node.ReadAttributeInt(L"cx");
			cy = node.ReadAttributeInt(L"cy");

			XmlMacroReadAttributeBase(node, L"type", type);

			Normalize();
		}
		std::wstring SldSz::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("cx"), cx);
			oAttr.Write(_T("cy"), cy);
			oAttr.WriteLimitNullable(_T("type"), type);

			return XmlUtils::CreateNode(_T("p:sldSz"), oAttr);
		}
		void SldSz::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

			pWriter->WriteInt1(0, cx);
			pWriter->WriteInt1(1, cy);
			pWriter->WriteLimit2(2, type);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void SldSz::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:sldSz"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("cx"), cx);
			pWriter->WriteAttribute(_T("cy"), cy);
			pWriter->WriteAttribute(_T("type"), type);
			pWriter->EndAttributes();
			pWriter->EndNode(_T("p:sldSz"));
		}
		void SldSz::FillParentPointersForChilds()
		{
		}
		void SldSz::Normalize()
		{
			cx = (std::min)((std::max)(cx, 914400), 51206400);
			cy = (std::min)((std::max)(cy, 914400), 51206400);
		}
//-----------------------------------------------------------------------------------------------------------
		void SectionLst::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t nCount = oNodes.size();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					Section sect;
					arSectionLst.push_back(sect);

					arSectionLst.back().fromXML(oNode);
				}
			}
		}
		std::wstring SectionLst::toXML() const
		{
			return L"";
		}
		void SectionLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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

						Section sect;

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
		void SectionLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecordArray(0, 0, arSectionLst);
		}
		void SectionLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p14:sectionLst");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"xmlns:p14", PPTX::g_Namespaces.p14.m_strLink);
			pWriter->EndAttributes();

			pWriter->WriteArray2(arSectionLst);

			pWriter->EndNode(L"p14:sectionLst");
		}
//----------------------------------------------------------------------------------------------------------------------
		void Section::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"id", id);
			XmlMacroReadAttributeBase(node, L"name", name);

			XmlUtils::CXmlNode oNodeSldIdLst;
			if (node.GetNode(L"p14:sldIdLst", oNodeSldIdLst))
			{
				XmlMacroLoadArray(oNodeSldIdLst, L"p14:sldId", arSldIdLst, Logic::XmlId);
			}
		}
		std::wstring Section::toXML() const
		{
			return L"";
		}
		void Section::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
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
		void Section::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, id);
			pWriter->WriteString2(1, name);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecordArray(0, 0, arSldIdLst);

		}
		void Section::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
//----------------------------------------------------------------------------------------------------------------------
		void CustShowLst::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t nCount = oNodes.size();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					arCustShowLst.emplace_back();
					arCustShowLst.back().fromXML(oNode);
				}
			}
		}
		std::wstring CustShowLst::toXML() const
		{
			return L"";
		}
		void CustShowLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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

						CustShow custShow;

						arCustShowLst.push_back(custShow);
						arCustShowLst.back().fromPPTY(pReader);
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
		void CustShowLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecordArray(0, 0, arCustShowLst);
		}
		void CustShowLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:custShowLst");
			
			pWriter->StartAttributes();
			pWriter->EndAttributes();

			pWriter->WriteArray2(arCustShowLst);

			pWriter->EndNode(L"p:custShowLst");
		}
//----------------------------------------------------------------------------------------------------------------------
		void CustShow::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNodeSldIdLst;
			if (node.GetNode(L"p:sldIdLst", oNodeSldIdLst))
			{
				if (oNodeSldIdLst.IsValid())
				{
					std::vector<XmlUtils::CXmlNode> arrNodeSldId;
					if (oNodeSldIdLst.GetNodes(L"sldId", arrNodeSldId))

					for (auto n : arrNodeSldId)
					{
						arSldIdLst.emplace_back();
						XmlMacroReadAttributeBase(n, L"r:id", arSldIdLst.back());
					}
				}
			}
		}
		std::wstring CustShow::toXML() const
		{
			return L"";
		}
		void CustShow::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:custShow");

			pWriter->StartAttributes();
			pWriter->EndAttributes();

			pWriter->StartNode(L"p:sldIdLst");
			pWriter->EndAttributes();
			for (auto sld : arSldIdLst)
			{
				pWriter->StartNode(L"p:sldId");
				std::wstring sRid = sld.ToString();
				if (sRid.empty() == false)
				{
					pWriter->WriteAttribute(L"r:id", sRid);
				}
				pWriter->EndAttributes();
				pWriter->EndNode(L"p:sldId");
			}
			pWriter->EndNode(L"p:sldIdLst");

			pWriter->EndNode(L"p:custShow");
		}
		void CustShow::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(0);

			_UINT32 len = (_UINT32)arSldIdLst.size();
			pWriter->WriteULONG(len);

			for (_UINT32 i = 0; i < len; ++i)
			{
				pWriter->WriteString(arSldIdLst[i].ToString());
			}

			pWriter->EndRecord();
		}
		void CustShow::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

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

						arSldIdLst.emplace_back();
						arSldIdLst.back() = pReader->GetString2();
					}
					
				}break;
				default:
				{
					pReader->SkipRecord();
					break;
				}
				}
			}
			pReader->Seek(_end_rec);
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
		void ExtendedGuideList::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = L"p15:" + XmlUtils::GetNameNoNS(node.GetName());

			std::vector<XmlUtils::CXmlNode> oNodes;
			node.GetNodes(L"*", oNodes);
			size_t nCount = oNodes.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode& oNode = oNodes[i];
				std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

				if (strName == L"guide")
				{
					m_arGuide.emplace_back();
					m_arGuide.back().fromXML(oNode);
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring ExtendedGuideList::toXML() const
		{
			return L"";
		}
		void ExtendedGuideList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			toXmlWriter(pWriter, m_name.empty() ? L"p15:sldGuideLst" : m_name);
		}
		void ExtendedGuideList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter, const std::wstring& strName) const
		{
			pWriter->StartNode(strName);
			pWriter->EndAttributes();

			for (size_t i = 0; i < m_arGuide.size(); ++i)
			{				
				m_arGuide[i].toXmlWriter(pWriter, (unsigned int)(i + 1)); //  from 1
			}

			pWriter->EndNode(strName);
		}
		void ExtendedGuideList::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecordArray(0, 1, m_arGuide);
		}
		void ExtendedGuideList::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					pReader->Skip(4); // len
					ULONG lCount = pReader->GetULong();

					for (ULONG i = 0; i < lCount; ++i)
					{
						pReader->Skip(1);

						m_arGuide.emplace_back();
						m_arGuide.back().fromPPTY(pReader);
					}

				}break;
				default:
				{
					pReader->SkipRecord();
					break;
				}
				}
			}

			pReader->Seek(_end_rec);
		}
		void ExtendedGuideList::FillParentPointersForChilds()
		{
			for (size_t i = 0; i < m_arGuide.size(); ++i)
				m_arGuide[i].SetParentPointer(this);
		}
		void ExtendedGuide::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"name", name);
			XmlMacroReadAttributeBase(node, L"id", id);
			XmlMacroReadAttributeBase(node, L"orient", orient);
			XmlMacroReadAttributeBase(node, L"pos", pos);
			XmlMacroReadAttributeBase(node, L"userDrawn", userDrawn);

			std::vector<XmlUtils::CXmlNode> oNodes;
			node.GetNodes(L"*", oNodes);

			for (size_t i = 0; i < oNodes.size(); ++i)
			{
				XmlUtils::CXmlNode& oNode = oNodes[i];
				std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

				if (strName == L"extLst")
				{
				}
				else if (strName == L"clr")
				{
					clr.GetColorFrom(oNode);
				}
			}
		}
		std::wstring ExtendedGuide::toXML() const
		{
			return L"";
		}
		void ExtendedGuide::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			toXmlWriter(pWriter, 1);
		}
		void ExtendedGuide::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter, unsigned int nDefaultId) const
		{
			pWriter->StartNode(L"p15:guide");

			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"id", id.get_value_or(nDefaultId));
			pWriter->WriteAttribute2(L"name", name);
			pWriter->WriteAttribute(L"orient", orient);
			pWriter->WriteAttribute(L"pos", pos.get_value_or(0));
			pWriter->WriteAttribute(L"userDrawn", userDrawn);
			pWriter->EndAttributes();

			pWriter->StartNode(L"p15:clr");
			pWriter->EndAttributes();
			if (clr.is_init())
				clr.toXmlWriter(pWriter);
			else
				pWriter->WriteString(L"<a:srgbClr val=\"A4A3A4\"/>"); // default
			pWriter->EndNode(L"p15:clr");

			pWriter->EndNode(L"p15:guide");
		}
		void ExtendedGuide::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, id);
			pWriter->WriteString2(1, name);
			pWriter->WriteLimit2(2, orient);
			pWriter->WriteInt2(3, pos);
			pWriter->WriteBool2(4, userDrawn);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			if (clr.is_init())
				pWriter->WriteRecord1(0, clr);
		}
		void ExtendedGuide::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						id = pReader->GetULong();
						break;
					case 1:
						name = pReader->GetString2();
						break;
					case 2:
						orient = pReader->GetUChar();
						break;
					case 3:
						pos = pReader->GetLong();
						break;
					case 4:
						userDrawn = pReader->GetBool();
						break;
					default:
					{
						pReader->Seek(_end_rec);
						return;
					}
				}
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						clr.fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
						break;
					}
				}
			}

			pReader->Seek(_end_rec);
		}
		void ExtendedGuide::FillParentPointersForChilds()
		{
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
	} // namespace 
} // namespace PPTX
