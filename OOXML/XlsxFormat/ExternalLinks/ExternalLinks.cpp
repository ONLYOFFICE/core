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
#include "ExternalLinks.h"

#include "../Common.h"

#include "../SharedStrings/Text.h"
#include "../ComplexTypes_Spreadsheet.h"

#include "../../DocxFormat/Drawing/DrawingExt.h"

#include "../../XlsbFormat/Xlsb.h"
#include "../../XlsbFormat/ExternalLinkStream.h"

#include "../../XlsbFormat/Biff12_unions/EXTERNALLINK.h"
#include "../../XlsbFormat/Biff12_records/BeginSupBook.h"
#include "../../XlsbFormat/Biff12_unions/EXTERNALBOOK.h"
#include "../../XlsbFormat/Biff12_unions/DDEOLELINK.h"
#include "../../XlsbFormat/Biff12_records/SupTabs.h"
#include "../../XlsbFormat/Biff12_unions/EXTERNNAME.h"
#include "../../XlsbFormat/Biff12_records/SupNameStart.h"
#include "../../XlsbFormat/Biff12_records/SupNameFmla.h"
#include "../../XlsbFormat/Biff12_records/SupNameBits.h"
#include "../../XlsbFormat/Biff12_unions/EXTERNTABLE.h"
#include "../../XlsbFormat/Biff12_records/ExternTableStart.h"
#include "../../XlsbFormat/Biff12_unions/EXTERNROW.h"
#include "../../XlsbFormat/Biff12_records/ExternRowHdr.h"
#include "../../XlsbFormat/Biff12_unions/EXTERNVALUE.h"
#include "../../XlsbFormat/Biff12_records/ExternValueMeta.h"
#include "../../XlsbFormat/Biff12_unions/EXTERNVALUEDATA.h"
#include "../../XlsbFormat/Biff12_records/ExternCell.h"
#include "../../XlsbFormat/Biff12_unions/DDEOLEITEM.h"
#include "../../XlsbFormat/Biff12_unions/DDEOLEITEMVALUES.h"
#include "../../XlsbFormat/Biff12_unions/DDEOLEITEMVALUE.h"
#include "../../XlsbFormat/Biff12_records/SupNameValueStart.h"
#include "../../XlsbFormat/Biff12_records/SupName.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/GlobalWorkbookInfo.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/CellRef.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/ExtPtgErr.h"
#include "../../Binary/XlsbFormat/FileTypes_SpreadsheetBin.h"
#include <string>

