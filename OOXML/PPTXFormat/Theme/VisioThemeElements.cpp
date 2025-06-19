/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "VisioThemeElements.h"

namespace PPTX
{
	namespace nsTheme
	{
		VariationStyleSchemeLst& VariationStyleSchemeLst::operator=(const VariationStyleSchemeLst& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			m_arrItems.clear();
			for (auto elm : oSrc.m_arrItems)
				m_arrItems.push_back(elm);

			return *this;
		}
		void VariationStyleSchemeLst::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"variationStyleScheme" == strName)
					{
						m_arrItems.emplace_back();
						m_arrItems.back().fromXML(oNode);
					}
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring VariationStyleSchemeLst::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(m_arrItems);

			return XmlUtils::CreateNode(L"vt:variationStyleSchemeLst", oAttr, oValue);
		}
		void VariationStyleSchemeLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (auto elm : m_arrItems)
				pWriter->WriteRecord1(0, elm);
		}
		void VariationStyleSchemeLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:variationStyleSchemeLst");
			pWriter->WriteAttribute(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");
			pWriter->EndAttributes();

			pWriter->WriteArray2(m_arrItems);

			pWriter->EndNode(_T("vt:variationStyleSchemeLst"));
		}
		void VariationStyleSchemeLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						m_arrItems.emplace_back();
						m_arrItems.back().fromPPTY(pReader);						
					}break;
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void VariationStyleSchemeLst::FillParentPointersForChilds()
		{
			for (auto elm : m_arrItems)
				elm.SetParentPointer(this);
		}
//-----------------------------------------------------------------------------------------
		FontStyles& FontStyles::operator=(const FontStyles& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			node_name = oSrc.node_name;

			m_arrItems.clear();
			for (auto elm : oSrc.m_arrItems)
				m_arrItems.push_back(elm);

			return *this;
		}
		void FontStyles::fromXML(XmlUtils::CXmlNode& node)
		{
			node_name = node.GetName();

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"fontProps" == strName)
					{
						m_arrItems.emplace_back();
						m_arrItems.back().fromXML(oNode);
					}
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring FontStyles::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(m_arrItems);

			return XmlUtils::CreateNode(node_name, oAttr, oValue);
		}
		void FontStyles::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (auto elm : m_arrItems)
				pWriter->WriteRecord1(0, elm);
		}
		void FontStyles::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(node_name);
			pWriter->EndAttributes();

			pWriter->WriteArray2(m_arrItems);

			pWriter->EndNode(node_name);
		}
		void FontStyles::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					m_arrItems.emplace_back();
					m_arrItems.back().fromPPTY(pReader);
				}break;
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void FontStyles::FillParentPointersForChilds()
		{
			for (auto elm : m_arrItems)
				elm.SetParentPointer(this);
		}
//-----------------------------------------------------------------------------------------
		SchemeLineStyles& SchemeLineStyles::operator=(const SchemeLineStyles& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			node_name = oSrc.node_name;

			m_arrItems.clear();
			for (auto elm : oSrc.m_arrItems)
				m_arrItems.push_back(elm);

			return *this;
		}
		void SchemeLineStyles::fromXML(XmlUtils::CXmlNode& node)
		{
			node_name = node.GetName();

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"lineStyle" == strName)
					{
						m_arrItems.emplace_back();
						m_arrItems.back().fromXML(oNode);
					}
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring SchemeLineStyles::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(m_arrItems);

			return XmlUtils::CreateNode(node_name, oAttr, oValue);
		}
		void SchemeLineStyles::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (auto elm : m_arrItems)
				pWriter->WriteRecord1(0, elm);
		}
		void SchemeLineStyles::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(node_name);
			pWriter->EndAttributes();

			pWriter->WriteArray2(m_arrItems);

			pWriter->EndNode(node_name);
		}
		void SchemeLineStyles::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					m_arrItems.emplace_back();
					m_arrItems.back().fromPPTY(pReader);
				}break;
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void SchemeLineStyles::FillParentPointersForChilds()
		{
			for (auto elm : m_arrItems)
				elm.SetParentPointer(this);
		}
