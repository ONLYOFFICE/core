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

#include "../../XlsbFormat/Xlsb.h"
#include "../../XlsbFormat/MetadataStream.h"
#include "../../XlsbFormat/Biff12_unions/ESSTR.h"
#include "../../XlsbFormat/Biff12_unions/ESMDTINFO.h"
#include "../../XlsbFormat/Biff12_unions/ESMDB.h"
#include "../../XlsbFormat/Biff12_unions/ESFMD.h"
#include "../../XlsbFormat/Biff12_unions/FMD.h"
#include "../../XlsbFormat/Biff12_unions/ESMDX.h"
#include "../../XlsbFormat/Biff12_unions/MDX.h"
#include "../../XlsbFormat/Biff12_unions/MDXKPI.h"
#include "../../XlsbFormat/Biff12_unions/MDXMBRPROP.h"
#include "../../XlsbFormat/Biff12_unions/MDXSET.h"
#include "../../XlsbFormat/Biff12_unions/MDXTUPLE.h"
#include "../../XlsbFormat/Biff12_unions/DYNAMICARRAYMETADATA.h"
#include "../../XlsbFormat/Biff12_unions/RICHDATAMETADATA.h"

#include "../../XlsbFormat/Biff12_records/Str.h"
#include "../../XlsbFormat/Biff12_records/Mdtinfo.h"
#include "../../XlsbFormat/Biff12_records/Mdb.h"
#include "../../XlsbFormat/Biff12_records/BeginEsmdb.h"
#include "../../XlsbFormat/Biff12_records/BeginEsfmd.h"
#include "../../XlsbFormat/Biff12_records/EndDynamicArrayPr.h"
#include "../../XlsbFormat/Biff12_records/BeginRichValueBlock.h"
#include "../../XlsbFormat/Biff12_records/BeginMdx.h"
#include "../../XlsbFormat/Biff12_records/BeginMdxKpi.h"
#include "../../XlsbFormat/Biff12_records/BeginMdxMbrProp.h"
#include "../../XlsbFormat/Biff12_records/BeginMdxSet.h"
#include "../../XlsbFormat/Biff12_records/BeginMdxTuple.h"
#include "../../XlsbFormat/Biff12_records/MdxMbrIstr.h"

#include "../FileTypes_Spreadsheet.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

#include "../../DocxFormat/Drawing/DrawingExt.h"
#include "../../../DesktopEditor/common/File.h"

#include "../../Binary/Presentation/XmlWriter.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"