namespace OOX
{
namespace Spreadsheet
{
	CExternalSheetNames::CExternalSheetNames()
	{
	}
	CExternalSheetNames::~CExternalSheetNames()
	{
	}
	void CExternalSheetNames::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CExternalSheetNames::getType() const
	{
		return et_x_ExternalSheetNames;
	}
	void CExternalSheetNames::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();
			if (L"sheetName" == sName)
			{
				m_arrItems.push_back(PPTX::CreatePtrXmlContent<ComplexTypes::Spreadsheet::String>(oReader));
			}
		}
	}
	void CExternalSheetNames::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<sheetNames>");
		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				writer.WriteString(L"<sheetName");
				WritingStringAttrEncodeXmlString(L"val", m_arrItems[i]->ToString2());
				writer.WriteString(L"/>");
			}
		}
		writer.WriteString(L"</sheetNames>");
	}
	std::wstring CExternalSheetNames::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	XLS::BaseObjectPtr CExternalSheetNames::toBin()
	{
		XLSB::SupTabsPtr supTabs(new XLSB::SupTabs);
		for (auto& item : m_arrItems)
		{
			XLSB::XLWideString str = *item->m_sVal;
			supTabs->sheetNames.push_back(str);
		}
		return supTabs;
	}
	void CExternalSheetNames::fromBin(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::SupTabs*>(obj.get());
		if (ptr != nullptr)
		{
			for (auto &item : ptr->sheetNames)
			{
				auto str = new ComplexTypes::Spreadsheet::String;
				str->m_sVal = item.value();
				m_arrItems.push_back(str);
			}
		}
	}

	CExternalDefinedName::CExternalDefinedName()
	{
	}
	CExternalDefinedName::~CExternalDefinedName()
	{
	}
	void CExternalDefinedName::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CExternalDefinedName::getType() const
	{
		return et_x_ExternalDefinedName;
	}
	void CExternalDefinedName::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void CExternalDefinedName::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<definedName");
		WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
		WritingStringNullableAttrEncodeXmlString(L"refersTo", m_oRefersTo, m_oRefersTo.get());
		WritingStringNullableAttrInt(L"sheetId", m_oSheetId, m_oSheetId->GetValue());
		writer.WriteString(L"/>");
	}
	std::wstring CExternalDefinedName::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	void CExternalDefinedName::fromBin(XLS::BaseObjectPtr& obj)
	{
		ReadAttributes(obj);
	}
	XLS::BaseObjectPtr CExternalDefinedName::toBin()
	{
		XLSB::ExternalReferenceType type;
		type = XLSB::ExternalReferenceType::WORKBOOK;
		auto ptr(new XLSB::EXTERNNAME(type));
		XLS::BaseObjectPtr objectPtr(ptr);

		if(m_oName.IsInit())
		{
			auto nameStart(new XLSB::SupNameStart);
			nameStart->name = m_oName.get();
			ptr->m_BrtSupNameStart = XLS::BaseObjectPtr{nameStart};
		}
		if(m_oRefersTo.IsInit())
		{
			auto nameFmla(new XLSB::SupNameFmla);
			ptr->m_BrtSupNameFmla = XLS::BaseObjectPtr{nameFmla};
			if(m_oRefersTo.get() == L"#REF!")
				m_oRefersTo = L"#N/A";
			nameFmla->fmla = m_oRefersTo.get();
		}
		if(m_oSheetId.IsInit())
		{
			auto supBits(new XLSB::SupNameBits(type));
			ptr->m_BrtSupNameBits = XLS::BaseObjectPtr{supBits};
			supBits->sbt = 0x0000;
			supBits->contentsExtName.iSheet = m_oSheetId->GetValue()+1;
			supBits->contentsExtName.fBuiltIn = false;
		}
		return objectPtr;
	}
	void CExternalDefinedName::ReadAttributes(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::EXTERNNAME*>(obj.get());
		if (ptr != nullptr)
		{
			auto ptrSupNameStart = static_cast<XLSB::SupNameStart*>(ptr->m_BrtSupNameStart.get());
			if (ptrSupNameStart != nullptr)
			{
				if (!ptrSupNameStart->name.value().empty())
					m_oName = ptrSupNameStart->name.value();
			}

			auto ptrSupNameFmla = static_cast<XLSB::SupNameFmla*>(ptr->m_BrtSupNameFmla.get());
			if (ptrSupNameFmla != nullptr)
			{
				auto ref = ptrSupNameFmla->fmla.getAssembledFormula();
				if (!ref.empty())
					m_oRefersTo = ref;
			}

			auto ptrSupNameBits = static_cast<XLSB::SupNameBits*>(ptr->m_BrtSupNameBits.get());
			if (ptrSupNameBits != nullptr)
			{
				m_oSheetId = ptrSupNameBits->contentsExtName.iSheet;
			}

		}
	}
	void CExternalDefinedName::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, (L"name"), m_oName)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"refersTo"), m_oRefersTo)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"sheetId"), m_oSheetId)
			WritingElement_ReadAttributes_End(oReader)
	}

	CExternalDefinedNames::CExternalDefinedNames()
	{
	}
	CExternalDefinedNames::~CExternalDefinedNames()
	{
	}
	void CExternalDefinedNames::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CExternalDefinedNames::getType() const
	{
		return et_x_ExternalDefinedNames;
	}
	void CExternalDefinedNames::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();
			if (L"definedName" == sName)
			{
				CExternalDefinedName* pExternalDefinedName = new CExternalDefinedName();
				*pExternalDefinedName = oReader;
				m_arrItems.push_back(pExternalDefinedName);
			}
		}
	}
	void CExternalDefinedNames::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<definedNames>");
		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}
		writer.WriteString(L"</definedNames>");
	}
	std::wstring CExternalDefinedNames::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	void CExternalDefinedNames::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
	{
		for (auto &item : obj)
		{
			m_arrItems.push_back(new CExternalDefinedName(item));
		}
	}
	std::vector<XLS::BaseObjectPtr> CExternalDefinedNames::toBin()
	{
		std::vector<XLS::BaseObjectPtr> objectVector;
		for(auto i:m_arrItems)
		{
			objectVector.push_back(i->toBin());
		}
		return objectVector;
	}
	CExternalCell::CExternalCell()
	{
	}
	CExternalCell::~CExternalCell()
	{
	}
	void CExternalCell::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CExternalCell::getType() const
	{
		return et_x_ExternalCell;
	}
	void CExternalCell::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();
			if (L"v" == sName)
			{
				m_oValue = oReader;
			}
		}
	}
	void CExternalCell::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cell");
		WritingStringNullableAttrString(L"r", m_oRef, m_oRef.get());
		WritingStringNullableAttrString(L"t", m_oType, m_oType->ToString());
		WritingStringNullableAttrInt2(L"vm", m_oValueMetadata);
		writer.WriteString(L">");
		if (m_oValue.IsInit())
			m_oValue->toXML2(writer, (L"v"));
		writer.WriteString(L"</cell>");
	}
	std::wstring CExternalCell::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	XLS::BaseObjectPtr CExternalCell::toBin()
	{
		
		auto ptr(new XLSB::EXTERNVALUE(0));
		XLS::BaseObjectPtr objectPtr(ptr);
		if(m_oValueMetadata.IsInit())
		{
			auto metaPtr(new XLSB::ExternValueMeta);
			ptr->m_BrtExternValueMeta = XLS::BaseObjectPtr{metaPtr};
			metaPtr->ivmb = m_oValueMetadata.get();
		}

		auto valuePtr(new XLSB::EXTERNVALUEDATA(0));
		ptr->m_EXTERNVALUEDATA = XLS::BaseObjectPtr{valuePtr};
		XLSB::RgceLoc loc;
		if(m_oRef.IsInit())
			loc = XLSB::RgceLoc(m_oRef.get());
		else
			loc = XLSB::RgceLoc(L"A1");
		valuePtr->m_Col = loc.getColumn();
		if(!m_oValue.IsInit())
		{	auto blanc(new XLSB::ExternCellBlank);
			blanc->col = valuePtr->m_Col;
			valuePtr->m_source = XLS::BaseObjectPtr{blanc};
			return objectPtr;
		}
		if(!m_oType.IsInit())
		{
			m_oType.Init();
			m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeStr);
		}
		switch(m_oType->GetValue())
		{
			case SimpleTypes::Spreadsheet::celltypeStr:
			{
				auto str(new XLSB::ExternCellString);
				str->col = valuePtr->m_Col;
				str->value = m_oValue->m_sText;
				valuePtr->m_source = XLS::BaseObjectPtr{str};
				break;
			}
			case SimpleTypes::Spreadsheet::celltypeBool:
			{
				auto boolVal(new XLSB::ExternCellBool);
				boolVal->col = valuePtr->m_Col;
				boolVal->value = m_oValue->m_sText == L"1" ? true : false;
				valuePtr->m_source = XLS::BaseObjectPtr{boolVal};
				break;
			}
			case SimpleTypes::Spreadsheet::celltypeNumber:
			{
				auto doubleVal(new XLSB::ExternCellReal);
				doubleVal->col = valuePtr->m_Col;
				doubleVal->value.data.value = std::stod(m_oValue->m_sText);
				valuePtr->m_source = XLS::BaseObjectPtr{doubleVal};
				break;
			}
			case SimpleTypes::Spreadsheet::celltypeError:
			{
				auto errVal(new XLSB::ExternCellError);
				errVal->col = valuePtr->m_Col;
				if(m_oValue->m_sText == L"#NULL!")
					errVal->value = 0;
				else if(m_oValue->m_sText == L"#DIV/0!")
					errVal->value = 0x7;
				else if(m_oValue->m_sText ==  L"#VALUE!")
					errVal->value = 0xF;
				else if(m_oValue->m_sText ==  L"#REF!")
					errVal->value = 0x17;
				else if(m_oValue->m_sText ==  L"#NAME?")
					errVal->value = 0x1D;
				else if(m_oValue->m_sText ==  L"#NUM!")
					errVal->value = 0x24;
				else if(m_oValue->m_sText ==  L"#N/A")
					errVal->value = 0x2A;
				else if(m_oValue->m_sText ==  L"#GETTING_DATA")
					errVal->value = 0x2B;
				else
					errVal->value = 0x2A;
				valuePtr->m_source = XLS::BaseObjectPtr{errVal};
				break;
			}
			default:
			{
				auto blanc(new XLSB::ExternCellBlank);
				blanc->col = valuePtr->m_Col;
				valuePtr->m_source = XLS::BaseObjectPtr{blanc};
			}
		};
		
		return objectPtr;
	}
	void CExternalCell::fromBin(XLS::BaseObjectPtr& obj)
	{
		ReadAttributes(obj);
	}
	void CExternalCell::ReadAttributes(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::EXTERNVALUE*>(obj.get());
		if (ptr != nullptr)
		{
			auto ptrExternValueMeta = static_cast<XLSB::ExternValueMeta*>(ptr->m_BrtExternValueMeta.get());
			if (ptrExternValueMeta != nullptr)
			{
				m_oValueMetadata = ptrExternValueMeta->ivmb;
			}

			auto ptrEXTERNVALUEDATA = static_cast<XLSB::EXTERNVALUEDATA*>(ptr->m_EXTERNVALUEDATA.get());
			if (ptrEXTERNVALUEDATA != nullptr)
			{
				if (!ptrEXTERNVALUEDATA->m_Ref.empty())
					m_oRef = ptrEXTERNVALUEDATA->m_Ref;

				if (ptrEXTERNVALUEDATA->m_source != nullptr)
				{
					auto pSource = static_cast<XLS::BiffRecord*>(ptrEXTERNVALUEDATA->m_source.get());
					auto nType = pSource->getTypeId();
					switch (nType)
					{
					case XLSB::rt_ExternCellBlank:
						break;
					case XLSB::rt_ExternCellReal:
					{
						auto pRealNum = reinterpret_cast<XLSB::ExternCellReal*>(pSource)->value.data.value;
						m_oType.Init();
                        m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeNumber);
						m_oValue.Init();
						m_oValue->m_sText = OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(pRealNum);
					}
					break;
					case XLSB::rt_ExternCellBool:
					{
						auto pBool = reinterpret_cast<XLSB::ExternCellBool*>(pSource)->value;
						m_oType.Init();
						m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeBool);
						m_oValue.Init();
						m_oValue->m_sText = pBool ? L"1" : L"0";
					}
					break;
					case XLSB::rt_ExternCellError:
					{
						auto pError = reinterpret_cast<XLSB::ExternCellError*>(pSource)->value;
						m_oType.Init();
						m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeError);
						m_oValue.Init();
						switch (pError)
						{
						case 0x00: m_oValue->m_sText = L"#NULL!"; break;
						case 0x07: m_oValue->m_sText = L"#DIV/0!"; break;
						case 0x0F: m_oValue->m_sText = L"#VALUE!"; break;
						case 0x17: m_oValue->m_sText = L"#REF!"; break;
						case 0x1D: m_oValue->m_sText = L"#NAME?"; break;
						case 0x24: m_oValue->m_sText = L"#NUM!"; break;
						case 0x2A: m_oValue->m_sText = L"#N/A"; break;
						case 0x2B: m_oValue->m_sText = L"#GETTING_DATA"; break;
						}
					}
					break;
					case XLSB::rt_ExternCellString:
					{
						auto pSt = reinterpret_cast<XLSB::ExternCellString*>(pSource)->value.value();
						m_oType.Init();
                        m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeStr);
						m_oValue.Init();
						m_oValue->m_sText = pSt;
					}
					break;
					}
				}

			}

		}
	}
	void CExternalCell::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, (L"r"), m_oRef)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"t"), m_oType)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"vm"), m_oValueMetadata)
		WritingElement_ReadAttributes_End(oReader)
	}

	CExternalRow::CExternalRow()
	{
	}
	CExternalRow::~CExternalRow()
	{
	}
	void CExternalRow::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CExternalRow::getType() const
	{
		return et_x_ExternalRow;
	}
	void CExternalRow::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();
			if (L"cell" == sName)
			{
				CExternalCell* pExternalCell = new CExternalCell();
				*pExternalCell = oReader;
				m_arrItems.push_back(pExternalCell);
			}
		}
	}
	void CExternalRow::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<row");
		WritingStringNullableAttrInt(L"r", m_oR, m_oR->GetValue());
		writer.WriteString(L">");

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}

		writer.WriteString(L"</row>");
	}
	std::wstring CExternalRow::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	XLS::BaseObjectPtr CExternalRow::toBin()
	{
		auto ptr(new XLSB::EXTERNROW);
		XLS::BaseObjectPtr objectPtr(ptr);
		auto ptr1(new XLSB::ExternRowHdr);
		ptr->m_BrtExternRowHdr = XLS::BaseObjectPtr{ptr1};

		if(m_oR.IsInit() && m_oR->GetValue() > 0)
		{
			ptr1->rw = m_oR->GetValue() - 1;
		}
		else
			ptr1->rw = 0;

		for(auto i:m_arrItems)
		{
			ptr->m_arEXTERNVALUE.push_back(i->toBin());
		}

		return objectPtr;
	}
	void CExternalRow::fromBin(XLS::BaseObjectPtr& obj)
	{
		ReadAttributes(obj);
	}
	void CExternalRow::ReadAttributes(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::EXTERNROW*>(obj.get());
		if (ptr != nullptr)
		{
			auto ptrExternRowHdr = static_cast<XLSB::ExternRowHdr*>(ptr->m_BrtExternRowHdr.get());
			if (ptrExternRowHdr != nullptr)
			{
				m_oR = ptrExternRowHdr->rw + 1;
			}

			for (auto &item : ptr->m_arEXTERNVALUE)
				m_arrItems.push_back(new CExternalCell(item));
		}
	}
	void CExternalRow::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_ReadSingle(oReader, (L"r"), m_oR)
			WritingElement_ReadAttributes_End(oReader)
	}

	CExternalSheetData::CExternalSheetData()
	{
	}
	CExternalSheetData::~CExternalSheetData()
	{
	}
	void CExternalSheetData::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CExternalSheetData::getType() const
	{
		return et_x_ExternalSheetData;
	}
	void CExternalSheetData::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();
			if (L"row" == sName)
			{
				CExternalRow* pExternalRow = new CExternalRow();
				*pExternalRow = oReader;
				m_arrItems.push_back(pExternalRow);
			}
		}
	}
	void CExternalSheetData::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<sheetData");
		WritingStringNullableAttrInt(L"sheetId", m_oSheetId, m_oSheetId->GetValue());
		WritingStringNullableAttrBool(L"refreshError", m_oRefreshError);
		writer.WriteString(L">");

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}

		writer.WriteString(L"</sheetData>");
	}
	std::wstring CExternalSheetData::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	XLS::BaseObjectPtr CExternalSheetData::toBin()
	{
		auto ptr(new XLSB::EXTERNTABLE);
		XLS::BaseObjectPtr objectPtr(ptr);
		auto ptr1(new XLSB::ExternTableStart);
		ptr->m_BrtExternTableStart = XLS::BaseObjectPtr{ptr1};

		if(m_oSheetId.IsInit())
			ptr1->iTab = m_oSheetId->GetValue();
		else
			ptr1->iTab = 0;
		if(m_oRefreshError.IsInit())
			ptr1->fRefreshError = m_oRefreshError->GetValue();
		else
			ptr1->fRefreshError = false;
		
		for(auto i:m_arrItems)
		{
			ptr->m_arEXTERNROW.push_back(i->toBin());
		}

		return objectPtr;
	}
	void CExternalSheetData::fromBin(XLS::BaseObjectPtr& obj)
	{
		ReadAttributes(obj);
	}
	void CExternalSheetData::ReadAttributes(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::EXTERNTABLE*>(obj.get());
		if (ptr != nullptr)
		{
			auto ptrExternTableStart = static_cast<XLSB::ExternTableStart*>(ptr->m_BrtExternTableStart.get());
			if (ptrExternTableStart != nullptr)
			{
				m_oSheetId = ptrExternTableStart->iTab;
				m_oRefreshError = ptrExternTableStart->fRefreshError;
			}

			for (auto &item : ptr->m_arEXTERNROW)
				m_arrItems.push_back(new CExternalRow(item));
		}
	}
	void CExternalSheetData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, (L"sheetId"), m_oSheetId)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"refreshError"), m_oRefreshError)
			WritingElement_ReadAttributes_End(oReader)
	}

	CExternalSheetDataSet::CExternalSheetDataSet()
	{
	}
	CExternalSheetDataSet::~CExternalSheetDataSet()
	{
	}
	void CExternalSheetDataSet::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CExternalSheetDataSet::getType() const
	{
		return et_x_ExternalSheetDataSet;
	}
	void CExternalSheetDataSet::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();
			if (L"sheetData" == sName)
			{
				CExternalSheetData* pExternalSheetData = new CExternalSheetData();
				*pExternalSheetData = oReader;
				m_arrItems.push_back(pExternalSheetData);
			}
		}
	}
	void CExternalSheetDataSet::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<sheetDataSet>");
		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}
		writer.WriteString(L"</sheetDataSet>");
	}
	std::wstring CExternalSheetDataSet::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	void CExternalSheetDataSet::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
	{
		for (auto &item : obj)
		{
			m_arrItems.push_back(new CExternalSheetData(item));
		}
	}
	std::vector<XLS::BaseObjectPtr> CExternalSheetDataSet::toBin()
	{
		std::vector<XLS::BaseObjectPtr> objectVector;
		for(auto i:m_arrItems)
		{
			objectVector.push_back(i->toBin());
		}
		return objectVector;
	}
	CExternalBook::CExternalBook()
	{
	}
	CExternalBook::~CExternalBook()
	{
	}
	void CExternalBook::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CExternalBook::getType() const
	{
		return et_x_ExternalBook;
	}
	void CExternalBook::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetNameNoNS();

			if (L"sheetNames" == sName)
			{
				m_oSheetNames = oReader;
			}
			else if (L"definedNames" == sName)
			{
				m_oDefinedNames = oReader;
			}
			else if (L"sheetDataSet" == sName)
			{
				m_oSheetDataSet = oReader;
			}
			else if (L"alternateUrls" == sName)
			{
				m_oAlternateUrls = oReader;
			}
		}
	}
	void CExternalBook::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<externalBook");
		if (m_oRid.IsInit())
		{
			writer.WriteString(L" r:id=\"");
			writer.WriteString(m_oRid->ToString());
			writer.WriteString(L"\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"");
		}
		writer.WriteString(L">");
		if (m_oAlternateUrls.IsInit())
		{
			m_oAlternateUrls ->toXML(writer);
		}
		if (m_oSheetNames.IsInit())
		{
			m_oSheetNames->toXML(writer);
		}
		if (m_oDefinedNames.IsInit())
		{
			m_oDefinedNames->toXML(writer);
		}
		if (m_oSheetDataSet.IsInit())
		{
			m_oSheetDataSet->toXML(writer);
		}
		writer.WriteString(L"</externalBook>");
	}
	std::wstring CExternalBook::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	XLS::BaseObjectPtr CExternalBook::toBin()
	{
		XLSB::EXTERNALLINKPtr externalLINK(new XLSB::EXTERNALLINK());
		auto beginSupbook(new XLSB::BeginSupBook);
		if(m_oRid.IsInit())
			beginSupbook->string1 = m_oRid->GetValue();
		else
			beginSupbook->string1 = L" ";
		beginSupbook->sbt = 0;
		externalLINK->m_BrtBeginSupBook = XLS::BaseObjectPtr{beginSupbook};
		XLSB::ExternalReferenceType type;
		type = XLSB::ExternalReferenceType::WORKBOOK;
		XLSB::EXTERNALBOOKPtr externalBOOK(new XLSB::EXTERNALBOOK(type));

		externalLINK->m_EXTERNALBOOK = externalBOOK;
		if (externalBOOK != nullptr)
		{
			if (m_oSheetNames.IsInit())
				externalBOOK->m_BrtSupTabs = m_oSheetNames->toBin();
			if(m_oDefinedNames.IsInit())
				externalBOOK->m_arEXTERNNAME = m_oDefinedNames->toBin();
			if(m_oSheetDataSet.IsInit())
				externalBOOK->m_arEXTERNTABLE = m_oSheetDataSet->toBin();
		}
		return externalLINK;
	}
	void CExternalBook::fromBin(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::EXTERNALLINK*>(obj.get());
		if (ptr != nullptr)
		{
			ReadAttributes(ptr->m_BrtBeginSupBook);

			auto ptr1 = static_cast<XLSB::EXTERNALBOOK*>(ptr->m_EXTERNALBOOK.get());
			if (ptr1 != nullptr)
			{
				if (ptr1->m_BrtSupTabs != nullptr)
					m_oSheetNames = ptr1->m_BrtSupTabs;
				if (!ptr1->m_arEXTERNNAME.empty())
					m_oDefinedNames = ptr1->m_arEXTERNNAME;
				if (!ptr1->m_arEXTERNTABLE.empty())
					m_oSheetDataSet = ptr1->m_arEXTERNTABLE;
			}
		}
	}
	void CExternalBook::ReadAttributes(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::BeginSupBook*>(obj.get());
		if (ptr != nullptr)
		{
			if (!ptr->string1.empty())
				m_oRid = ptr->string1;
		}
	}
	void CExternalBook::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS(oReader)
			WritingElement_ReadAttributes_ReadSingle(oReader, L"id", m_oRid)
			WritingElement_ReadAttributes_End_No_NS(oReader)
	}

	CDdeValue::CDdeValue()
	{
	}
	CDdeValue::~CDdeValue()
	{
	}
	void CDdeValue::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CDdeValue::getType() const
	{
		return et_x_DdeValue;
	}
	void CDdeValue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"val" == sName)
			{
				CText* pText = new CText();
				*pText = oReader;
				m_arrItems.push_back(pText);
			}
		}
	}
	void CDdeValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<value");
		WritingStringNullableAttrString(L"t", m_oType, m_oType->ToString());
		writer.WriteString(L">");

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML2(writer, L"val");
			}
		}
		writer.WriteString(L"</value>");
	}
	std::wstring CDdeValue::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	XLS::BaseObjectPtr CDdeValue::toBin()
	{
		XLSB::DDEOLEITEMVALUEPtr ptr(new XLSB::DDEOLEITEMVALUE);
		if(!m_oType.IsInit())
			return ptr;
		auto type = m_oType->GetValue();
		switch(type)
		{
			case XLSB::rt_SupNameNum:
				{
					ptr->m_source = XLS::BaseObjectPtr{new XLSB::SupNameNum};
					if(!m_arrItems.empty())
					{
						auto castedSource = reinterpret_cast<XLSB::SupNameNum*>(ptr->m_source.get());
						castedSource->value.data.value = std::stod(m_arrItems.back()->m_sText);
						m_arrItems.pop_back();
					}
				}
				break;
				case XLSB::rt_SupNameBool:
				{
                    ptr->m_source = XLS::BaseObjectPtr{new XLSB::SupNameBool};
					if(!m_arrItems.empty())
					{
						auto castedSource = reinterpret_cast<XLSB::SupNameBool*>(ptr->m_source.get());
                        castedSource->value = std::stoi(m_arrItems.back()->m_sText);
						m_arrItems.pop_back();
					}

				}
				break;
				case XLSB::rt_SupNameErr:
				{
                    ptr->m_source = XLS::BaseObjectPtr{new XLSB::SupNameErr};
					if(!m_arrItems.empty())
					{
						auto castedSource = reinterpret_cast<XLSB::SupNameErr*>(ptr->m_source.get());
                        auto error = m_arrItems.back()->m_sText.c_str();

						if(error == L"#NULL!"){ castedSource->value = 0x00;}
						else if(error == L"#DIV/0!"){castedSource->value = 0x07;}
						else if(error == L"#VALUE!"){ castedSource->value = 0x0F;}
						else if(error == L"#REF!"){ castedSource->value = 0x17;}
						else if(error == L"#NAME?"){ castedSource->value = 0x1D;}
						else if(error == L"#NUM!"){castedSource->value = 0x24;}
						else if(error == L"#N/A"){castedSource->value = 0x2A;}
						else if(error == L"#GETTING_DATA"){castedSource->value = 0x2B;}

						m_arrItems.pop_back();
					}
				}
				break;
				case XLSB::rt_SupNameSt:
				{
					ptr->m_source = XLS::BaseObjectPtr{new XLSB::SupNameSt};
					if(!m_arrItems.empty())
					{
						auto castedSource = reinterpret_cast<XLSB::SupNameSt*>(ptr->m_source.get());
						castedSource->value = m_arrItems.back()->m_sText;
						m_arrItems.pop_back();
					}
				}
				break;
				case XLSB::rt_SupNameNil:
				{
					ptr->m_source = XLS::BaseObjectPtr{new XLSB::SupNameNil};
				}
				break;
		}
		return ptr;
	}
	void CDdeValue::fromBin(XLS::BaseObjectPtr& obj)
	{
		ReadAttributes(obj);
	}
	void CDdeValue::ReadAttributes(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::DDEOLEITEMVALUE*>(obj.get());
		if (ptr != nullptr)
		{

			if (ptr->m_source != nullptr)
			{
				auto pSource = static_cast<XLS::BiffRecord*>(ptr->m_source.get());
				auto nType = pSource->getTypeId();
				switch (nType)
				{
				case XLSB::rt_SupNameNum:
				{
					auto pRealNum = reinterpret_cast<XLSB::SupNameNum*>(pSource)->value.data.value;
					m_oType.Init();
					m_oType->SetValue(SimpleTypes::Spreadsheet::EDdeValueType::ddevaluetypeN);

					auto text = new CText();
					std::wstring val = OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(pRealNum);
					text->fromBin(val);
					m_arrItems.push_back(text);
				}
				break;
				case XLSB::rt_SupNameBool:
				{
					auto pBool = reinterpret_cast<XLSB::SupNameBool*>(pSource)->value;
					m_oType.Init();
					m_oType->SetValue(SimpleTypes::Spreadsheet::EDdeValueType::ddevaluetypeB);

					auto text = new CText();
					std::wstring val = pBool ? L"1" : L"0";
					text->fromBin(val);
					m_arrItems.push_back(text);
				}
				break;
				case XLSB::rt_SupNameErr:
				{
					auto pError = reinterpret_cast<XLSB::SupNameErr*>(pSource)->value;
					m_oType.Init();
					m_oType->SetValue(SimpleTypes::Spreadsheet::EDdeValueType::ddevaluetypeE);

					auto text = new CText();
					std::wstring val;
					switch (pError)
					{
					case 0x00: val = L"#NULL!"; break;
					case 0x07: val = L"#DIV/0!"; break;
					case 0x0F: val = L"#VALUE!"; break;
					case 0x17: val = L"#REF!"; break;
					case 0x1D: val = L"#NAME?"; break;
					case 0x24: val = L"#NUM!"; break;
					case 0x2A: val = L"#N/A"; break;
					case 0x2B: val = L"#GETTING_DATA"; break;
					}
					text->fromBin(val);
					m_arrItems.push_back(text);
				}
				break;
				case XLSB::rt_SupNameSt:
				{
					auto pSt = reinterpret_cast<XLSB::SupNameSt*>(pSource)->value.value();
					m_oType.Init();
					m_oType->SetValue(SimpleTypes::Spreadsheet::EDdeValueType::ddevaluetypeStr);

					auto text = new CText();
					text->fromBin(pSt);
					m_arrItems.push_back(text);
				}
				break;
				case XLSB::rt_SupNameNil:
				{
					m_oType.Init();
					m_oType->SetValue(SimpleTypes::Spreadsheet::EDdeValueType::ddevaluetypeNil);;
				}
				break;
				}
			}
		}
	}
	void CDdeValue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_ReadSingle(oReader, (L"t"), m_oType)
			WritingElement_ReadAttributes_End(oReader)
	}

	CDdeValues::CDdeValues()
	{
	}
	CDdeValues::~CDdeValues()
	{
	}
	void CDdeValues::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CDdeValues::getType() const
	{
		return et_x_DdeValues;
	}
	void CDdeValues::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"value" == sName)
			{
				CDdeValue* pDdeValue = new CDdeValue();
				*pDdeValue = oReader;
				m_arrItems.push_back(pDdeValue);
			}
		}
	}
	void CDdeValues::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<values");
		WritingStringNullableAttrInt(L"rows", m_oRows, m_oRows->GetValue());
		WritingStringNullableAttrInt(L"cols", m_oCols, m_oCols->GetValue());
		writer.WriteString(L">");

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}
		writer.WriteString(L"</values>");
	}
	std::wstring CDdeValues::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	void CDdeValues::fromBin(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::DDEOLEITEMVALUES*>(obj.get());
		if (ptr != nullptr)
		{
			ReadAttributes(ptr->m_BrtSupNameValueStart);

			for (auto &item : ptr->m_arDDEOLEITEMVALUE)
				m_arrItems.push_back(new CDdeValue(item));
		}
	}
	XLS::BaseObjectPtr CDdeValues::toBin()
	{
		XLSB::DDEOLEITEMVALUESPtr ptr(new XLSB::DDEOLEITEMVALUES);
		auto castedPtr = static_cast<XLSB::SupNameValueStart*>(ptr->m_BrtSupNameValueStart.get());
		if(m_oRows.IsInit())
			castedPtr->cRw = m_oRows->GetValue();
		if(m_oCols.IsInit())
		castedPtr->cCol = m_oCols->GetValue();
		for (auto &item : m_arrItems)
				ptr->m_arDDEOLEITEMVALUE.push_back(item->toBin());
       return ptr;
	}
	void CDdeValues::ReadAttributes(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::SupNameValueStart*>(obj.get());
		if (ptr != nullptr)
		{
			m_oRows = ptr->cRw;
			m_oCols = ptr->cCol;
		}
	}
	void CDdeValues::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, (L"rows"), m_oRows)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"cols"), m_oCols)
			WritingElement_ReadAttributes_End(oReader)
	}

	CDdeItem::CDdeItem()
	{
	}
	CDdeItem::~CDdeItem()
	{
	}
	void CDdeItem::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CDdeItem::getType() const
	{
		return et_x_OleItem;
	}
	void CDdeItem::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"values" == sName)
			{
				m_oDdeValues = oReader;
			}
		}
	}
	void CDdeItem::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<ddeItem");
		WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
		WritingStringNullableAttrBool(L"ole", m_oOle);
		WritingStringNullableAttrBool(L"advise", m_oAdvise);
		WritingStringNullableAttrBool(L"preferPic", m_oPreferPic);
		writer.WriteString(L">");
		if (m_oDdeValues.IsInit())
		{
			m_oDdeValues->toXML(writer);
		}
		writer.WriteString(L"</ddeItem>");
	}
	std::wstring CDdeItem::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	XLS::BaseObjectPtr CDdeItem::toBin()
	{
		XLSB::ExternalReferenceType type;
		XLSB::DDEOLEITEMPtr ptr(new XLSB::DDEOLEITEM(type));
		if (m_oDdeValues.IsInit())
				ptr->m_DDEOLEITEMVALUES = m_oDdeValues->toBin();
		if(m_oName.IsInit())
		{
			ptr->m_BrtSupNameStart = XLS::BaseObjectPtr{new XLSB::SupNameStart};
			auto ptrSupNameStart = static_cast<XLSB::SupNameStart*>(ptr->m_BrtSupNameStart.get());
			ptrSupNameStart->name = m_oName.get();
		}
		if(m_oOle.IsInit() || m_oAdvise.IsInit() || m_oPreferPic.IsInit())
		{
			ptr->m_BrtSupNameBits = XLS::BaseObjectPtr{new XLSB::SupNameBits(type)};
			auto ptrSupNameBits = static_cast<XLSB::SupNameBits*>(ptr->m_BrtSupNameBits.get());
			ptrSupNameBits->sbt = 0x0001;
			if(m_oOle.IsInit())
			{
				ptrSupNameBits->contentsDDE.fOLE = m_oOle->ToBool();
			}
			if(m_oAdvise.IsInit())
			{
				ptrSupNameBits->contentsDDE.fWantAdvise  = m_oAdvise->ToBool();
			}
			if(m_oPreferPic.IsInit())
			{
				ptrSupNameBits->contentsDDE.fWantPict = m_oPreferPic->ToBool();
			}
		}
		return ptr;
	}
	void CDdeItem::fromBin(XLS::BaseObjectPtr& obj)
	{
		ReadAttributes(obj);
	}
	void CDdeItem::ReadAttributes(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::DDEOLEITEM*>(obj.get());
		if (ptr != nullptr)
		{
			if (ptr->m_DDEOLEITEMVALUES != nullptr)
				m_oDdeValues = ptr->m_DDEOLEITEMVALUES;

			auto ptrSupNameStart = static_cast<XLSB::SupNameStart*>(ptr->m_BrtSupNameStart.get());
			if (ptrSupNameStart != nullptr)
			{
				if (!ptrSupNameStart->name.value().empty())
					m_oName = ptrSupNameStart->name.value();
			}

			auto ptrSupNameBits = static_cast<XLSB::SupNameBits*>(ptr->m_BrtSupNameBits.get());
			if (ptrSupNameBits != nullptr)
			{
				m_oOle = ptrSupNameBits->contentsDDE.fOLE;
				m_oAdvise = ptrSupNameBits->contentsDDE.fWantAdvise;
				m_oPreferPic = ptrSupNameBits->contentsDDE.fWantPict;
			}
		}
	}
	void CDdeItem::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, (L"name"), m_oName)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"ole"), m_oOle)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"advise"), m_oAdvise)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"preferPic"), m_oPreferPic)
			WritingElement_ReadAttributes_End(oReader)
	}

	CDdeItems::CDdeItems()
	{
	}
	CDdeItems::~CDdeItems()
	{
	}
	void CDdeItems::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CDdeItems::getType() const
	{
		return et_x_DdeItems;
	}
	void CDdeItems::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"ddeItem" == sName)
			{
				CDdeItem* pDdeItem = new CDdeItem();
				*pDdeItem = oReader;
				m_arrItems.push_back(pDdeItem);
			}
		}
	}
	void CDdeItems::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<ddeItems>");
		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}
		writer.WriteString(L"</ddeItems>");
	}
	std::wstring CDdeItems::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	void CDdeItems::fromBin(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::DDEOLELINK*>(obj.get());
		if (ptr != nullptr)
		{
			for (auto &item : ptr->m_arDDEOLEITEM)
				m_arrItems.push_back(new CDdeItem(item));
		}
	}

	XLS::BaseObjectPtr CDdeItems::toBin()
	{
		XLSB::ExternalReferenceType type;
		XLSB::DDEOLELINKPtr ptr(new XLSB::DDEOLELINK(type));
		for (auto item : m_arrItems)
				ptr->m_arDDEOLEITEM.push_back(item->toBin());
		return ptr;
	}

	void CDdeLink::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"ddeItems" == sName)
			{
				m_oDdeItems = oReader;
			}
		}
	}
	void CDdeLink::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<ddeLink xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"");
		WritingStringNullableAttrEncodeXmlString(L"ddeService", m_oDdeService, m_oDdeService.get());
		WritingStringNullableAttrEncodeXmlString(L"ddeTopic", m_oDdeTopic, m_oDdeTopic.get());
		writer.WriteString(L">");
		if (m_oDdeItems.IsInit())
		{
			m_oDdeItems->toXML(writer);
		}
		writer.WriteString(L"</ddeLink>");
	}
	std::wstring CDdeLink::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	void CDdeLink::fromBin(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::EXTERNALLINK*>(obj.get());
		if (ptr != nullptr)
		{
			ReadAttributes(ptr->m_BrtBeginSupBook);

			if (ptr->m_DDEOLELINK != nullptr)
				m_oDdeItems = ptr->m_DDEOLELINK;
		}
	}
	XLS::BaseObjectPtr CDdeLink::toBin()
	{
		XLSB::EXTERNALLINKPtr ptr(new XLSB::EXTERNALLINK);
		ptr->m_BrtBeginSupBook = XLS::BaseObjectPtr{new XLSB::BeginSupBook};
		auto castedBook = static_cast<XLSB::BeginSupBook*>(ptr->m_BrtBeginSupBook.get());
		if (m_oDdeService.IsInit())
			castedBook->string1 = m_oDdeService.get();
		else
			castedBook->string1 = L"";
		if (m_oDdeTopic.IsInit())
			castedBook->string2 = m_oDdeTopic.get();
		else
			castedBook->string2 = L"";
		if(m_oDdeItems.IsInit())
			ptr->m_DDEOLELINK = m_oDdeItems->toBin();
		castedBook->sbt = 0x0001;
        return ptr;
	}

	CDdeLink::CDdeLink()
	{
	}
	CDdeLink::~CDdeLink()
	{
	}
	void CDdeLink::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CDdeLink::getType() const
	{
		return et_x_DdeLink;
	}
	void CDdeLink::ReadAttributes(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::BeginSupBook*>(obj.get());
		if (ptr != nullptr)
		{
			if (!ptr->string1.empty())
				m_oDdeService = ptr->string1;
			if (!ptr->string2.empty())
				m_oDdeTopic = ptr->string2;
		}
	}
	void CDdeLink::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, (L"ddeService"), m_oDdeService)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"ddeTopic"), m_oDdeTopic)
			WritingElement_ReadAttributes_End(oReader)
	}

	COleItem::COleItem()
	{
	}
	COleItem::~COleItem()
	{
	}
	void COleItem::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType COleItem::getType() const
	{
		return et_x_OleItem;
	}
	void COleItem::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void COleItem::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<oleItem");
		WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
		WritingStringNullableAttrBool(L"icon", m_oIcon);
		WritingStringNullableAttrBool(L"advise", m_oAdvise);
		WritingStringNullableAttrBool(L"preferPic", m_oPreferPic);
		writer.WriteString(L"/>");
	}
	std::wstring COleItem::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	XLS::BaseObjectPtr COleItem::toBin()
	{
		XLSB::ExternalReferenceType type;
		XLSB::DDEOLEITEMPtr coleItem(new XLSB::DDEOLEITEM(type));
		if(m_oName.IsInit())
		{
			coleItem->m_BrtSupNameStart = XLS::BaseObjectPtr{new XLSB::SupNameStart};
			auto castedName = static_cast<XLSB::SupNameStart*>(coleItem->m_BrtSupNameStart.get());
			castedName->name = m_oName.get();
		}
		if(m_oIcon.IsInit() || m_oAdvise.IsInit() || m_oPreferPic.IsInit())
		{
			coleItem->m_BrtSupNameBits = XLS::BaseObjectPtr{new XLSB::SupNameBits(type)};
			auto ptrSupNameBits = static_cast<XLSB::SupNameBits*>(coleItem->m_BrtSupNameBits.get());
			ptrSupNameBits->sbt = 0x0002;
			if(m_oIcon.IsInit())
				ptrSupNameBits->contentsOLE.fIcon = m_oIcon->ToBool();
			if(m_oAdvise.IsInit())
				ptrSupNameBits->contentsOLE.fWantAdvise = m_oAdvise->ToBool();
			if(m_oPreferPic.IsInit())
				ptrSupNameBits->contentsOLE.fWantPict = m_oPreferPic->ToBool();
		}
		return coleItem;
	}
	void COleItem::fromBin(XLS::BaseObjectPtr& obj)
	{
		ReadAttributes(obj);
	}
	void COleItem::ReadAttributes(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::DDEOLEITEM*>(obj.get());
		if (ptr != nullptr)
		{
			auto ptrSupNameStart = static_cast<XLSB::SupNameStart*>(ptr->m_BrtSupNameStart.get());
			if (ptrSupNameStart != nullptr)
			{
				if (!ptrSupNameStart->name.value().empty())
					m_oName = ptrSupNameStart->name.value();
			}

			auto ptrSupNameBits = static_cast<XLSB::SupNameBits*>(ptr->m_BrtSupNameBits.get());
			if (ptrSupNameBits != nullptr)
			{
				m_oIcon = ptrSupNameBits->contentsOLE.fIcon;
				m_oAdvise = ptrSupNameBits->contentsOLE.fWantAdvise;
				m_oPreferPic = ptrSupNameBits->contentsOLE.fWantPict;
			}
		}
	}
	void COleItem::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, (L"name"), m_oName)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"icon"), m_oIcon)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"advise"), m_oAdvise)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"preferPic"), m_oPreferPic)
			WritingElement_ReadAttributes_End(oReader)
	}

	COleItems::COleItems()
	{
	}
	COleItems::~COleItems()
	{
	}
	void COleItems::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType COleItems::getType() const
	{
		return et_x_OleItems;
	}
	void COleItems::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"oleItem" == sName)
			{
				COleItem* pOleItem = new COleItem();
				*pOleItem = oReader;
				m_arrItems.push_back(pOleItem);
			}
		}
	}
	void COleItems::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<oleItems>");
		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
			{
				m_arrItems[i]->toXML(writer);
			}
		}
		writer.WriteString(L"</oleItems>");
	}
	std::wstring COleItems::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	void COleItems::fromBin(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::DDEOLELINK*>(obj.get());
		if (ptr != nullptr)
		{
			for (auto &item : ptr->m_arDDEOLEITEM)
				m_arrItems.push_back(new COleItem(item));
		}
	}
	XLS::BaseObjectPtr COleItems::toBin()
	{
		XLSB::ExternalReferenceType type;
		XLSB::DDEOLELINKPtr ptr(new XLSB::DDEOLELINK(type));

		for(auto i: m_arrItems)
		{
			ptr->m_arDDEOLEITEM.push_back(i->toBin());
		}
		return ptr;
	}

	COleLink::COleLink()
	{
	}
	COleLink::~COleLink()
	{
	}
	void COleLink::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType COleLink::getType() const
	{
		return et_x_OleLink;
	}
	void COleLink::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"oleItems" == sName)
			{
				m_oOleItems = oReader;
			}
		}
	}
	void COleLink::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<oleLink xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"");
		WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
		WritingStringNullableAttrEncodeXmlString(L"progId", m_oProgId, m_oProgId.get());
		writer.WriteString(L">");
		if (m_oOleItems.IsInit())
		{
			m_oOleItems->toXML(writer);
		}
		writer.WriteString(L"</oleLink>");
	}
	std::wstring COleLink::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
	void COleLink::fromBin(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::EXTERNALLINK*>(obj.get());
		if (ptr != nullptr)
		{
			ReadAttributes(ptr->m_BrtBeginSupBook);

			if (ptr->m_DDEOLELINK != nullptr)
				m_oOleItems = ptr->m_DDEOLELINK;
		}
	}
	XLS::BaseObjectPtr COleLink::toBin()
	{
		XLSB::EXTERNALLINKPtr COleLink(new XLSB::EXTERNALLINK);
		COleLink->m_BrtBeginSupBook = XLS::BaseObjectPtr{new XLSB::BeginSupBook};
		auto castedBook = static_cast<XLSB::BeginSupBook*>(COleLink->m_BrtBeginSupBook.get());
		castedBook->sbt = 0x0002;
		if(m_oRid.IsInit())
			castedBook->string1 = m_oRid->ToString();
        else
            castedBook->string1 = L"";
		if(m_oProgId.IsInit())
			castedBook->string2 = m_oProgId.get();
        else
            castedBook->string2 = L"";

        if(m_oOleItems.IsInit())
            COleLink->m_DDEOLELINK = m_oOleItems->toBin();
		return COleLink;
	}
	void COleLink::ReadAttributes(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::BeginSupBook*>(obj.get());
		if (ptr != nullptr)
		{
			if (!ptr->string1.empty())
				m_oRid = ptr->string1;
			if (!ptr->string2.empty())
				m_oProgId = ptr->string2;
		}
	}
	void COleLink::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, (L"r:id"), m_oRid)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"relationships:id"), m_oRid)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"progId"), m_oProgId)
		WritingElement_ReadAttributes_End(oReader)
	}
	CAlternateUrls::CAlternateUrls()
	{
	}
	CAlternateUrls::~CAlternateUrls()
	{
	}
	void CAlternateUrls::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	EElementType CAlternateUrls::getType() const
	{
		return et_x_ExternalDefinedName;
	}
	void CAlternateUrls::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetNameNoNS();

			if (L"absoluteUrl" == sName)
			{
				ReadAttributes(oReader, m_oAbsoluteUrlRid);
			}
			else if (L"relativeUrl" == sName)
			{
				ReadAttributes(oReader, m_oRelativeUrlRid);
			}
		}
	}
	void CAlternateUrls::ReadAttributes(XmlUtils::CXmlLiteReader& oReader, nullable<SimpleTypes::CRelationshipId>& rid)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_ReadSingle(oReader, (L"r:id"), rid)
		WritingElement_ReadAttributes_End(oReader)
	}
	void CAlternateUrls::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, (L"driveId"), m_oDriveId)
			WritingElement_ReadAttributes_Read_else_if(oReader, (L"itemId"), m_oItemId)
		WritingElement_ReadAttributes_End(oReader)
	}
	void CAlternateUrls::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<xxl21:alternateUrls");
		WritingStringNullableAttrEncodeXmlString2(L"driveId", m_oDriveId);
		WritingStringNullableAttrEncodeXmlString2(L"itemId", m_oItemId);
		writer.WriteString(L">");

		if (m_oAbsoluteUrlRid.IsInit())
		{
			writer.WriteString(L"<xxl21:absoluteUrl r:id=\"" + m_oAbsoluteUrlRid->ToString() + L"\"/>");
		}
		else if (m_oRelativeUrlRid.IsInit())
		{
			writer.WriteString(L"<xxl21:relativeUrl r:id=\"" + m_oRelativeUrlRid->ToString() + L"\"/>");
		}
		writer.WriteString(L"</xxl21:alternateUrls>");
	}
	std::wstring CAlternateUrls::toXML() const
	{
		NSStringUtils::CStringBuilder writer;
		toXML(writer);
		return writer.GetData().c_str();
	}