//-----------------------------------------------------------------------------------------
		FillStyles& FillStyles::operator=(const FillStyles& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			m_arrItems.clear();
			for (auto elm : oSrc.m_arrItems)
				m_arrItems.push_back(elm);

			return *this;
		}
		void FillStyles::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"fillProps" == strName)
					{
						m_arrItems.emplace_back();
						m_arrItems.back().fromXML(oNode);
					}
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring FillStyles::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");
			
			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(m_arrItems);

			return XmlUtils::CreateNode(L"vt:fillStyles", oAttr, oValue);
		}
		void FillStyles::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (auto elm : m_arrItems)
				pWriter->WriteRecord1(0, elm);
		}
		void FillStyles::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:fillStyles");
			pWriter->WriteAttribute(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");
			pWriter->EndAttributes();

			pWriter->WriteArray2(m_arrItems);

			pWriter->EndNode(L"vt:fillStyles");
		}
		void FillStyles::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					m_arrItems.emplace_back();
					m_arrItems.back().fromPPTY(pReader);
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void FillStyles::FillParentPointersForChilds()
		{
			for (auto elm : m_arrItems)
				elm.SetParentPointer(this);
		}
//-----------------------------------------------------------------------------------------
		VariationStyleScheme& VariationStyleScheme::operator=(const VariationStyleScheme& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;
		
			embellishment = oSrc.embellishment;

			m_arrItems.clear();
			for (auto elm : oSrc.m_arrItems)
				m_arrItems.push_back(elm);

			return *this;
		}
		void VariationStyleScheme::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"embellishment", embellishment);

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"varStyle" == strName)
					{
						m_arrItems.emplace_back();
						m_arrItems.back().fromXML(oNode);
					}
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring VariationStyleScheme::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"embellishment", embellishment);

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(m_arrItems);

			return XmlUtils::CreateNode(L"vt:variationStyleScheme", oAttr, oValue);
		}
		void VariationStyleScheme::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, embellishment);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			for (auto elm : m_arrItems)
				pWriter->WriteRecord1(0, elm);
		}
		void VariationStyleScheme::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:variationStyleScheme");
			pWriter->WriteAttribute2(L"embellishment", embellishment);
			pWriter->EndAttributes();

			pWriter->WriteArray2(m_arrItems);

			pWriter->EndNode(L"vt:variationStyleScheme");
		}
		void VariationStyleScheme::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				{
					embellishment = pReader->GetULong();
				}break;
				default:
					break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					m_arrItems.emplace_back();
					m_arrItems.back().fromPPTY(pReader);
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void VariationStyleScheme::FillParentPointersForChilds()
		{
			for (auto elm : m_arrItems)
				elm.SetParentPointer(this);
		}
//-----------------------------------------------------------------------------------------
		FontStylesGroup& FontStylesGroup::operator=(const FontStylesGroup& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			connectorFontStyles = oSrc.connectorFontStyles;
			fontStyles = oSrc.fontStyles;

			return *this;
		}
		void FontStylesGroup::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"connectorFontStyles" == strName)
					{
						connectorFontStyles.fromXML(oNode);
					}
					else if (L"fontStyles" == strName)
					{
						fontStyles.fromXML(oNode);
					}
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring FontStylesGroup::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");

			XmlUtils::CNodeValue oValue;
			oValue.Write(connectorFontStyles);
			oValue.Write(fontStyles);

			return XmlUtils::CreateNode(L"vt:fontStylesGroup", oAttr, oValue);
		}
		void FontStylesGroup::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, connectorFontStyles);
			pWriter->WriteRecord1(1, fontStyles);
		}
		void FontStylesGroup::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:fontStylesGroup");
			pWriter->WriteAttribute(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");
			pWriter->EndAttributes();

			connectorFontStyles.toXmlWriter(pWriter);
			fontStyles.toXmlWriter(pWriter);

			pWriter->EndNode(L"vt:fontStylesGroup");
		}
		void FontStylesGroup::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					connectorFontStyles.node_name = L"vt:connectorFontStyles";
					connectorFontStyles.fromPPTY(pReader);
				}break;
				case 1:
				{
					fontStyles.node_name = L"vt:fontStyles";
					fontStyles.fromPPTY(pReader);
				}break;
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void FontStylesGroup::FillParentPointersForChilds()
		{
			connectorFontStyles.SetParentPointer(this);
			fontStyles.SetParentPointer(this);
		}
