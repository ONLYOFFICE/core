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
#include "Metadata.h"

#include "../FileTypes_Spreadsheet.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

#include "../../DocxFormat/Drawing/DrawingExt.h"
#include "../../../DesktopEditor/common/File.h"

#include "../../Binary/Presentation/XmlWriter.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CMdxKPI::CMdxKPI() {}
		CMdxKPI::~CMdxKPI() {}
		void CMdxKPI::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMdxKPI::toXML() const
		{
			return L"";
		}
		EElementType CMdxKPI::getType() const
		{
			return et_x_MdxKPI;
		}
		void CMdxKPI::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<k");
			WritingStringNullableAttrInt2(L"n", m_oN);
			WritingStringNullableAttrInt2(L"np", m_oNp);
			WritingStringNullableAttrString(L"p", m_oP, m_oP->ToString());
			writer.WriteString(L"/>");
		}
		void CMdxKPI::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CMdxKPI::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"n", m_oN)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"np", m_oNp)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"p", m_oP)
				WritingElement_ReadAttributes_End(oReader)
		}
		//--------------------------------------------------------------------------------------------------------
		CMdxMemeberProp::CMdxMemeberProp() {}
		CMdxMemeberProp::~CMdxMemeberProp() {}
		void CMdxMemeberProp::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMdxMemeberProp::toXML() const
		{
			return L"";
		}
		EElementType CMdxMemeberProp::getType() const
		{
			return et_x_MdxMemeberProp;
		}
		void CMdxMemeberProp::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<p");
			WritingStringNullableAttrInt2(L"n", m_oN);
			WritingStringNullableAttrInt2(L"np", m_oNp);
			writer.WriteString(L"/>");
		}
		void CMdxMemeberProp::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CMdxMemeberProp::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"n", m_oN)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"np", m_oNp)
				WritingElement_ReadAttributes_End(oReader)
		}
		//--------------------------------------------------------------------------------------------------------
		CMdxSet::CMdxSet() {}
		CMdxSet::~CMdxSet() {}
		void CMdxSet::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMdxSet::toXML() const
		{
			return L"";
		}
		EElementType CMdxSet::getType() const
		{
			return et_x_MdxSet;
		}
		void CMdxSet::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<s>");
			WritingStringNullableAttrInt2(L"ns", m_oNs);
			WritingStringNullableAttrInt2(L"c", m_oC);
			WritingStringNullableAttrString(L"o", m_oO, m_oO->ToString());
			writer.WriteString(L">");
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</s>");
		}
		void CMdxSet::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"n" == sName)
				{
					CMetadataStringIndex* ind = new CMetadataStringIndex();
					*ind = oReader;
					m_arrItems.push_back(ind);
				}
			}
		}
		void CMdxSet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"ns", m_oNs)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"c", m_oC)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"o", m_oO)
				WritingElement_ReadAttributes_End(oReader)
		}
		//--------------------------------------------------------------------------------------------------------
		CMetadataStringIndex::CMetadataStringIndex() {}
		CMetadataStringIndex::~CMetadataStringIndex() {}
		void CMetadataStringIndex::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMetadataStringIndex::toXML() const
		{
			return L"";
		}
		EElementType CMetadataStringIndex::getType() const
		{
			return et_x_MetadataStringIndex;
		}
		void CMetadataStringIndex::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<n");
			WritingStringNullableAttrInt2(L"x", m_oX);
			WritingStringNullableAttrInt2(L"s", m_oS);
			writer.WriteString(L"/>");
		}
		void CMetadataStringIndex::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CMetadataStringIndex::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"x", m_oX)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"s", m_oS)
				WritingElement_ReadAttributes_End(oReader)
		}
		//--------------------------------------------------------------------------------------------------------
		CMdxTuple::CMdxTuple() {}
		CMdxTuple::~CMdxTuple() {}
		void CMdxTuple::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMdxTuple::toXML() const
		{
			return L"";
		}
		EElementType CMdxTuple::getType() const
		{
			return et_x_MdxTuple;
		}
		void CMdxTuple::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<t>");
			WritingStringNullableAttrInt2(L"c", m_oC);
			WritingStringNullableAttrEncodeXmlString2(L"o", m_oCt);
			WritingStringNullableAttrInt2(L"si", m_oSi);
			WritingStringNullableAttrInt2(L"fi", m_oFi);
			WritingStringNullableAttrInt2(L"bc", m_oBc);
			WritingStringNullableAttrInt2(L"fc", m_oFc);
			WritingStringNullableAttrBool2(L"i", m_oI);
			WritingStringNullableAttrBool2(L"u", m_oU);
			WritingStringNullableAttrBool2(L"st", m_oSt);
			WritingStringNullableAttrBool2(L"b", m_oB);
			writer.WriteString(L">");
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</t>");
		}
		void CMdxTuple::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"n" == sName)
				{
					CMetadataStringIndex* ind = new CMetadataStringIndex();
					*ind = oReader;
					m_arrItems.push_back(ind);
				}
			}
		}
		void CMdxTuple::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"c", m_oC)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"ct", m_oCt)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"si", m_oSi)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"fi", m_oFi)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"bc", m_oBc)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"fc", m_oFc)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"i", m_oI)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"u", m_oU)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"st", m_oSt)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"b", m_oB)
			WritingElement_ReadAttributes_End(oReader)
		}
		//--------------------------------------------------------------------------------------------------------
		CMdx::CMdx() {}
		CMdx::~CMdx() {}
		void CMdx::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMdx::toXML() const
		{
			return L"";
		}
		EElementType CMdx::getType() const
		{
			return et_x_Mdx;
		}
		void CMdx::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<mdx>");
			WritingStringNullableAttrInt2(L"n", m_oN);
			WritingStringNullableAttrString(L"f", m_oF, m_oF->ToString());

			if (m_oMdxTuple.IsInit())
			{
				m_oMdxTuple->toXML(writer);
			}
			if (m_oMdxSet.IsInit())
			{
				m_oMdxSet->toXML(writer);
			}
			if (m_oCMdxKPI.IsInit())
			{
				m_oCMdxKPI->toXML(writer);
			}
			if (m_oMdxMemeberProp.IsInit())
			{
				m_oMdxMemeberProp->toXML(writer);
			}
			writer.WriteString(L"</mdx>");
		}
		void CMdx::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"t" == sName)
					m_oMdxTuple = oReader;
				else if (L"ms" == sName)
					m_oMdxSet = oReader;
				else if (L"p" == sName)
					m_oMdxMemeberProp = oReader;
				else if (L"k" == sName)
					m_oCMdxKPI = oReader;
			}
		}
		void CMdx::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"n", m_oN)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"f", m_oF)
				WritingElement_ReadAttributes_End(oReader)
		}
		//-------------------------------------------------------------------------------------
		CMdxMetadata::CMdxMetadata() {}
		CMdxMetadata::~CMdxMetadata() {}
		void CMdxMetadata::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMdxMetadata::toXML() const
		{
			return L"";
		}
		EElementType CMdxMetadata::getType() const
		{
			return et_x_MdxMetadata;
		}
		void CMdxMetadata::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"mdx" == sName)
				{
					CMdx* mdx = new CMdx();
					*mdx = oReader;
					m_arrItems.push_back(mdx);
				}
			}
		}
		void CMdxMetadata::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString(L"<mdxMetadata>");

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</mdxMetadata>");
		}
		//-------------------------------------------------------------------------------------
		CMetadataStrings::CMetadataStrings() {}
		CMetadataStrings::~CMetadataStrings() {}
		void CMetadataStrings::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMetadataStrings::toXML() const
		{
			return L"";
		}
		EElementType CMetadataStrings::getType() const
		{
			return et_x_MetadataStrings;
		}
		void CMetadataStrings::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"s" == sName)
				{
					CMetadataString* pS = new CMetadataString();
					*pS = oReader;
					m_arrItems.push_back(pS);
				}
			}
		}
		void CMetadataStrings::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString(L"<metadataStrings count=\"" + std::to_wstring(m_arrItems.size()) + L"\">");

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</metadataStrings>");
		}
		//--------------------------------------------------------------------------------------------------------
		CMetadataString::CMetadataString() {}
		CMetadataString::~CMetadataString() {}
		void CMetadataString::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMetadataString::toXML() const
		{
			return L"";
		}
		EElementType CMetadataString::getType() const
		{
			return et_x_MetadataString;
		}
		void CMetadataString::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<s");
			WritingStringNullableAttrEncodeXmlString2(L"v", m_oV);
			writer.WriteString(L"/>");
		}
		void CMetadataString::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CMetadataString::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"v", m_oV)
				WritingElement_ReadAttributes_End(oReader)
		}
		// --------------------------------------------------------------------------------------------------------
		CMetadataRecord::CMetadataRecord() {}
		CMetadataRecord::~CMetadataRecord() {}
		void CMetadataRecord::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMetadataRecord::toXML() const
		{
			return L"";
		}
		EElementType CMetadataRecord::getType() const
		{
			return et_x_MetadataRecord;
		}
		void CMetadataRecord::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<rc");
			WritingStringNullableAttrInt2(L"t", m_oT);
			WritingStringNullableAttrInt2(L"v", m_oV);
			writer.WriteString(L"/>");
		}
		void CMetadataRecord::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CMetadataRecord::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"t", m_oT)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"v", m_oV)
				WritingElement_ReadAttributes_End(oReader)
		}
		//-------------------------------------------------------------------------------------
		CMetadataBlock::CMetadataBlock() {}
		CMetadataBlock::~CMetadataBlock() {}
		void CMetadataBlock::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMetadataBlock::toXML() const
		{
			return L"";
		}
		EElementType CMetadataBlock::getType() const
		{
			return et_x_MetadataBlock;
		}
		void CMetadataBlock::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"rc" == sName)
				{
					CMetadataRecord* pR = new CMetadataRecord();
					*pR = oReader;
					m_arrItems.push_back(pR);
				}
			}
		}
		void CMetadataBlock::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString(L"<bk>");

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</bk>");
		}
		//-------------------------------------------------------------------------------------
		CMetadataBlocks::CMetadataBlocks() {}
		CMetadataBlocks::~CMetadataBlocks() {}
		void CMetadataBlocks::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMetadataBlocks::toXML() const
		{
			return L"";
		}
		EElementType CMetadataBlocks::getType() const
		{
			return et_x_MetadataBlocks;
		}
		void CMetadataBlocks::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring  sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"bk" == sName)
				{
					CMetadataBlock* pB = new CMetadataBlock();
					*pB = oReader;
					m_arrItems.push_back(pB);
				}
			}
		}
		void CMetadataBlocks::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString(L"<" + m_sName + L" count=\"" + std::to_wstring(m_arrItems.size()) + L"\">");

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</" + m_sName + L">");
		}
		//-------------------------------------------------------------------------------------
		CMetadataTypes::CMetadataTypes() {}
		CMetadataTypes::~CMetadataTypes() {}
		void CMetadataTypes::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMetadataTypes::toXML() const
		{
			return L"";
		}
		EElementType CMetadataTypes::getType() const
		{
			return et_x_MetadataTypes;
		}
		void CMetadataTypes::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"metadataType" == sName)
				{
					CMetadataType* pT = new CMetadataType();
					*pT = oReader;
					m_arrItems.push_back(pT);
				}
			}
		}
		void CMetadataTypes::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString(L"<metadataTypes count=\"" + std::to_wstring(m_arrItems.size()) + L"\">");

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</metadataTypes>");
		}
		//--------------------------------------------------------------------------------------------------------
		CMetadataType::CMetadataType() {}
		CMetadataType::~CMetadataType() {}
		void CMetadataType::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMetadataType::toXML() const
		{
			return L"";
		}
		EElementType CMetadataType::getType() const
		{
			return et_x_MetadataType;
		}
		void CMetadataType::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<metadataType");
			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
			WritingStringNullableAttrInt2(L"minSupportedVersion", m_oMinSupportedVersion);
			WritingStringNullableAttrBool2(L"ghostRow", m_oGhostRow);
			WritingStringNullableAttrBool2(L"ghostCol", m_oGhostCol);
			WritingStringNullableAttrBool2(L"edit", m_oEdit);
			WritingStringNullableAttrBool2(L"delete", m_oDelete);
			WritingStringNullableAttrBool2(L"copy", m_oCopy);
			WritingStringNullableAttrBool2(L"pasteAll", m_oPasteAll);
			WritingStringNullableAttrBool2(L"pasteFormulas", m_oPasteFormulas);
			WritingStringNullableAttrBool2(L"pasteValues", m_oPasteValues);
			WritingStringNullableAttrBool2(L"pasteFormats", m_oPasteFormats);
			WritingStringNullableAttrBool2(L"pasteComments", m_oPasteComments);
			WritingStringNullableAttrBool2(L"pasteDataValidation", m_oPasteDataValidation);
			WritingStringNullableAttrBool2(L"pasteBorders", m_oPasteBorders);
			WritingStringNullableAttrBool2(L"pasteColWidths", m_oPasteColWidths);
			WritingStringNullableAttrBool2(L"pasteNumberFormats", m_oPasteNumberFormats);
			WritingStringNullableAttrBool2(L"merge", m_oMerge);
			WritingStringNullableAttrBool2(L"splitFirst", m_oSplitFirst);
			WritingStringNullableAttrBool2(L"splitAll", m_oSplitAll);
			WritingStringNullableAttrBool2(L"rowColShift", m_oRowColShift);
			WritingStringNullableAttrBool2(L"clearAll", m_oClearAll);
			WritingStringNullableAttrBool2(L"clearFormats", m_oClearFormats);
			WritingStringNullableAttrBool2(L"clearContents", m_oClearContents);
			WritingStringNullableAttrBool2(L"clearComments", m_oClearComments);
			WritingStringNullableAttrBool2(L"assign", m_oAssign);
			WritingStringNullableAttrBool2(L"coerce", m_oCoerce);
			WritingStringNullableAttrBool2(L"cellMeta", m_oCellMeta);
			writer.WriteString(L"/>");
		}
		void CMetadataType::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CMetadataType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"name", m_oName)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"minSupportedVersion", m_oMinSupportedVersion)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"ghostRow", m_oGhostRow)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"ghostCol", m_oGhostCol)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"edit", m_oEdit)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"delete", m_oDelete)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"copy", m_oCopy)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pasteAll", m_oPasteAll)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pasteFormulas", m_oPasteFormulas)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pasteValues", m_oPasteValues)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pasteFormats", m_oPasteFormats)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pasteComments", m_oPasteComments)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pasteDataValidation", m_oPasteDataValidation)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pasteBorders", m_oPasteBorders)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pasteColWidths", m_oPasteColWidths)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"pasteNumberFormats", m_oPasteNumberFormats)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"merge", m_oMerge)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"splitFirst", m_oSplitFirst)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"splitAll", m_oSplitAll)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"rowColShift", m_oRowColShift)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"clearAll", m_oClearAll)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"clearFormats", m_oClearFormats)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"clearContents", m_oClearContents)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"clearComments", m_oClearComments)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"assign", m_oAssign)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"coerce", m_oCoerce)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"cellMeta", m_oCellMeta)
			WritingElement_ReadAttributes_End(oReader)
		}
		//--------------------------------------------------------------------------------------------------------
		CFutureMetadataBlock::CFutureMetadataBlock() {}
		CFutureMetadataBlock::~CFutureMetadataBlock() {}
		void CFutureMetadataBlock::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CFutureMetadataBlock::toXML() const
		{
			return L"";
		}
		EElementType CFutureMetadataBlock::getType() const
		{
			return et_x_FutureMetadataBlock;
		}
		void CFutureMetadataBlock::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<bk>");

			if (m_oExtLst.IsInit())
			{
				writer.WriteString(m_oExtLst->toXMLWithNS(L""));
			}
			writer.WriteString(L"</bk>");
		}
		void CFutureMetadataBlock::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"extLst" == sName)
					m_oExtLst = oReader;
			}
		}
		//-------------------------------------------------------------------------------------
		CFutureMetadata::CFutureMetadata() {}
		CFutureMetadata::~CFutureMetadata()
		{
		}
		void CFutureMetadata::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CFutureMetadata::toXML() const
		{
			return L"";
		}
		EElementType CFutureMetadata::getType() const
		{
			return et_x_FutureMetadata;
		}
		void CFutureMetadata::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"bk" == sName)
				{
					CFutureMetadataBlock* pT = new CFutureMetadataBlock();
					*pT = oReader;
					m_arrItems.push_back(pT);
				}
			}
		}
		void CFutureMetadata::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"name", m_oName)
			WritingElement_ReadAttributes_End(oReader)
		}
		void CFutureMetadata::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString(L"<futureMetadata");
				WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
				writer.WriteString(L" count=\"" + std::to_wstring(m_arrItems.size()) + L"\"");
			writer.WriteString(L">");

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</futureMetadata>");
		}
		//--------------------------------------------------------------------------------------------------------
		CMetadata::CMetadata() {}
		CMetadata::~CMetadata()
		{
			for (size_t i = 0; i < m_arFutureMetadata.size(); ++i)
			{
				if (m_arFutureMetadata[i]) delete m_arFutureMetadata[i];
			}
			m_arFutureMetadata.clear();
		}
		void CMetadata::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMetadata::toXML() const
		{
			return L"";
		}
		EElementType CMetadata::getType() const
		{
			return et_x_Metadata;
		}
		void CMetadata::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<metadata \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:xlrd=\"http://schemas.microsoft.com/office/spreadsheetml/2017/richdata\" \
xmlns:xda=\"http://schemas.microsoft.com/office/spreadsheetml/2017/dynamicarray\">");

			if (m_oMetadataTypes.IsInit())
			{
				m_oMetadataTypes->toXML(writer);
			}
			if (m_oMetadataStrings.IsInit())
			{
				m_oMetadataStrings->toXML(writer);
			}
			if (m_oMdxMetadata.IsInit())
			{
				m_oMdxMetadata->toXML(writer);
			}
			for (size_t i = 0; i < m_arFutureMetadata.size(); ++i)
			{
				if (m_arFutureMetadata[i])
				{
					m_arFutureMetadata[i]->toXML(writer);
				}
			}
			if (m_oCellMetadata.IsInit())
			{
				m_oCellMetadata->m_sName = L"cellMetadata";
				m_oCellMetadata->toXML(writer);
			}
			if (m_oValueMetadata.IsInit())
			{
				m_oValueMetadata->m_sName = L"valueMetadata";
				m_oValueMetadata->toXML(writer);
			}
			if (m_oExtLst.IsInit())
			{
				writer.WriteString(m_oExtLst->toXMLWithNS(L""));
			}
			writer.WriteString(L"</metadata>");
		}
		void CMetadata::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"metadataTypes" == sName)
					m_oMetadataTypes = oReader;
				else if (L"metadataStrings" == sName)
					m_oMetadataStrings = oReader;
				else if (L"mdxMetadata" == sName)
					m_oMdxMetadata = oReader;
				else if (L"cellMetadata" == sName)
					m_oCellMetadata = oReader;
				else if (L"valueMetadata" == sName)
					m_oValueMetadata = oReader;
				else if (L"futureMetadata" == sName)
				{
					CFutureMetadata* pF = new CFutureMetadata(); 
					*pF = oReader;
					m_arFutureMetadata.push_back(pF);
				}
				else if (L"extLst" == sName)
					m_oExtLst = oReader;
			}
		}
		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		CMetadataFile::CMetadataFile(OOX::Document* pMain) : OOX::File(pMain)
		{
		}
		CMetadataFile::CMetadataFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
		{
			read(oRootPath, oPath);
		}
		CMetadataFile::~CMetadataFile()
		{
		}
		void CMetadataFile::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		const OOX::FileType CMetadataFile::type() const
		{
			return OOX::Spreadsheet::FileTypes::Metadata;
		}
		const CPath CMetadataFile::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CMetadataFile::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CMetadataFile::GetReadPath()
		{
			return m_oReadPath;
		}
		void CMetadataFile::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;

			XmlUtils::CXmlLiteReader oReader;

			if (!oReader.FromFile(oPath.GetPath()))
				return;

			if (!oReader.ReadNextNode())
				return;

			m_oMetadata = oReader;
		}
		void CMetadataFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			if (false == m_oMetadata.IsInit()) return;

			NSStringUtils::CStringBuilder sXml;

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			m_oMetadata->toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

			oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
		}