//---------------------------------------------------------------------------------------------------------------------------------
	CExternalLink::CExternalLink(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
	{
		m_bSpreadsheets = true;
	}
	CExternalLink::CExternalLink(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath, const std::wstring & rId) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
	{
		m_bSpreadsheets = true;
		m_rId = rId;
		read(oRootPath, oPath);
	}
	CExternalLink::~CExternalLink()
	{
	}
    XLS::BaseObjectPtr CExternalLink::writeBin() const
	{
		XLSB::ExternalLinkStreamPtr externalLinkStreamStream(new XLSB::ExternalLinkStream);

		if (externalLinkStreamStream != nullptr)
		{
			if (m_oExternalBook.IsInit())
				externalLinkStreamStream->m_EXTERNALLINK = m_oExternalBook->toBin();
			else if (m_oDdeLink.IsInit())
				externalLinkStreamStream->m_EXTERNALLINK = m_oDdeLink->toBin();
			else if (m_oOleLink.IsInit())
				externalLinkStreamStream->m_EXTERNALLINK = m_oOleLink->toBin();
		}
		return externalLinkStreamStream;
	}
	void CExternalLink::readBin(const CPath& oPath)
	{
		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
		if (xlsb)
		{
			XLSB::ExternalLinkStreamPtr externalLinkStreamStream(new XLSB::ExternalLinkStream);

			xlsb->ReadBin(oPath, externalLinkStreamStream.get());

			externalLinkStreamStream->UpdateXti(xlsb->GetGlobalinfo(), m_rId);

			if (externalLinkStreamStream != nullptr)
			{
				if (externalLinkStreamStream->m_EXTERNALLINK != nullptr)
				{
					auto ptr = static_cast<XLSB::EXTERNALLINK*>(externalLinkStreamStream->m_EXTERNALLINK.get());
					if (ptr != nullptr)
					{
						auto ptr1 = static_cast<XLSB::BeginSupBook*>(ptr->m_BrtBeginSupBook.get());
						if (ptr1 != nullptr)
						{
							switch (ptr1->sbt.value().get())
							{
							case XLSB::ExternalReferenceType::WORKBOOK:
								m_oExternalBook = externalLinkStreamStream->m_EXTERNALLINK;
								break;
							case XLSB::ExternalReferenceType::DDE:
								m_oDdeLink = externalLinkStreamStream->m_EXTERNALLINK;
								break;
							case XLSB::ExternalReferenceType::OLE:
								m_oOleLink = externalLinkStreamStream->m_EXTERNALLINK;
								break;
							}
						}
					}
				}
			}

			//externalLinkStreamStream.reset();
		}
	}
	void CExternalLink::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	void CExternalLink::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read(oRootPath, oPath);

		if (m_oReadPath.GetExtention() == _T(".bin"))
		{
			readBin(m_oReadPath);
			return;
		}

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oPath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
		if (L"externalLink" == sName)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
			{
				int nStylesDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nStylesDepth))
				{
					sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if (L"externalBook" == sName)
					{
						m_oExternalBook = oReader;
					}
					else if (L"oleLink" == sName)
					{
						m_oOleLink = oReader;
					}
					else if (L"ddeLink" == sName)
					{
						m_oDdeLink = oReader;
					}
					else if (L"extLst" == sName)
					{
						nullable<OOX::Drawing::COfficeArtExtensionList>	oExtLst;
						oExtLst = oReader;

						for (size_t i = 0; (oExtLst.IsInit()) && (i < oExtLst->m_arrExt.size()); i++)
						{
							if (oExtLst->m_arrExt[i]->m_oFileKey.IsInit() ||
								oExtLst->m_arrExt[i]->m_oInstanceId.IsInit())
							{
								m_oFileKey = oExtLst->m_arrExt[i]->m_oFileKey;
								m_oInstanceId = oExtLst->m_arrExt[i]->m_oInstanceId;
								break;
							}
						}
					}
				}
			}
		}
	}
	void CExternalLink::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
		if ((xlsb) && (xlsb->m_bWriteToXlsb))
		{
			XLS::BaseObjectPtr object = writeBin();
			xlsb->WriteBin(oPath, object.get());
		}
		else
		{
		NSStringUtils::CStringBuilder sXml;
		sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		sXml.WriteString(L"<externalLink \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"x14 xxl21\" \
xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\" \
xmlns:xxl21=\"http://schemas.microsoft.com/office/spreadsheetml/2021/extlinks2021\">");

		if (m_oExternalBook.IsInit())
		{
			m_oExternalBook->toXML(sXml);
		}
		if (m_oOleLink.IsInit())
		{
			m_oOleLink->toXML(sXml);
		}
		if (m_oDdeLink.IsInit())
		{
			m_oDdeLink->toXML(sXml);
		}
		if (m_oFileKey.IsInit() || m_oInstanceId.IsInit())
		{
			OOX::Drawing::COfficeArtExtension oExt;
			oExt.m_oFileKey = m_oFileKey;
			oExt.m_oInstanceId = m_oInstanceId;

			oExt.m_sUri = L"{C3750BE0-5CA9-4D1C-82C7-79D762991C26}";

			sXml.WriteString(L"<extLst>" + oExt.toXMLWithNS(L"") + L"</extLst>");
		}
		sXml.WriteString(L"</externalLink>");

		std::wstring sPath = oPath.GetPath();
		NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());
		}
		oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
		IFileContainer::Write(oPath, oDirectory, oContent);
	}
	bool CExternalLink::isValid() const
	{
		return true;
	}
	const OOX::FileType CExternalLink::type() const
	{
		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
		if ((xlsb) && (xlsb->m_bWriteToXlsb))
		{
			return OOX::SpreadsheetBin::FileTypes::ExternalLinksBin;
		}
		return OOX::Spreadsheet::FileTypes::ExternalLinks;
	}
	const CPath CExternalLink::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CExternalLink::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	const CPath& CExternalLink::GetReadPath()
	{
		return m_oReadPath;
	}
	void CExternalLink::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
	}
}
}