//-----------------------------------------------------------------------------------------
		LineStyles& LineStyles::operator=(const LineStyles& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			fmtConnectorSchemeLineStyles = oSrc.fmtConnectorSchemeLineStyles;
			fmtSchemeLineStyles = oSrc.fmtSchemeLineStyles;

			return *this;
		}
		void LineStyles::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"fmtConnectorSchemeLineStyles" == strName)
					{
						fmtConnectorSchemeLineStyles.fromXML(oNode);
					}
					else if (L"fmtSchemeLineStyles" == strName)
					{
						fmtSchemeLineStyles.fromXML(oNode);
					}
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring LineStyles::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");

			XmlUtils::CNodeValue oValue;
			oValue.Write(fmtConnectorSchemeLineStyles);
			oValue.Write(fmtSchemeLineStyles);

			return XmlUtils::CreateNode(L"vt:lineStyles", oAttr, oValue);
		}
		void LineStyles::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, fmtConnectorSchemeLineStyles);
			pWriter->WriteRecord1(1, fmtSchemeLineStyles);
		}
		void LineStyles::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:lineStyles");
			pWriter->WriteAttribute(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");
			pWriter->EndAttributes();

			fmtConnectorSchemeLineStyles.toXmlWriter(pWriter);
			fmtSchemeLineStyles.toXmlWriter(pWriter);

			pWriter->EndNode(L"vt:lineStyles");
		}
		void LineStyles::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					fmtConnectorSchemeLineStyles.node_name = L"vt:fmtConnectorSchemeLineStyles";
					fmtConnectorSchemeLineStyles.fromPPTY(pReader);
				}break;
				case 1:
				{
					fmtSchemeLineStyles.node_name = L"vt:fmtSchemeLineStyles";
					fmtSchemeLineStyles.fromPPTY(pReader);
				}break;
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void LineStyles::FillParentPointersForChilds()
		{
			fmtConnectorSchemeLineStyles.SetParentPointer(this);
			fmtSchemeLineStyles.SetParentPointer(this);
		}