#include "../../Binary/XlsbFormat/FileTypes_SpreadsheetBin.h"

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

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CMdxKPI::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"n", m_oN)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"np", m_oNp)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"p", m_oP)
				WritingElement_ReadAttributes_End(oReader)
		}
		XLS::BaseObjectPtr CMdxKPI::toBin() const
		{
			auto ptr(new XLSB::MDXKPI);
			XLS::BaseObjectPtr objectPtr(ptr);
			auto ptr1(new XLSB::BeginMdxKpi);
			ptr->m_BrtBeginMdxKpi = XLS::BaseObjectPtr{ptr1};
			ptr1->istrKPIName = 0;
			ptr1->istrMbrKPI = 0;
			ptr1->kpiprop = XLSB::KPIProp::KPIPROPVALUE;
			if(m_oN.IsInit())
				ptr1->istrKPIName = m_oN.get();
			if(m_oNp.IsInit())
				ptr1->istrMbrKPI = m_oNp.get();
			if(m_oP.IsInit())
			{
				auto numProp = static_cast<_UINT32>(m_oP.get().GetValue()) + 1;
				if(numProp > 0 && numProp <= 6)
					ptr1->kpiprop = static_cast<XLSB::KPIProp>(numProp);
			}
			return objectPtr;
		}
		void CMdxKPI::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::MDXKPI*>(obj.get());
			if(ptr->m_BrtBeginMdxKpi == nullptr)
				return;
			auto beginPtr = static_cast<XLSB::BeginMdxKpi*>(ptr->m_BrtBeginMdxKpi.get());
			m_oN = beginPtr->istrKPIName;
			m_oNp = beginPtr->istrMbrKPI;
			auto numProp = static_cast<_INT32>(beginPtr->kpiprop) - 1;
			m_oP.Init();
			m_oP->SetValueFromByte(numProp);
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

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CMdxMemeberProp::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"n", m_oN)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"np", m_oNp)
				WritingElement_ReadAttributes_End(oReader)
		}
		XLS::BaseObjectPtr CMdxMemeberProp::toBin() const
		{
			auto ptr(new XLSB::MDXMBRPROP);
			auto ptr1(new XLSB::BeginMdxMbrProp);
			XLS::BaseObjectPtr objectPtr(ptr);
			ptr->m_BrtBeginMdxMbrProp = XLS::BaseObjectPtr(ptr1);
			ptr1->istrMbr = 0;
			ptr1->istrProp = 0;
			if(m_oN.IsInit())
				ptr1->istrMbr = m_oN.get();
			if(m_oNp.IsInit())
				ptr1->istrProp = m_oNp.get();
			return objectPtr;
		}
		void CMdxMemeberProp::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::MDXMBRPROP*>(obj.get());
			if(ptr->m_BrtBeginMdxMbrProp == nullptr)
				return;
			auto ptr1 = static_cast<XLSB::BeginMdxMbrProp*>(ptr->m_BrtBeginMdxMbrProp.get());
			m_oN = ptr1->istrMbr;
			m_oNp = ptr1->istrProp;
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
		XLS::BaseObjectPtr CMdxSet::toBin() const
		{
			auto ptr(new XLSB::MDXSET);
			XLS::BaseObjectPtr objectPtr(ptr);
			auto ptr1(new XLSB::BeginMdxSet);
			ptr->m_BrtBeginMdxSet = XLS::BaseObjectPtr{ptr1};

			ptr1->istrSetDef = 0;
			ptr1->cMbrsSortBy = 0;
			ptr1->sso = XLSB::SdSetSortOrderMdx::SSONONE;
			
			if(m_oNs.IsInit())
				ptr1->istrSetDef = m_oNs.get();
			if(m_oC.IsInit())
				ptr1->cMbrsSortBy = m_oC.get();
			if(m_oO.IsInit())
			{
				auto orderNum = static_cast<_UINT32>(m_oO->GetValue());
				if(orderNum >= 0 && orderNum <=6)
					ptr1->sso = static_cast<XLSB::SdSetSortOrderMdx>(orderNum);
			}
			for(auto i:m_arrItems)
				ptr->MdxMbrIstrs.push_back(i->toBin());
			return objectPtr;
		}
		void CMdxSet::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::MDXSET*>(obj.get());
			if(ptr->m_BrtBeginMdxSet != nullptr)
			{
				auto ptr1 = static_cast<XLSB::BeginMdxSet*>(ptr->m_BrtBeginMdxSet.get());
				m_oNs = ptr1->istrSetDef;
				m_oC = ptr1->cMbrsSortBy;
				auto orderNum = static_cast<_INT32>(ptr1->sso);
				m_oO.Init();
				m_oO->SetValueFromByte(orderNum);
			}
			for(auto i : ptr->MdxMbrIstrs)
				m_arrItems.push_back(new CMetadataStringIndex(i));
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

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CMetadataStringIndex::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"x", m_oX)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"s", m_oS)
				WritingElement_ReadAttributes_End(oReader)
		}
		XLS::BaseObjectPtr CMetadataStringIndex::toBin() const
		{
			auto ptr(new XLSB::MdxMbrIstr);
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_oX.IsInit())
				ptr->istr = m_oX.get();
			if(m_oS.IsInit())
				ptr->fCubeSet = m_oS.get();
			return objectPtr;
		}
		void CMetadataStringIndex::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::MdxMbrIstr*>(obj.get());
			m_oX = ptr->istr;
			m_oS = ptr->fCubeSet;
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
		XLS::BaseObjectPtr CMdxTuple::toBin() const
		{
			auto ptr(new XLSB::MDXTUPLE);
			XLS::BaseObjectPtr objectPtr(ptr);
			auto ptr1(new XLSB::BeginMdxTuple);
			ptr->m_BrtBeginMdxTuple = XLS::BaseObjectPtr{ptr1};
			
			if(m_oC.IsInit())
				ptr1->cMbrs = m_oC.get();
			if(m_oI.IsInit())
				ptr1->fSrvFmtItalic = m_oI.get();
			if(m_oB.IsInit())
				ptr1->fSrvFmtBold = m_oB.get();
			if(m_oU.IsInit())
				ptr1->fSrvFmtUnderline = m_oU.get();
			if(m_oSt.IsInit())
				ptr1->fSrvFmtStrikethrough = m_oSt.get();
			
			if(m_oBc.IsInit())
			{
				ptr1->fSrvFmtBack = true;
				ptr1->dwSrvFmtBack.fromHex(m_oBc.get());
			}
			if(m_oFc.IsInit())
			{
				ptr1->fSrvFmtFore = true;
				ptr1->dwSrvFmtFore.fromHex(m_oFc.get());
			}
			if(m_oCt.IsInit())
			{
				ptr1->fSrvFmtNum = true;
				ptr1->fSrvFmtNumCurrency = true;
				ptr1->stSfnum = m_oCt.get();
			}

			for(auto i : m_arrItems)
				ptr->MdxMbrIstrs.push_back(i->toBin());
			return objectPtr;
		}
		void CMdxTuple::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::MDXTUPLE*>(obj.get());
			if(ptr->m_BrtBeginMdxTuple != nullptr)
			{
				auto ptr1 = static_cast<XLSB::BeginMdxTuple*>(ptr->m_BrtBeginMdxTuple.get());
				m_oC = ptr1->cMbrs;
				m_oI = ptr1->fSrvFmtItalic;
				m_oB = ptr1->fSrvFmtBold;
				m_oU = ptr1->fSrvFmtUnderline;
				m_oSt = ptr1->fSrvFmtStrikethrough;
				if(ptr1->fSrvFmtBack)
					m_oBc = ptr1->dwSrvFmtBack.toHex();
				if(ptr1->fSrvFmtFore)
					m_oFc = ptr1->dwSrvFmtFore.toHex();
				if(ptr1->fSrvFmtNum && ptr1->fSrvFmtNumCurrency)
					m_oCt = ptr1->stSfnum;
			}
			for(auto i : ptr->MdxMbrIstrs)
				m_arrItems.push_back(new CMetadataStringIndex(i));
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
		XLS::BaseObjectPtr CMdx::toBin() const
		{
			auto ptr(new XLSB::MDX);
			XLS::BaseObjectPtr objectPtr(ptr);
			auto ptr1(new XLSB::BeginMdx);
			ptr->m_BrtBeginMdx = XLS::BaseObjectPtr{ptr1};
			ptr1->istrConnName = 0;
			ptr1->tfnSrc = XLSB::TagFnMdx::CUBEKPIMEMBER;
			if(m_oN.IsInit())
				ptr1->istrConnName = m_oN.get();
			if(m_oF.IsInit())
			{
				auto funcNum = static_cast<_UINT32>(m_oF.get().GetValue()) + 1;
				if(funcNum > 0 && funcNum <= 7)
					ptr1->tfnSrc = static_cast<XLSB::TagFnMdx>(funcNum);
			}
			if(m_oCMdxKPI.IsInit())
			{
				ptr->m_MDXKPI = m_oCMdxKPI->toBin();
			}
			if(m_oMdxMemeberProp.IsInit())
			{
				ptr->m_MDXMBRPROP = m_oMdxMemeberProp->toBin();
			}
			if(m_oMdxSet.IsInit())
			{
				ptr->m_MDXSET = m_oMdxSet->toBin();
			}
			if(m_oMdxTuple.IsInit())
			{
				ptr->m_MDXTUPLE = m_oMdxTuple->toBin();
			}
			return objectPtr;
		}
		void CMdx::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::MDX*>(obj.get());
			if(ptr->m_BrtBeginMdx != nullptr)
			{
				auto beginPtr = static_cast<XLSB::BeginMdx*>(ptr->m_BrtBeginMdx.get());
				m_oN = beginPtr->istrConnName;
				auto numFunc = static_cast<_INT32>(beginPtr->tfnSrc) - 1;
				m_oF.Init();
				m_oF->SetValueFromByte(numFunc);
			}
			if(ptr->m_MDXKPI != nullptr)
				m_oCMdxKPI = ptr->m_MDXKPI;
			
			if(ptr->m_MDXMBRPROP != nullptr)
				m_oMdxMemeberProp = ptr->m_MDXMBRPROP;

			if(ptr->m_MDXSET != nullptr)
				m_oMdxSet = ptr->m_MDXSET;

			if(ptr->m_MDXTUPLE != nullptr)
				m_oMdxTuple = ptr->m_MDXTUPLE;
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
		XLS::BaseObjectPtr CMdxMetadata::toBin() const
		{
			auto ptr(new XLSB::ESMDX);
			XLS::BaseObjectPtr objectPtr(ptr);
			for(auto i:m_arrItems)
				ptr->MDXs.push_back(i->toBin());
			return objectPtr;
		}
		void CMdxMetadata::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ESMDX*>(obj.get());
			for(auto i : ptr->MDXs)
				m_arrItems.push_back(new CMdx(i));
			m_oCount = m_arrItems.size();
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
		XLS::BaseObjectPtr CMetadataStrings::toBin() const
		{
			auto ptr(new XLSB::ESSTR);
			for(auto str:m_arrItems)
				ptr->m_BrtStrs.push_back(str->toBin());
			XLS::BaseObjectPtr objectPtr(ptr);
			return objectPtr;
		}
		void CMetadataStrings::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ESSTR*>(obj.get());
			m_oCount = ptr->m_BrtStrs.size();
			for(auto i:ptr->m_BrtStrs)
				m_arrItems.push_back(new CMetadataString(i));
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
		XLS::BaseObjectPtr CMetadataString::toBin() const
		{
			auto ptr(new XLSB::Str);
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_oV.IsInit())
				ptr->stText = m_oV.get();
			else
				ptr->stText = L"";
			return objectPtr;
		}
		void CMetadataString::fromBin(XLS::BaseObjectPtr& obj)
		{
			if(obj == nullptr)
				return;
			auto ptr = static_cast<XLSB::Str*>(obj.get());
			m_oV =  ptr->stText;
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

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
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

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
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
		XLS::BaseObjectPtr CMetadataBlock::toBin() const
		{
			auto ptr(new XLSB::Mdb);
			XLS::BaseObjectPtr objectPtr(ptr);
			ptr->cMdir = m_arrItems.size();
			for(auto item : m_arrItems)
			{
				XLSB::Mdir dir;
				if(item->m_oT.IsInit())
					dir.imdt = item->m_oT.get();
				else
					dir.imdt = 0;
				if(item->m_oV.IsInit())
					dir.mdd = item->m_oV.get();
				else
					dir.mdd = 0;
				ptr->rgMdir.push_back(dir);
			}
			return objectPtr;
		}
		void CMetadataBlock::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::Mdb*>(obj.get());
			for(auto i : ptr->rgMdir)
			{
				auto  record(new CMetadataRecord);
				record->m_oT = i.imdt;
				record->m_oV = i.mdd;
				m_arrItems.push_back(record);
			}
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
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

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
		XLS::BaseObjectPtr CMetadataBlocks::toBin(const bool cellMeta) const
		{
			auto ptr(new XLSB::ESMDB);
			XLS::BaseObjectPtr objectPtr(ptr);
			auto ptr1(new XLSB::BeginEsmdb);
			ptr->m_BrtBeginEsmdb = XLS::BaseObjectPtr(ptr1);
			ptr1->fCellMeta = cellMeta;

			for(auto i:m_arrItems)
				ptr->m_BrtMdbs.push_back(i->toBin());
			ptr1->CMdb = ptr->m_BrtMdbs.size();

			return objectPtr;
		}
		void CMetadataBlocks::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ESMDB*>(obj.get());
			m_oCount = ptr->m_BrtMdbs.size();
			for(auto i: ptr->m_BrtMdbs)
				m_arrItems.push_back(new CMetadataBlock(i));
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
		XLS::BaseObjectPtr CMetadataTypes::toBin() const
		{
			auto ptr(new XLSB::ESMDTINFO);
			XLS::BaseObjectPtr objectPtr(ptr);
			for(auto i :m_arrItems)
			{
				ptr->BrtMdtinfos.push_back(i->toBin());
			}
			return objectPtr;
		}
		void CMetadataTypes::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ESMDTINFO*>(obj.get());
			m_oCount = ptr->BrtMdtinfos.size();

			for(auto i : ptr->BrtMdtinfos)
			{
				m_arrItems.push_back(new CMetadataType(i));
			}
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

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
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
		XLS::BaseObjectPtr CMetadataType::toBin() const
		{
			auto ptr(new XLSB::Mdtinfo);
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_oMinSupportedVersion.IsInit())
				ptr->metadataID = m_oMinSupportedVersion.get();
			else
				ptr->metadataID = 0;
			if(m_oName.IsInit())
				ptr->stName = m_oName.get();
			else
				ptr->stName = L"";

			if(m_oGhostRow.IsInit())
				ptr->fGhostRw = m_oGhostRow.get();
			if(m_oGhostCol.IsInit())
				ptr->fGhostCol = m_oGhostCol.get();
			if(m_oEdit.IsInit())
				ptr->fEdit = m_oEdit.get();
			if(m_oDelete.IsInit())
				ptr->fDelete = m_oDelete.get();
			if(m_oCopy.IsInit())
				ptr->fCopy = m_oCopy.get();
			if(m_oPasteAll.IsInit())
				ptr->fPasteAll = m_oPasteAll.get();
			if(m_oPasteFormulas.IsInit())
				ptr->fPasteFmlas = m_oPasteFormulas.get();
			if(m_oPasteValues.IsInit())
				ptr->fPasteValues = m_oPasteValues.get();
			if(m_oPasteFormats.IsInit())
				ptr->fPasteFmts = m_oPasteFormats.get();
			if(m_oPasteComments.IsInit())
				ptr->fPasteComments = m_oPasteComments.get();
			if(m_oPasteDataValidation.IsInit())
				ptr->fPasteDv = m_oPasteDataValidation.get();
			if(m_oPasteBorders.IsInit())
				ptr->fPasteBorders = m_oPasteBorders.get();
			if(m_oPasteColWidths.IsInit())
				ptr->fPasteColWidths = m_oPasteColWidths.get();
			if(m_oPasteNumberFormats.IsInit())
				ptr->fPasteNumFmts = m_oPasteNumberFormats.get();
			if(m_oMerge.IsInit())
				ptr->fMerge = m_oMerge.get();
			if(m_oSplitFirst.IsInit())
				ptr->fSplitFirst = m_oSplitFirst.get();
			if(m_oSplitAll.IsInit())
				ptr->fSplitAll = m_oSplitAll.get();
			if(m_oRowColShift.IsInit())
				ptr->fRwColShift = m_oRowColShift.get();
			if(m_oClearAll.IsInit())
				ptr->fClearAll = m_oClearAll.get();
			if(m_oClearFormats.IsInit())
				ptr->fClearFmts = m_oClearFormats.get();
			if(m_oClearContents.IsInit())
				ptr->fClearContents = m_oClearContents.get();
			if(m_oClearComments.IsInit())
				ptr->fClearComments = m_oClearComments.get();
			if(m_oAssign.IsInit())
				ptr->fAssign = m_oAssign.get();
			if(m_oCoerce.IsInit())
				ptr->fCanCoerce = m_oCoerce.get();
			if(m_oCellMeta.IsInit())
				ptr->fCellMeta = m_oCellMeta.get();
			return objectPtr;
		}
		void CMetadataType::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::Mdtinfo*>(obj.get());
			m_oName = ptr->stName;
			m_oMinSupportedVersion = ptr->metadataID;
			m_oGhostRow = ptr->fGhostRw;
			m_oGhostCol = ptr->fGhostCol;
			m_oEdit = ptr->fEdit;
			m_oDelete = ptr->fDelete;
			m_oCopy = ptr->fCopy;
			m_oPasteAll = ptr->fPasteAll;
			m_oPasteFormulas = ptr->fPasteFmlas;
			m_oPasteValues = ptr->fPasteValues;
			m_oPasteFormats = ptr->fPasteFmts;
			m_oPasteComments = ptr->fPasteComments;
			m_oPasteDataValidation = ptr->fPasteDv;
			m_oPasteBorders = ptr->fPasteBorders;
			m_oPasteColWidths = ptr->fPasteColWidths;
			m_oPasteNumberFormats = ptr->fPasteNumFmts;
			m_oMerge = ptr->fMerge;
			m_oSplitFirst = ptr->fSplitFirst;
			m_oSplitAll = ptr->fSplitAll;
			m_oRowColShift = ptr->fRwColShift;
			m_oClearAll = ptr->fClearAll;
			m_oClearFormats = ptr->fClearFmts;
			m_oClearContents = ptr->fClearContents;
			m_oClearComments = ptr->fClearComments;
			m_oAssign = ptr->fAssign;
			m_oCoerce = ptr->fCanCoerce;
			m_oCellMeta = ptr->fCellMeta;
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
		XLS::BaseObjectPtr CFutureMetadataBlock::toBin() const
		{
			return m_oExtLst->toBinMetadata();
		}
		void CFutureMetadataBlock::fromBin(XLS::BaseObjectPtr& obj)
		{
            m_oExtLst = OOX::Drawing::COfficeArtExtensionList();
			m_oExtLst->fromBin(obj);
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
		XLS::BaseObjectPtr CFutureMetadata::toBin() const
		{
			auto ptr(new XLSB::ESFMD);
			XLS::BaseObjectPtr objectPtr(ptr);
			auto ptr1(new XLSB::BeginEsfmd);
			ptr->m_BrtBeginEsfmd = XLS::BaseObjectPtr(ptr1);
			if(m_oName.IsInit())
				ptr1->stName = m_oName.get();
			else
				ptr1->stName = L"";
			for(auto i:m_arrItems)
            {
                auto fmd = i->toBin();
                if(fmd)
                    ptr->FMDs.push_back(fmd);
            }
            ptr1->cFmd = ptr->FMDs.size();
			return objectPtr;
		}
		void CFutureMetadata::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ESFMD*>(obj.get());
			if(ptr->m_BrtBeginEsfmd != nullptr)
			{
				auto beginPtr = static_cast<XLSB::BeginEsfmd*>(ptr->m_BrtBeginEsfmd.get());
				m_oName = beginPtr->stName;
			}
			for(auto i : ptr->FMDs)
			{
				m_arrItems.push_back(new CFutureMetadataBlock(i));
			}
			m_oCount = m_arrItems.size();
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
		void CMetadata::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto streamPtr = static_cast<XLSB::MetadataStream*>(obj.get());
			if(streamPtr->m_ESSTR != nullptr)
				m_oMetadataStrings = streamPtr->m_ESSTR;
			if(streamPtr->m_ESMDTINFO != nullptr)
				m_oMetadataTypes = streamPtr->m_ESMDTINFO;
            if(streamPtr->m_ValueMetadataBlocks != nullptr)
                m_oValueMetadata = streamPtr->m_ValueMetadataBlocks;
            if(streamPtr->m_CellMetadataBlocks != nullptr)
                m_oCellMetadata = streamPtr->m_CellMetadataBlocks;
			if(streamPtr->m_ESMDX != nullptr)
				m_oMdxMetadata = streamPtr->m_ESMDX;
			for(auto i : streamPtr->m_ESFMDs)
				m_arFutureMetadata.push_back(new CFutureMetadata(i));
		}
		XLS::BaseObjectPtr CMetadata::toBin() const
		{
			XLSB::MetadataStreamPtr streamPtr(new XLSB::MetadataStream);

			if(m_oMetadataStrings.IsInit())
				streamPtr->m_ESSTR = m_oMetadataStrings->toBin();
			if(m_oMetadataTypes.IsInit())
				streamPtr->m_ESMDTINFO = m_oMetadataTypes->toBin();
			if(m_oCellMetadata.IsInit())
				streamPtr->m_CellMetadataBlocks = m_oCellMetadata->toBin(true);
			if(m_oValueMetadata.IsInit())
				streamPtr->m_ValueMetadataBlocks = m_oValueMetadata->toBin(false);
			for(auto i : m_arFutureMetadata)
			{
				streamPtr->m_ESFMDs.push_back(i->toBin());
			}
			if(m_oMdxMetadata.IsInit())
				streamPtr->m_ESMDX = m_oMdxMetadata->toBin();
			return streamPtr;
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
			CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if ((xlsb) && (xlsb->m_bWriteToXlsb))
			{
				return OOX::SpreadsheetBin::FileTypes::MetadataBin;
			}
			return OOX::Spreadsheet::FileTypes::Metadata;
		}
		const CPath CMetadataFile::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CMetadataFile::DefaultFileName() const
		{
			CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if ((xlsb) && (xlsb->m_bWriteToXlsb))
			{
				CPath name = type().DefaultFileName();
				name.SetExtention(L"bin");
				return name;
			}
			else
			{
				return type().DefaultFileName();
			}
		}
		const CPath& CMetadataFile::GetReadPath()
		{
			return m_oReadPath;
		}
		void CMetadataFile::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;

			if (m_oReadPath.GetExtention() == _T(".bin"))
			{
				readBin(m_oReadPath);
			}
			else
			{
				XmlUtils::CXmlLiteReader oReader;

				if (!oReader.FromFile(oPath.GetPath()))
					return;

				if (!oReader.ReadNextNode())
					return;

				m_oMetadata = oReader;
			}
		}
		void CMetadataFile::readBin(const CPath& oPath)
		{
			CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if (xlsb)
			{
				XLSB::MetadataStreamPtr streamPtr(new XLSB::MetadataStream);

				xlsb->ReadBin(oPath, streamPtr.get());
				XLS::BaseObjectPtr objectPtr(streamPtr);
                m_oMetadata = CMetadata();
				m_oMetadata->fromBin(objectPtr);
			}
		}
		XLS::BaseObjectPtr CMetadataFile::WriteBin() const
		{
			return m_oMetadata->toBin();
		}
		void CMetadataFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			if (false == m_oMetadata.IsInit()) return;

			CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if ((xlsb) && (xlsb->m_bWriteToXlsb))
			{
				auto object = WriteBin();
				xlsb->WriteBin(oPath, object.get());
			}
			else
			{
				NSStringUtils::CStringBuilder sXml;

				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
				m_oMetadata->toXML(sXml);

				std::wstring sPath = oPath.GetPath();
				NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());
			}

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

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CDynamicArrayProperties::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"fDynamic", m_oFDynamic)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"fCollapsed", m_oFCollapsed)
			WritingElement_ReadAttributes_End(oReader)
		}
		XLS::BaseObjectPtr CDynamicArrayProperties::toBin()
		{
			auto ptr(new XLSB::EndDynamicArrayPr);
			XLS::BaseObjectPtr objectPtr(ptr);
			ptr->fDynamic = false;
			ptr->fCollapsed = false;
			if(m_oFDynamic.IsInit())
				ptr->fDynamic = m_oFDynamic.get();
			if(m_oFCollapsed.IsInit())
				ptr->fCollapsed = m_oFCollapsed.get();
			return objectPtr;
		}
		void CDynamicArrayProperties::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::DYNAMICARRAYMETADATA*>(obj.get());
			auto endPtr = static_cast<XLSB::EndDynamicArrayPr*>(ptr->m_EndDynamicArrayPr.get());
			m_oFDynamic = endPtr->fDynamic;
			m_oFCollapsed = endPtr->fCollapsed;
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

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CRichValueBlock::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"i", m_oI)
			WritingElement_ReadAttributes_End(oReader)
		}
		XLS::BaseObjectPtr CRichValueBlock::toBin()
		{
			auto ptr(new XLSB::BeginRichValueBlock);
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_oI.IsInit())
				ptr->irv = m_oI.get();
			else
				ptr->irv = 0;
			return objectPtr;
		}
		void CRichValueBlock::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::RICHDATAMETADATA*>(obj.get());
			if(ptr->m_BeginRichValueBlock == nullptr)
				return;
			auto ptr1 = static_cast<XLSB::BeginRichValueBlock*>(ptr->m_BeginRichValueBlock.get());
			m_oI = ptr1->irv;
		}
	}

} // namespace OOX