//-------------------------------------------------------------------------------------------------------------------------------------
		CDynamicArrayProperties::CDynamicArrayProperties() {}
		CDynamicArrayProperties::~CDynamicArrayProperties() {}
		void CDynamicArrayProperties::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDynamicArrayProperties::toXML() const
		{
			return L"";
		}
		EElementType CDynamicArrayProperties::getType() const
		{
			return et_x_DynamicArrayProperties;
		}
		void CDynamicArrayProperties::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<xda:dynamicArrayProperties");
			WritingStringNullableAttrBool2(L"fDynamic", m_oFDynamic);
			WritingStringNullableAttrBool2(L"fCollapsed", m_oFCollapsed);
			writer.WriteString(L"/>");
		}
		void CDynamicArrayProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CDynamicArrayProperties::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"fDynamic", m_oFDynamic)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"fCollapsed", m_oFCollapsed)
			WritingElement_ReadAttributes_End(oReader)
		}
//-------------------------------------------------------------------------------------------------------------------------------------
		CRichValueBlock::CRichValueBlock() {}
		CRichValueBlock::~CRichValueBlock() {}
		void CRichValueBlock::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CRichValueBlock::toXML() const
		{
			return L"";
		}
		EElementType CRichValueBlock::getType() const
		{
			return et_x_RichValueBlock;
		}
		void CRichValueBlock::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<xlrd:rvb");
			WritingStringNullableAttrInt2(L"i", m_oI);
			writer.WriteString(L"/>");
		}
		void CRichValueBlock::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CRichValueBlock::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"i", m_oI)
			WritingElement_ReadAttributes_End(oReader)
		}
	}

} // namespace OOX