//-----------------------------------------------------------------------------------------
		VarStyle& VarStyle::operator=(const VarStyle& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			fillIdx = oSrc.fillIdx;
			lineIdx = oSrc.lineIdx;
			effectIdx = oSrc.effectIdx;
			fontIdx = oSrc.fontIdx;

			return *this;
		}
		void VarStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"fillIdx", fillIdx);
			XmlMacroReadAttributeBase(node, L"lineIdx", lineIdx);
			XmlMacroReadAttributeBase(node, L"effectIdx", effectIdx);
			XmlMacroReadAttributeBase(node, L"fontIdx", fontIdx);
		}
		std::wstring VarStyle::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"fillIdx", fillIdx);
			oAttr.Write(L"lineIdx", lineIdx);
			oAttr.Write(L"effectIdx", effectIdx);
			oAttr.Write(L"fontIdx", fontIdx);


			XmlUtils::CNodeValue oValue;

			return XmlUtils::CreateNode(L"vt:varStyle", oAttr, oValue);
		}
		void VarStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, fillIdx);
			pWriter->WriteUInt2(1, lineIdx);
			pWriter->WriteUInt2(2, effectIdx);
			pWriter->WriteUInt2(3, fontIdx);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void VarStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:varStyle");
			pWriter->WriteAttribute2(L"fillIdx", fillIdx);
			pWriter->WriteAttribute2(L"lineIdx", lineIdx);
			pWriter->WriteAttribute2(L"effectIdx", effectIdx);
			pWriter->WriteAttribute2(L"fontIdx", fontIdx);
			pWriter->EndAttributes();

			pWriter->EndNode(L"vt:varStyle");
		}
		void VarStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				{
					fillIdx = pReader->GetULong();
				}break;
				case 1:
				{
					lineIdx = pReader->GetULong();
				}break;
				case 2:
				{
					effectIdx = pReader->GetULong();
				}break;
				case 3:
				{
					fontIdx = pReader->GetULong();
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
//-----------------------------------------------------------------------------------------
		FillProps& FillProps::operator=(const FillProps& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			pattern = oSrc.pattern;
			return *this;
		}
		void FillProps::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"pattern", pattern);
		}
		std::wstring FillProps::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"pattern", pattern);

			XmlUtils::CNodeValue oValue;

			return XmlUtils::CreateNode(L"vt:fillProps", oAttr, oValue);
		}
		void FillProps::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, pattern);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void FillProps::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:fillProps");
				pWriter->WriteAttribute2(L"pattern", pattern);
				pWriter->EndAttributes();
			pWriter->EndNode(L"vt:fillProps");
		}
		void FillProps::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				{
					pattern = pReader->GetULong();
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
//-----------------------------------------------------------------------------------------
		Sketch& Sketch::operator=(const Sketch& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			lnAmp = oSrc.lnAmp;
			fillAmp = oSrc.fillAmp;
			lnWeight = oSrc.lnWeight;
			numPts = oSrc.numPts;

			return *this;
		}
		void Sketch::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"lnAmp", lnAmp);
			XmlMacroReadAttributeBase(node, L"fillAmp", fillAmp);
			XmlMacroReadAttributeBase(node, L"lnWeight", lnWeight);
			XmlMacroReadAttributeBase(node, L"numPts", numPts);
		}
		std::wstring Sketch::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"lnAmp", lnAmp);
			oAttr.Write(L"fillAmp", fillAmp);
			oAttr.Write(L"lnWeight", lnWeight);
			oAttr.Write(L"numPts", numPts);

			XmlUtils::CNodeValue oValue;

			return XmlUtils::CreateNode(L"vt:sketch", oAttr, oValue);
		}
		void Sketch::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, lnAmp);
			pWriter->WriteUInt2(1, fillAmp);
			pWriter->WriteUInt2(2, lnWeight);
			pWriter->WriteUInt2(3, numPts);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Sketch::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:sketch");
			pWriter->WriteAttribute2(L"lnAmp", lnAmp);
			pWriter->WriteAttribute2(L"fillAmp", fillAmp);
			pWriter->WriteAttribute2(L"lnWeight", lnWeight);
			pWriter->WriteAttribute2(L"numPts", numPts);
			pWriter->EndAttributes();

			pWriter->EndNode(L"vt:sketch");
		}
		void Sketch::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				{
					lnAmp = pReader->GetULong();
				}break;
				case 1:
				{
					fillAmp = pReader->GetULong();
				}break;
				case 2:
				{
					lnWeight = pReader->GetULong();
				}break;
				case 3:
				{
					numPts = pReader->GetULong();
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
//-----------------------------------------------------------------------------------------
		FontProps& FontProps::operator=(const FontProps& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			style = oSrc.style;
			color = oSrc.color;

			return *this;
		}
		void FontProps::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"style", style);
			
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"color" == strName)
					{
						color.GetColorFrom(oNode);
					}
				}
			}
		}
		std::wstring FontProps::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"style", style);

			XmlUtils::CNodeValue oValue;
			oValue.Write(L"vt:color", color);

			return XmlUtils::CreateNode(L"vt:fontProps", oAttr, oValue);
		}
		void FontProps::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, style);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			
			pWriter->WriteRecord1(0, color);
		}
		void FontProps::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:fontProps");
			pWriter->WriteAttribute2(L"style", style);
			pWriter->EndAttributes();

			if (color.is_init())
			{
				pWriter->WriteString(L"<vt:color>");
				color.toXmlWriter(pWriter);
				pWriter->WriteString(L"</vt:color>");
			}

			pWriter->EndNode(L"vt:fontProps");
		}
		void FontProps::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				{
					style = pReader->GetULong();
				}break;
				default:
					break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					color.fromPPTY(pReader);					
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
//-----------------------------------------------------------------------------------------
		LineEx& LineEx::operator=(const LineEx& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			rndg = oSrc.rndg;
			start = oSrc.start;
			startSize = oSrc.startSize;
			end = oSrc.end;
			endSize = oSrc.endSize;
			pattern = oSrc.pattern;

			return *this;
		}
		void LineEx::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"rndg", rndg);
			XmlMacroReadAttributeBase(node, L"start", start);
			XmlMacroReadAttributeBase(node, L"startSize", startSize);
			XmlMacroReadAttributeBase(node, L"end", end);
			XmlMacroReadAttributeBase(node, L"endSize", endSize);
			XmlMacroReadAttributeBase(node, L"pattern", pattern);			
		}
		std::wstring LineEx::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"rndg", rndg);
			oAttr.Write(L"start", start);
			oAttr.Write(L"startSize", startSize);
			oAttr.Write(L"end", end);
			oAttr.Write(L"endSize", endSize);
			oAttr.Write(L"pattern", pattern);

			XmlUtils::CNodeValue oValue;

			return XmlUtils::CreateNode(L"vt:lineEx", oAttr, oValue);
		}
		void LineEx::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, rndg);
			pWriter->WriteUInt2(1, start);
			pWriter->WriteUInt2(2, startSize);
			pWriter->WriteUInt2(3, end);
			pWriter->WriteUInt2(4, endSize);
			pWriter->WriteUInt2(5, pattern);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void LineEx::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:lineEx");
				pWriter->WriteAttribute2(L"rndg", rndg);
				pWriter->WriteAttribute2(L"start", start);
				pWriter->WriteAttribute2(L"startSize", startSize);
				pWriter->WriteAttribute2(L"end", end);
				pWriter->WriteAttribute2(L"endSize", endSize);
				pWriter->WriteAttribute2(L"pattern", pattern);
				pWriter->EndAttributes();
			pWriter->EndNode(L"vt:lineEx");
		}
		void LineEx::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				{
					rndg = pReader->GetULong();
				}break;
				case 1:
				{
					start = pReader->GetULong();
				}break;
				case 2:
				{
					startSize = pReader->GetULong();
				}break;
				case 3:
				{
					end = pReader->GetULong();
				}break;
				case 4:
				{
					endSize = pReader->GetULong();
				}break;
				case 5:
				{
					pattern = pReader->GetULong();
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
//-----------------------------------------------------------------------------------------
		LineStyle& LineStyle::operator=(const LineStyle& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			lineEx = oSrc.lineEx;
			sketch = oSrc.sketch;

			return *this;
		}
		void LineStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"lineEx" == strName)
					{
						lineEx.fromXML(oNode);
					}
					else if (L"sketch" == strName)
					{
						sketch = oNode;
					}
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring LineStyle::toXML() const
		{
			XmlUtils::CAttribute oAttr;

			XmlUtils::CNodeValue oValue;
			oValue.Write(lineEx);
			oValue.WriteNullable(sketch);

			return XmlUtils::CreateNode(L"vt:lineStyle", oAttr, oValue);
		}
		void LineStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, lineEx);
			pWriter->WriteRecord2(1, sketch);
		}
		void LineStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:lineStyle");
			pWriter->EndAttributes();

			lineEx.toXmlWriter(pWriter);
			pWriter->Write(sketch);

			pWriter->EndNode(L"vt:lineStyle");
		}
		void LineStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					lineEx.fromPPTY(pReader);
				}break;
				case 1:
				{
					sketch.Init();
					sketch->fromPPTY(pReader);
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void LineStyle::FillParentPointersForChilds()
		{
			lineEx.SetParentPointer(this);
			if (sketch.IsInit()) sketch->SetParentPointer(this);
		}
//-----------------------------------------------------------------------------------------
		VariationClrScheme& VariationClrScheme::operator=(const VariationClrScheme& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			for (size_t i = 0; i < 7; ++i)
			{
				varColor[i] = oSrc.varColor[i];
			}
			return *this;
		}
		void VariationClrScheme::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (0 == strName.find(L"varColor"))
					{
						int ind = XmlUtils::GetInteger(strName.substr(8)) - 1;
						if (ind >= 0 && ind < 7)
							varColor[ind].GetColorFrom(oNode);
					}
				}
			}
		}
		std::wstring VariationClrScheme::toXML() const
		{
			XmlUtils::CAttribute oAttr;

			XmlUtils::CNodeValue oValue;

			for (size_t i = 0; i < 7; ++i)
			{
				std::wstring name = L"vt:varColor" + std::to_wstring(i + 1);
				oValue.Write(name, varColor[i]);
			}

			return XmlUtils::CreateNode(L"vt:variationClrScheme", oAttr, oValue);
		}
		void VariationClrScheme::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (size_t i = 0; i < 7; ++i)
			{
				pWriter->WriteRecord1(i, varColor[i]);
			}
		}
		void VariationClrScheme::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:variationClrScheme");
			pWriter->EndAttributes();

			for (size_t i = 0; i < 7; ++i)
			{
				if (varColor[i].is_init())
				{
					std::wstring name = L"vt:varColor" + std::to_wstring(i + 1);
					pWriter->WriteString(L"<" + name  + L">");
					varColor[i].toXmlWriter(pWriter);
					pWriter->WriteString(L"</" + name + L">");
				}
			}

			pWriter->EndNode(L"vt:variationClrScheme");
		}
		void VariationClrScheme::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				
				if (_at >= 0 && _at < 7)
				{
					varColor[_at].fromPPTY(pReader);
				}
			}
			pReader->Seek(_end_rec);
		}
//-----------------------------------------------------------------------------------------
		VariationClrSchemeLst& VariationClrSchemeLst::operator=(const VariationClrSchemeLst& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			m_arrItems.clear();
			for (auto elm : oSrc.m_arrItems)
				m_arrItems.push_back(elm);

			return *this;
		}
		void VariationClrSchemeLst::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"variationClrScheme" == strName)
					{
						m_arrItems.emplace_back();
						m_arrItems.back().fromXML(oNode);
					}
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring VariationClrSchemeLst::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");
			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(m_arrItems);

			return XmlUtils::CreateNode(L"vt:variationClrSchemeLst", oAttr, oValue);
		}
		void VariationClrSchemeLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (auto elm : m_arrItems)
				pWriter->WriteRecord1(0, elm);
		}
		void VariationClrSchemeLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:variationClrSchemeLst");
			pWriter->WriteAttribute(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");
			pWriter->EndAttributes();

			pWriter->WriteArray2(m_arrItems);

			pWriter->EndNode(L"vt:variationClrSchemeLst");
		}
		void VariationClrSchemeLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					m_arrItems.emplace_back();
					m_arrItems.back().fromPPTY(pReader);
				}break;
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void VariationClrSchemeLst::FillParentPointersForChilds()
		{
			for (auto elm : m_arrItems)
				elm.SetParentPointer(this);
		}
//-----------------------------------------------------------------------------------------
		Bkgnd& Bkgnd::operator=(const Bkgnd& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			color = oSrc.color;

			return *this;
		}
		void Bkgnd::fromXML(XmlUtils::CXmlNode& node)
		{
			color.GetColorFrom(node);
		}
		std::wstring Bkgnd::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");

			return XmlUtils::CreateNode(L"vt:bkgnd", oAttr, color.toXML());
		}
		void Bkgnd::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, color);
		}
		void Bkgnd::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:bkgnd");
			pWriter->WriteAttribute(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");
			pWriter->EndAttributes();

			if (color.is_init())
			{
				color.toXmlWriter(pWriter);
			}
			pWriter->EndNode(L"vt:bkgnd");
		}
		void Bkgnd::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					color.fromPPTY(pReader);
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
//-----------------------------------------------------------------------------------------
		SchemeID& SchemeID::operator=(const SchemeID& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			schemeEnum = oSrc.schemeEnum;
			schemeGUID = oSrc.schemeGUID;

			return *this;
		}
		void SchemeID::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"schemeEnum", schemeEnum);
			XmlMacroReadAttributeBase(node, L"schemeGUID", schemeGUID);
		}
		std::wstring SchemeID::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			if (bSchemas)
			{
				oAttr.Write(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");
			}
			oAttr.Write(L"schemeEnum", schemeEnum);
			oAttr.Write(L"schemeGUID", schemeGUID);

			XmlUtils::CNodeValue oValue;

			return XmlUtils::CreateNode(L"vt:schemeID", oAttr, oValue);
		}
		void SchemeID::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, schemeEnum);
			pWriter->WriteString2(1, schemeGUID);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void SchemeID::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:schemeID");
			if (bSchemas)
			{
				pWriter->WriteAttribute(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");
			}
			pWriter->WriteAttribute2(L"schemeEnum", schemeEnum);
			pWriter->WriteAttribute2(L"schemeGUID", schemeGUID);
			pWriter->EndAttributes();
			pWriter->EndNode(L"vt:schemeID");
		}
		void SchemeID::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				{
					schemeEnum = pReader->GetULong();
				}break;
				case 1:
				{
					schemeGUID = pReader->GetString2();
				}break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
//-----------------------------------------------------------------------------------------
		Scheme& Scheme::operator=(const Scheme& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			node_name = oSrc.node_name;
			schemeID = oSrc.schemeID;

			return *this;
		}
		void Scheme::fromXML(XmlUtils::CXmlNode& node)
		{
			node_name = node.GetName();

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());
					if (L"schemeID" == strName)
					{
						schemeID = oNode;
					}
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring Scheme::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(schemeID);

			return XmlUtils::CreateNode(node_name, oAttr, oValue);
		}
		void Scheme::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, schemeID);
		}
		void Scheme::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(node_name);
			pWriter->WriteAttribute(L"xmlns:vt", L"http://schemas.microsoft.com/office/visio/2012/theme");
			pWriter->EndAttributes();

			pWriter->Write(schemeID);

			pWriter->EndNode(node_name);
		}
		void Scheme::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					schemeID.Init();
					schemeID->fromPPTY(pReader);
				}break;
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void Scheme::FillParentPointersForChilds()
		{
			if (schemeID.IsInit())
				schemeID->SetParentPointer(this);
		}
	} // namespace nsTheme
} // namespace PPTX
