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

#include "WorksheetChildOther.h"

#include "../../XlsbFormat/Biff12_records/WsDim.h"
#include "../../XlsbFormat/Biff12_records/WsFmtInfo.h"
#include "../../XlsbFormat/Biff12_unions/WSVIEWS2.h"
#include "../../XlsbFormat/Biff12_unions/WSVIEW2.h"
#include "../../XlsbFormat/Biff12_unions/HEADERFOOTER.h"
#include "../../XlsbFormat/Biff12_records/BeginHeaderFooter.h"
#include "../../XlsbFormat/Biff12_records/SheetProtectionIso.h"
#include "../../XlsbFormat/Biff12_records/SheetProtection.h"
#include "../../XlsbFormat/Biff12_records/LegacyDrawingHF.h"
#include "../../XlsbFormat/Biff12_records/Margins.h"
#include "../../XlsbFormat/Biff12_records/PrintOptions.h"
#include "../../XlsbFormat/Biff12_records/WsProp.h"
#include "../../XlsbFormat/Biff12_records/BkHim.h"
#include "../../XlsbFormat/Biff12_unions/RWBRK.h"
#include "../../XlsbFormat/Biff12_unions/COLBRK.h"
#include "../../XlsbFormat/Biff12_records/BeginRwBrk.h"
#include "../../XlsbFormat/Biff12_records/BeginColBrk.h"
#include "../../XlsbFormat/Biff12_records/Brk.h"
#include "../../XlsbFormat/Biff12_records/RangeProtectionIso.h"
#include "../../XlsbFormat/Biff12_records/RangeProtection.h"
#include "../../XlsbFormat/Biff12_unions/DCON.h"
#include "../../XlsbFormat/Biff12_records/BeginDCon.h"
#include "../../XlsbFormat/Biff12_unions/DREFS.h"
#include "../../XlsbFormat/Biff12_records/BeginDRefs.h"
#include "../../XlsbFormat/Biff12_records/DRef.h"
#include "../../XlsbFormat/Biff12_unions/CSVIEWS.h"
#include "../../XlsbFormat/Biff12_unions/CSVIEW.h"
#include "../../XlsbFormat/Biff12_records/CsProp.h"
#include "../../XlsbFormat/Biff12_records/CsProtectionIso.h"
#include "../../XlsbFormat/Biff12_records/CsProtection.h"

#include "../../XlsbFormat/Biff12_records/CommonRecords.h"
#include "../Styles/rPr.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

#include "../../Common/SimpleTypes_Word.h"
#include "../../../MsBinaryFile/XlsFile/Format/Binary/CFStreamCacheWriter.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CProtectedRange::CProtectedRange()
		{
		}
		CProtectedRange::~CProtectedRange()
		{
		}
		void CProtectedRange::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CProtectedRange::toXML() const
		{
			return L"";
		}
		void CProtectedRange::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<protectedRange");
			WritingStringNullableAttrString(L"name", m_oName, m_oName.get());
			WritingStringNullableAttrString(L"sqref", m_oSqref, m_oSqref.get());
			WritingStringNullableAttrString(L"algorithmName", m_oAlgorithmName, m_oAlgorithmName->ToString());
			WritingStringNullableAttrString(L"hashValue", m_oHashValue, m_oHashValue.get());
			WritingStringNullableAttrString(L"saltValue", m_oSaltValue, m_oSaltValue.get());
			WritingStringNullableAttrInt(L"spinCount", m_oSpinCount, m_oSpinCount->GetValue());

			if (m_arSecurityDescriptors.size() == 1)
			{
				WritingStringAttrString(L"securityDescriptor", XmlUtils::EncodeXmlString(m_arSecurityDescriptors[0]));
			}
			if (m_arSecurityDescriptors.size() > 1)
			{
				writer.WriteString(L">");
				for (size_t i = 0; i < m_arSecurityDescriptors.size(); ++i)
				{
					writer.WriteString(L"<securityDescriptor>");
					writer.WriteString(XmlUtils::EncodeXmlString(m_arSecurityDescriptors[i]));
					writer.WriteString(L"</securityDescriptor>");
				}
				writer.WriteString(L"</protectedRange>");
			}
			else
			{
				writer.WriteString(L"/>");
			}
		}
		void CProtectedRange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"securityDescriptor" == sName)
					m_arSecurityDescriptors.push_back(oReader.GetText2());
			}

		}
		XLS::BaseObjectPtr CProtectedRange::toBin()
		{
			XLS::BaseObjectPtr objectPtr;
            if(m_oSpinCount.IsInit() || m_oSaltValue.IsInit())
			{
				auto ptr(new XLSB::RangeProtectionIso);
				objectPtr = XLS::BaseObjectPtr{ptr};
				if(m_oSpinCount.IsInit())
					ptr->dwSpinCount = m_oSpinCount->GetValue();
				if(m_oSqref.IsInit())
					ptr->sqRfX.strValue = m_oSqref.get();

				if (m_oAlgorithmName.IsInit())
					ptr->ipdPasswordData.szAlgName = m_oAlgorithmName->GetValue();

				if (m_oName.IsInit())
					ptr->rangeProtectionTitleSDRel.rgchTitle = m_oName.get();

				BYTE * temp;
				auto tempSize = 0;
				NSFile::CBase64Converter::CBase64Converter::Decode(std::string{m_oHashValue.get().begin(),
					m_oHashValue.get().end()}.c_str(), m_oHashValue.get().size(), temp, tempSize);
				ptr->ipdPasswordData.rgbHash.cbLength = tempSize;
				ptr->ipdPasswordData.rgbHash.rgbData = std::vector<BYTE>(temp, temp + tempSize);
				delete[] temp; 

				NSFile::CBase64Converter::Decode(std::string{m_oSaltValue.get().begin(),
					m_oSaltValue.get().end()}.c_str(), m_oSaltValue.get().size(), temp, tempSize);
				ptr->ipdPasswordData.rgbSalt.cbLength = tempSize;
				ptr->ipdPasswordData.rgbSalt.rgbData = std::vector<BYTE>(temp, temp + tempSize);
				delete[] temp; 
			}
			else
			{
				auto ptr(new XLSB::RangeProtection);
				objectPtr = XLS::BaseObjectPtr{ptr};
				if(m_oSqref.IsInit())
					ptr->sqRfX.strValue = m_oSqref.get();

				if (m_oName.IsInit())
					ptr->rangeProtectionTitleSDRel.rgchTitle = m_oName.get();

			}
			return objectPtr;
		}
        void CProtectedRange::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            XLS::CFRecordPtr rangeProtRecord;
            std::vector<BYTE> dataBuffer;
            if(m_oSpinCount.IsInit() || m_oSaltValue.IsInit())
            {
                rangeProtRecord = writer->getNextRecord(XLSB::rt_RangeProtectionIso);
                _UINT32 dwSpinCount = 0;
                if(m_oSpinCount.IsInit())
                    dwSpinCount = m_oSpinCount->GetValue();
                *rangeProtRecord << dwSpinCount;
            }
            else
            {
                rangeProtRecord = writer->getNextRecord(XLSB::rt_RangeProtection);
                _UINT16 protpwd = 0;
                *rangeProtRecord << protpwd;
            }
            {
                XLSB::UncheckedSqRfX ref;
                if(m_oSqref.IsInit())
                        ref.strValue = m_oSqref.get();
                *rangeProtRecord << ref;
            }
            {
                XLSB::RangeProtectionTitleSDRel rangeProtectionTitleSDRel;
                if (m_oName.IsInit())
                    rangeProtectionTitleSDRel.rgchTitle = m_oName.get();
                *rangeProtRecord << rangeProtectionTitleSDRel;
            }
            if(rangeProtRecord->getTypeId() ==  XLSB::rt_RangeProtectionIso)
            {
                dataBuffer.reserve(rangeProtRecord->getDataSize() - 4);
                std::memcpy(dataBuffer.data(), rangeProtRecord->getData() + 4, rangeProtRecord->getDataSize() - 4);
                XLSB::IsoPasswordData ipdPasswordData;
                BYTE * temp;
                auto tempSize = 0;
                NSFile::CBase64Converter::CBase64Converter::Decode(std::string{m_oHashValue.get().begin(),
                    m_oHashValue.get().end()}.c_str(), m_oHashValue.get().size(), temp, tempSize);
                ipdPasswordData.rgbHash.cbLength = tempSize;
                ipdPasswordData.rgbHash.rgbData = std::vector<BYTE>(temp, temp + tempSize);
                delete[] temp;

                NSFile::CBase64Converter::Decode(std::string{m_oSaltValue.get().begin(),
                    m_oSaltValue.get().end()}.c_str(), m_oSaltValue.get().size(), temp, tempSize);
                ipdPasswordData.rgbSalt.cbLength = tempSize;
                ipdPasswordData.rgbSalt.rgbData = std::vector<BYTE>(temp, temp + tempSize);
                delete[] temp;
            }
            writer->storeNextRecord(rangeProtRecord);
            if(rangeProtRecord->getTypeId() ==  XLSB::rt_RangeProtectionIso)
            {
                rangeProtRecord.reset();
                rangeProtRecord = writer->getNextRecord(XLSB::rt_RangeProtection);
                _UINT16 protpwd = 0;
                *rangeProtRecord << protpwd;
                rangeProtRecord->appendRawDataToStatic(dataBuffer.data(), dataBuffer.size());
                writer->storeNextRecord(rangeProtRecord);
            }
        }
		void CProtectedRange::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CProtectedRange::getType() const
		{
			return et_x_ProtectedRange;
		}
		void CProtectedRange::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typeRangeProtectionIso)
			{
				auto ptr = static_cast<XLSB::RangeProtectionIso*>(obj.get());
				if (ptr != nullptr)
				{
					m_oSpinCount = ptr->dwSpinCount;
					m_oSqref = ptr->sqRfX.strValue;

					if (!ptr->ipdPasswordData.szAlgName.value().empty())
						m_oAlgorithmName = ptr->ipdPasswordData.szAlgName.value();

					if (!ptr->rangeProtectionTitleSDRel.rgchTitle.value().empty())
						m_oName = ptr->rangeProtectionTitleSDRel.rgchTitle.value();

					m_oHashValue = ptr->ipdPasswordData.rgbHash.GetBase64();
					m_oSaltValue = ptr->ipdPasswordData.rgbSalt.GetBase64();
				}
			}

			else if (obj->get_type() == XLS::typeRangeProtection)
			{
				auto ptr = static_cast<XLSB::RangeProtection*>(obj.get());
				if (ptr != nullptr)
				{
					m_oSqref = ptr->sqRfX.strValue;

					if (!ptr->rangeProtectionTitleSDRel.rgchTitle.value().empty())
						m_oName = ptr->rangeProtectionTitleSDRel.rgchTitle.value();
				}
			}
		}
		void CProtectedRange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			nullable_string desc;
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"algorithmName"), m_oAlgorithmName)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"hashValue"), m_oHashValue)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"saltValue"), m_oSaltValue)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"spinCount"), m_oSpinCount)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"name"), m_oName)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"sqref"), m_oSqref)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"securityDescriptor"), desc)
				WritingElement_ReadAttributes_End(oReader)

				if (desc.IsInit())
					m_arSecurityDescriptors.push_back(*desc);
		}

		CProtectedRanges::CProtectedRanges()
		{
		}
		CProtectedRanges::~CProtectedRanges()
		{
		}
		void CProtectedRanges::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CProtectedRanges::toXML() const
		{
			return (L"");
		}
		void CProtectedRanges::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString((L"<protectedRanges>"));

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString((L"</protectedRanges>"));
		}
		void CProtectedRanges::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"protectedRange" == sName)
				{
					CProtectedRange* pProtectedRange = new CProtectedRange();
					*pProtectedRange = oReader;
					m_arrItems.push_back(pProtectedRange);
				}
			}
		}
		std::vector<XLS::BaseObjectPtr> CProtectedRanges::toBin()
		{
			std::vector<XLS::BaseObjectPtr> result;
			for(auto i:m_arrItems)
            {
                result.push_back(i->toBin());
            }
			return result;
		}
        void CProtectedRanges::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            for(auto i:m_arrItems)
                i->toBin(writer);
        }
		void CProtectedRanges::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			for (auto &protRange : obj)
				m_arrItems.push_back(new CProtectedRange(protRange));
		}
		EElementType CProtectedRanges::getType() const
		{
			return et_x_ProtectedRanges;
		}

		CCellWatch::CCellWatch()
		{
		}
		CCellWatch::~CCellWatch()
		{
		}
		void CCellWatch::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CCellWatch::toXML() const
		{
			return L"";
		}
		void CCellWatch::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<cellWatch");
			WritingStringNullableAttrString(L"r", m_oR, m_oR.get());
			writer.WriteString(L"/>");
		}
		void CCellWatch::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
		}
		void CCellWatch::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CCellWatch::getType() const
		{
			return et_x_CellWatch;
		}
		void CCellWatch::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
		}
		void CCellWatch::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"r"), m_oR)
				WritingElement_ReadAttributes_End(oReader)
		}

		CCellWatches::CCellWatches()
		{
		}
		CCellWatches::~CCellWatches()
		{
		}
		void CCellWatches::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CCellWatches::toXML() const
		{
			return (L"");
		}
		void CCellWatches::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString((L"<cellWatches>"));

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString((L"</cellWatches>"));
		}
		void CCellWatches::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"cellWatch" == sName)
				{
					CCellWatch* pCellWatch = new CCellWatch();
					*pCellWatch = oReader;
					m_arrItems.push_back(pCellWatch);
				}
			}
		}
		void CCellWatches::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
		}
		EElementType CCellWatches::getType() const
		{
			return et_x_CellWatches;
		}

		CPageMargins::CPageMargins()
		{
		}
		CPageMargins::~CPageMargins()
		{
		}
		void CPageMargins::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CPageMargins::toXML() const
		{
			return (L"");
		}
		void CPageMargins::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_oLeft.IsInit() || m_oTop.IsInit() || m_oRight.IsInit() || m_oBottom.IsInit() || m_oHeader.IsInit() || m_oFooter.IsInit())
			{
				writer.WriteString(L"<pageMargins");
				WritingStringNullableAttrDouble(L"left", m_oLeft, m_oLeft->ToInches());
				WritingStringNullableAttrDouble(L"right", m_oRight, m_oRight->ToInches());
				WritingStringNullableAttrDouble(L"top", m_oTop, m_oTop->ToInches());
				WritingStringNullableAttrDouble(L"bottom", m_oBottom, m_oBottom->ToInches());
				WritingStringNullableAttrDouble(L"header", m_oHeader, m_oHeader->ToInches());
				WritingStringNullableAttrDouble(L"footer", m_oFooter, m_oFooter->ToInches());
				writer.WriteString(L"/>");
			}
		}
		void CPageMargins::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CPageMargins::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CPageMargins::toBin()
		{
			auto ptr(new XLSB::Margins);
			XLS::BaseObjectPtr objPtr(ptr);

			if(m_oLeft.IsInit())
                ptr->xnumLeft.data.value = std::round(m_oLeft->GetValue()) / 100;
			else
				ptr->xnumLeft.data.value = 0;

			if(m_oTop.IsInit())
                ptr->xnumTop.data.value = std::round(m_oTop->GetValue()) / 100;
			else
				ptr->xnumTop.data.value = 0;
			if(m_oRight.IsInit())
                ptr->xnumRight.data.value = std::round(m_oRight->GetValue()) / 100;
			else
				ptr->xnumRight.data.value = 0;
			if(m_oBottom.IsInit())
                ptr->xnumBottom.data.value = std::round(m_oBottom->GetValue()) / 100;
			else
				ptr->xnumBottom.data.value = 0;
			if(m_oHeader.IsInit())
                ptr->xnumHeader.data.value = std::round(m_oHeader->GetValue()) / 100;
			else
				ptr->xnumHeader.data.value = 0;
			if(m_oFooter.IsInit())
                ptr->xnumFooter.data.value = std::round(m_oFooter->GetValue()) / 100;
			else
				ptr->xnumFooter.data.value = 0;
			return objPtr;
		}
        void CPageMargins::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_Margins);
            XLS::Xnum marginData;
            if(m_oLeft.IsInit())
                marginData.data.value = std::round(m_oLeft->GetValue()) / 100;
            else
                marginData.data.value = 0;
            *record << marginData;
            if(m_oRight.IsInit())
                marginData.data.value = std::round(m_oRight->GetValue()) / 100;
            else
                marginData.data.value = 0;
            *record << marginData;
            if(m_oTop.IsInit())
                marginData.data.value = std::round(m_oTop->GetValue()) / 100;
            else
                marginData.data.value = 0;
            *record << marginData;
            if(m_oBottom.IsInit())
                marginData.data.value = std::round(m_oBottom->GetValue()) / 100;
            else
                marginData.data.value = 0;
            *record << marginData;
            if(m_oHeader.IsInit())
                marginData.data.value = std::round(m_oHeader->GetValue()) / 100;
            else
                marginData.data.value = 0;
            *record << marginData;
            if(m_oFooter.IsInit())
                marginData.data.value = std::round(m_oFooter->GetValue()) / 100;
            else
                marginData.data.value = 0;
            *record << marginData;
            writer->storeNextRecord(record);
        }
		EElementType CPageMargins::getType() const
		{
			return et_x_PageMargins;
		}
		void CPageMargins::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"left"), m_oLeft)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"top"), m_oTop)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"right"), m_oRight)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"bottom"), m_oBottom)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"header"), m_oHeader)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"footer"), m_oFooter)
				WritingElement_ReadAttributes_End(oReader)
		}
		void CPageMargins::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::Margins*>(obj.get());
			if (ptr != nullptr)
			{
				m_oLeft.Init();
				m_oLeft->SetValue(ptr->xnumLeft.data.value);

				m_oTop.Init();
				m_oTop->SetValue(ptr->xnumTop.data.value);

				m_oRight.Init();
				m_oRight->SetValue(ptr->xnumRight.data.value);

				m_oBottom.Init();
				m_oBottom->SetValue(ptr->xnumBottom.data.value);

				m_oHeader.Init();
				m_oHeader->SetValue(ptr->xnumHeader.data.value);

				m_oFooter.Init();
				m_oFooter->SetValue(ptr->xnumFooter.data.value);
			}
		}

		CPageSetup::CPageSetup()
		{
		}
		CPageSetup::~CPageSetup()
		{
		}
		void CPageSetup::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CPageSetup::toXML() const
		{
			return (L"");
		}
		void CPageSetup::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_oPaperSize.IsInit() || m_oOrientation.IsInit())
			{
				writer.WriteString(L"<pageSetup");
				WritingStringNullableAttrString(L"paperSize", m_oPaperSize, m_oPaperSize->ToString());
				WritingStringNullableAttrDouble(L"paperHeight", m_oPaperHeight, m_oPaperHeight->GetValue());
				WritingStringNullableAttrDouble(L"paperWidth", m_oPaperWidth, m_oPaperWidth->GetValue());
				WritingStringNullableAttrUInt(L"scale", m_oScale, m_oScale->GetValue());
				WritingStringNullableAttrUInt(L"firstPageNumber", m_oFirstPageNumber, m_oFirstPageNumber->GetValue());
				WritingStringNullableAttrInt(L"fitToWidth", m_oFitToWidth, m_oFitToWidth->GetValue());
				WritingStringNullableAttrInt(L"fitToHeight", m_oFitToHeight, m_oFitToHeight->GetValue());
				WritingStringNullableAttrString(L"pageOrder", m_oPageOrder, m_oPageOrder->ToString());
				WritingStringNullableAttrString(L"orientation", m_oOrientation, m_oOrientation->ToString());
				WritingStringNullableAttrBool(L"usePrinterDefaults", m_oUsePrinterDefaults);
				WritingStringNullableAttrBool(L"blackAndWhite", m_oBlackAndWhite);
				WritingStringNullableAttrBool(L"draft", m_oDraft);
				WritingStringNullableAttrString(L"cellComments", m_oCellComments, m_oCellComments->ToString());
				WritingStringNullableAttrBool(L"useFirstPageNumber", m_oUseFirstPageNumber);
				WritingStringNullableAttrString(L"errors", m_oErrors, m_oErrors->ToString());
				WritingStringNullableAttrUInt(L"horizontalDpi", m_oHorizontalDpi, m_oHorizontalDpi->GetValue());
				WritingStringNullableAttrUInt(L"verticalDpi", m_oVerticalDpi, m_oVerticalDpi->GetValue());
				WritingStringNullableAttrUInt(L"copies", m_oCopies, m_oCopies->GetValue());
				WritingStringNullableAttrString(L"paperUnits", m_oPaperUnits, m_oPaperUnits->ToString());
				WritingStringNullableAttrString(L"r:id", m_oRId, m_oRId->ToString());
				writer.WriteString(L"/>");
			}
		}
		void CPageSetup::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CPageSetup::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CPageSetup::toBin()
		{
			auto ptr(new XLSB::PageSetup);
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_oBlackAndWhite.IsInit())
				ptr->fNoColor  = m_oBlackAndWhite->m_eValue;
			else
				ptr->fNoColor = false;
			if (ptr->fNoColor && m_oCellComments.IsInit())
			{
				if (m_oCellComments == SimpleTypes::Spreadsheet::ECellComments::cellcommentsAtEnd)
					ptr->fNotes = true;
				else if(m_oCellComments == SimpleTypes::Spreadsheet::ECellComments::cellcommentsAsDisplayed)
					ptr->fNotes = false;
			}

			if (m_oCopies.IsInit())
				ptr->iCopies = m_oCopies->m_eValue;
			else
				ptr->iCopies = 1;

			if (m_oDraft.IsInit())
				ptr->fDraft = m_oDraft->m_eValue;
			else
				ptr->fDraft = false;

			if (m_oErrors.IsInit())
				ptr->iErrors = m_oErrors->m_eValue;
			else
				ptr->iErrors = 0;

			if (m_oFirstPageNumber.IsInit())
				ptr->iPageStart = m_oFirstPageNumber->m_eValue;
			else
				ptr->iPageStart = 1;

			if (m_oFitToHeight.IsInit())
				ptr->iFitHeight = m_oFitToHeight->m_eValue;
			else
				ptr->iFitHeight = 1;

			if (m_oFitToWidth.IsInit())
				ptr->iFitWidth = m_oFitToWidth->m_eValue;
			else
				ptr->iFitWidth = 1;

			if (m_oHorizontalDpi.IsInit())
				ptr->iRes = m_oHorizontalDpi->m_eValue;
			else
				ptr->iRes = 600;

			if (m_oRId.IsInit())
				ptr->szRelID = m_oRId->GetValue();

            if (m_oOrientation.IsInit() && m_oOrientation->GetValue() == SimpleTypes::EPageOrientation::pageorientLandscape)
            {
                ptr->fPortrait = false;
				ptr->fLandscape = true;
            }
            else if(m_oOrientation.IsInit())
            {
				ptr->fLandscape = false;
                ptr->fPortrait = true;
            }
            else
            {
                ptr->fPortrait = false;
                ptr->fLandscape = false;
                ptr->fNoOrient = true;
            }

            if (m_oPageOrder.IsInit() && m_oPageOrder->GetValue() == SimpleTypes::Spreadsheet::EPageOrder::pageorderOverThenDown)
				ptr->fLeftToRight = true;
			else
				ptr->fLeftToRight = false;

			if (m_oPaperSize.IsInit())
				ptr->iPaperSize = m_oPaperSize->m_eValue;
			else
				ptr->iPaperSize = 9;

			if (m_oScale.IsInit())
				ptr->iScale = m_oScale->m_eValue;
			else
				ptr->iScale = 100;

			if (m_oUseFirstPageNumber.IsInit())
				ptr->fUsePage = m_oUseFirstPageNumber->m_eValue;
			else
				ptr->fUsePage = true;

			if (m_oVerticalDpi.IsInit())
				ptr->iVRes = m_oVerticalDpi->m_eValue;
			else
				ptr->iVRes = 0;
			ptr->fEndNotes = false;
			return objectPtr;
		}
        void CPageSetup::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_PageSetup);
            {
                _UINT32 uintBuf = 9;
                if (m_oPaperSize.IsInit())
                    uintBuf = m_oPaperSize->m_eValue;
                *record << uintBuf;
                if (m_oScale.IsInit())
                    uintBuf = m_oScale->m_eValue;
                else
                    uintBuf = 100;
                *record << uintBuf;
                if (m_oHorizontalDpi.IsInit())
                    uintBuf = m_oHorizontalDpi->m_eValue;
                else
                    uintBuf = 600;
                *record << uintBuf;
                if (m_oVerticalDpi.IsInit())
                    uintBuf = m_oVerticalDpi->m_eValue;
                else
                    uintBuf = 0;
                *record << uintBuf;
                if (m_oCopies.IsInit())
                    uintBuf = m_oCopies->m_eValue;
                else
                    uintBuf = 1;
                *record << uintBuf;
                _INT32 iPageStart = 1;
                if (m_oFirstPageNumber.IsInit())
                    iPageStart = m_oFirstPageNumber->m_eValue;
                *record << iPageStart;
                if (m_oFitToWidth.IsInit())
                    uintBuf = m_oFitToWidth->m_eValue;
                else
                    uintBuf = 1;
                *record << uintBuf;
                if (m_oFitToHeight.IsInit())
                    uintBuf = m_oFitToHeight->m_eValue;
                else
                    uintBuf = 1;
                *record << uintBuf;
            }
            {
                _UINT16 flags = 0;
                if (m_oPageOrder.IsInit() && m_oPageOrder->GetValue() == SimpleTypes::Spreadsheet::EPageOrder::pageorderOverThenDown)
                    SETBIT(flags, 0, 1)
                if (m_oOrientation.IsInit() && m_oOrientation->GetValue() == SimpleTypes::EPageOrientation::pageorientLandscape)
                {
                    SETBIT(flags, 1, 1)
                }
                else if(!m_oOrientation.IsInit())
                {
                    SETBIT(flags, 6, true)
                }
                if(m_oBlackAndWhite.IsInit())
                {
                    SETBIT(flags, 3, m_oBlackAndWhite->m_eValue)
                    if (m_oBlackAndWhite->m_eValue && m_oCellComments.IsInit())
                        if (m_oCellComments == SimpleTypes::Spreadsheet::ECellComments::cellcommentsAtEnd)
                            SETBIT(flags, 5, 1)
                }
                if (m_oDraft.IsInit())
                    SETBIT(flags, 4, m_oDraft->m_eValue)
                if (m_oUseFirstPageNumber.IsInit())
                    SETBIT(flags, 7, m_oUseFirstPageNumber->m_eValue)
                else
                    SETBIT(flags, 7, 1)
                if (m_oErrors.IsInit())
                    SETBITS(flags, 9, 10, m_oErrors->m_eValue)
                *record << flags;
            }
            XLSB::XLNullableWideString szRelID;
            if (m_oRId.IsInit())
                szRelID = m_oRId->GetValue();
            else
                szRelID.setSize(0xFFFFFFFF);
             *record << szRelID;
            writer->storeNextRecord(record);
        }
		XLS::BaseObjectPtr CPageSetup::toBinCs()
		{
			auto ptr(new XLSB::CsPageSetup);
			XLS::BaseObjectPtr objectPtr(ptr);

			if(m_oBlackAndWhite.IsInit())
				ptr->fNoColor  = m_oBlackAndWhite->m_eValue;
			else
				ptr->fNoColor = false;
			if (ptr->fNoColor)
			{
				if (m_oCellComments == SimpleTypes::Spreadsheet::ECellComments::cellcommentsAtEnd)
					ptr->fNotes = true;
				else if(m_oCellComments == SimpleTypes::Spreadsheet::ECellComments::cellcommentsAsDisplayed)
					ptr->fNotes = false;
			}
			ptr->fEndNotes = false;
			if (m_oCopies.IsInit())
				ptr->iCopies = m_oCopies->m_eValue;
			else
				ptr->iCopies = 0;

			if (m_oDraft.IsInit())
				ptr->fDraft = m_oDraft->m_eValue;
			else
				ptr->fDraft = 0;
			if (m_oFirstPageNumber.IsInit())
				ptr->iPageStart = m_oFirstPageNumber->m_eValue;
			else
				ptr->iPageStart = 0;
			if (m_oHorizontalDpi.IsInit())
				ptr->iRes = m_oHorizontalDpi->m_eValue;
			else
				ptr->iRes = 0;
			if (m_oRId.IsInit())
				ptr->szRelID = m_oRId->GetValue();

			if (m_oOrientation.IsInit())
			{
				if (m_oOrientation == SimpleTypes::EPageOrientation::pageorientLandscape)
					ptr->fLandscape = true;
				else
					ptr->fLandscape = false;
			}

			if (m_oPaperSize.IsInit())
				ptr->iPaperSize = m_oPaperSize->m_eValue;
			else
				ptr->iPaperSize = 9;

			if (m_oScale.IsInit())
				ptr->iScale = m_oScale->m_eValue;
			else
				ptr->iScale = 100;

			if (m_oUseFirstPageNumber.IsInit())
				ptr->fUsePage = m_oUseFirstPageNumber->m_eValue;
			else
				ptr->fUsePage = false;
			if (m_oVerticalDpi.IsInit())
				ptr->iVRes = m_oVerticalDpi->m_eValue;
			else
				ptr->iVRes = 0;
			return objectPtr;
		}

		EElementType CPageSetup::getType() const
		{
			return et_x_PageSetup;
		}
		void CPageSetup::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"blackAndWhite"), m_oBlackAndWhite)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"cellComments"), m_oCellComments)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"copies"), m_oCopies)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"draft"), m_oDraft)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"errors"), m_oErrors)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"firstPageNumber"), m_oFirstPageNumber)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"fitToHeight"), m_oFitToHeight)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"fitToWidth"), m_oFitToWidth)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"horizontalDpi"), m_oHorizontalDpi)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"r:id"), m_oRId)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"relationships:id"), m_oRId)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"orientation"), m_oOrientation)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"pageOrder"), m_oPageOrder)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"paperHeight"), m_oPaperHeight)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"paperSize"), m_oPaperSize)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"paperWidth"), m_oPaperWidth)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"paperUnits"), m_oPaperUnits)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"scale"), m_oScale)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"useFirstPageNumber"), m_oUseFirstPageNumber)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"usePrinterDefaults"), m_oUsePrinterDefaults)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"verticalDpi"), m_oVerticalDpi)
				WritingElement_ReadAttributes_End(oReader)
		}
		void CPageSetup::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typePageSetup)
			{
				auto ptr = static_cast<XLSB::PageSetup*>(obj.get());
				if (ptr != nullptr)
				{
					m_oBlackAndWhite = ptr->fNoColor;
					if (ptr->fNoColor)
					{
						if (ptr->fNotes)
							m_oCellComments = SimpleTypes::Spreadsheet::ECellComments::cellcommentsAtEnd;
						else
							m_oCellComments = SimpleTypes::Spreadsheet::ECellComments::cellcommentsAsDisplayed;
					}
					else
						m_oCellComments = SimpleTypes::Spreadsheet::ECellComments::cellcommentsNone;

					m_oCopies = ptr->iCopies;
					m_oDraft = ptr->fDraft;
					m_oErrors = (SimpleTypes::Spreadsheet::EPrintError)ptr->iErrors;
					m_oFirstPageNumber = ptr->iPageStart;
					m_oFitToHeight = ptr->iFitHeight;
					m_oFitToWidth = ptr->iFitWidth;
					m_oHorizontalDpi = ptr->iRes;
					m_oRId = ptr->szRelID;

					if (ptr->fLandscape)
						m_oOrientation = SimpleTypes::EPageOrientation::pageorientLandscape;
					else
						m_oOrientation = SimpleTypes::EPageOrientation::pageorientPortrait;

					if (ptr->fLeftToRight)
						m_oPageOrder = SimpleTypes::Spreadsheet::EPageOrder::pageorderOverThenDown;
					else
						m_oPageOrder = SimpleTypes::Spreadsheet::EPageOrder::pageorderDownThenOver;

					m_oPaperSize = (SimpleTypes::Spreadsheet::EPageSize)ptr->iPaperSize;
					m_oScale = ptr->iScale;
					m_oUseFirstPageNumber = ptr->fUsePage;
					m_oVerticalDpi = ptr->iVRes;

				}
			}
			else if (obj->get_type() == XLS::typeCsPageSetup)
			{
				auto ptr = static_cast<XLSB::CsPageSetup*>(obj.get());
				if (ptr != nullptr)
				{
					m_oBlackAndWhite = ptr->fNoColor;
					if (ptr->fNoColor)
					{
						if (ptr->fNotes)
							m_oCellComments = SimpleTypes::Spreadsheet::ECellComments::cellcommentsAtEnd;
						else
							m_oCellComments = SimpleTypes::Spreadsheet::ECellComments::cellcommentsAsDisplayed;
					}
					else
						m_oCellComments = SimpleTypes::Spreadsheet::ECellComments::cellcommentsNone;

					m_oCopies = ptr->iCopies;
					m_oDraft = ptr->fDraft;
					m_oFirstPageNumber = ptr->iPageStart;
					m_oHorizontalDpi = ptr->iRes;
					m_oRId = ptr->szRelID;

					if (ptr->fLandscape)
						m_oOrientation = SimpleTypes::EPageOrientation::pageorientLandscape;
					else
						m_oOrientation = SimpleTypes::EPageOrientation::pageorientPortrait;

					m_oPaperSize = (SimpleTypes::Spreadsheet::EPageSize)ptr->iPaperSize;
					m_oUseFirstPageNumber = ptr->fUsePage;
					m_oVerticalDpi = ptr->iVRes;
				}
			}
		}

		CPrintOptions::CPrintOptions()
		{
		}
		CPrintOptions::~CPrintOptions()
		{
		}
		void CPrintOptions::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CPrintOptions::toXML() const
		{
			return (L"");
		}
		void CPrintOptions::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_oGridLines.IsInit() || m_oGridLinesSet.IsInit() || m_oHeadings.IsInit() || m_oHorizontalCentered.IsInit() || m_oVerticalCentered.IsInit())
			{
				writer.WriteString(L"<printOptions");
				WritingStringNullableAttrBool(L"headings", m_oHeadings);
				WritingStringNullableAttrBool(L"gridLines", m_oGridLines);
				WritingStringNullableAttrBool(L"gridLinesSet", m_oGridLinesSet);
				WritingStringNullableAttrBool(L"horizontalCentered", m_oHorizontalCentered);
				WritingStringNullableAttrBool(L"verticalCentered", m_oVerticalCentered);
				writer.WriteString(L"/>");
			}
		}
		void CPrintOptions::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CPrintOptions::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CPrintOptions::toBin()
		{
			auto ptr(new XLSB::PrintOptions);
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_oGridLines.IsInit())
				ptr->fPrintGrid = m_oGridLines->m_eValue;
			else
				ptr->fPrintGrid = false;
			if(m_oGridLinesSet.IsInit())
				ptr->fPrintGrid = m_oGridLinesSet->m_eValue;
			if(m_oHeadings.IsInit())
				ptr->fPrintHeaders = m_oHeadings->m_eValue;
			else
				ptr->fPrintHeaders = false;
			if(m_oHorizontalCentered.IsInit())
				ptr->fHCenter = m_oHorizontalCentered->m_eValue;
			else
				ptr->fHCenter = false;
			if(m_oVerticalCentered.IsInit())
				ptr->fVCenter = m_oVerticalCentered->m_eValue;
			else
				ptr->fVCenter = false;
			return objectPtr;
		}
        void CPrintOptions::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_PrintOptions);
            _UINT16 flags = 0;
            if(m_oHorizontalCentered.IsInit())
                SETBIT(flags, 0, m_oHorizontalCentered->GetValue())
            if(m_oVerticalCentered.IsInit())
                SETBIT(flags, 1, m_oVerticalCentered->GetValue())
            if(m_oHeadings.IsInit())
                SETBIT(flags, 2, m_oHeadings->GetValue())
            if(m_oGridLines.IsInit())
                SETBIT(flags, 3, m_oGridLines->GetValue())
            *record << flags;
            writer->storeNextRecord(record);
        }
		EElementType CPrintOptions::getType() const
		{
			return et_x_PrintOptions;
		}
		void CPrintOptions::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"gridLines"), m_oGridLines)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"gridLinesSet"), m_oGridLinesSet)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"headings"), m_oHeadings)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"horizontalCentered"), m_oHorizontalCentered)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"verticalCentered"), m_oVerticalCentered)
			WritingElement_ReadAttributes_End(oReader)
		}
		void CPrintOptions::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::PrintOptions*>(obj.get());
			if (ptr != nullptr)
			{
				m_oGridLines = ptr->fPrintGrid;
				m_oGridLinesSet = ptr->fPrintGrid;
				m_oHeadings = ptr->fPrintHeaders;
				m_oHorizontalCentered = ptr->fHCenter;
				m_oVerticalCentered = ptr->fVCenter;
			}
		}

		CDimension::CDimension()
		{
		}
		CDimension::~CDimension()
		{
		}
		void CDimension::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDimension::toXML() const
		{
			return (L"");
		}
		void CDimension::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CDimension::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CDimension::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CDimension::toBin()
		{
			auto castedPtr(new XLSB::WsDim);
			XLS::BaseObjectPtr ptr(castedPtr);
			if (m_oRef.IsInit())
				castedPtr->rfx = m_oRef.get();
			return ptr;
		}
        void CDimension::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_WsDim);
            XLSB::UncheckedRfX ref;
            if(m_oRef.IsInit())
                ref = m_oRef.get();
            *record << ref;
            writer->storeNextRecord(record);
        }
		EElementType CDimension::getType() const
		{
			return et_x_Dimension;
		}
		void CDimension::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_ReadSingle(oReader, (L"ref"), m_oRef)
				WritingElement_ReadAttributes_End(oReader)
		}
		void CDimension::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::WsDim*>(obj.get());
			if (ptr != nullptr)
				m_oRef = ptr->rfx.toString(true, true);
		}

		CSheetFormatPr::CSheetFormatPr()
		{
		}
		CSheetFormatPr::~CSheetFormatPr()
		{
		}
		void CSheetFormatPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSheetFormatPr::toXML() const
		{
			return (L"");
		}
		void CSheetFormatPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<sheetFormatPr"));
			WritingStringNullableAttrInt2(L"baseColWidth", m_oBaseColWidth);
			WritingStringNullableAttrDouble2(L"defaultColWidth", m_oDefaultColWidth);
			WritingStringNullableAttrDouble2(L"defaultRowHeight", m_oDefaultRowHeight);
			WritingStringNullableAttrBool2(L"customHeight", m_oCustomHeight);
			WritingStringNullableAttrBool2(L"zeroHeight", m_oZeroHeight);
			WritingStringNullableAttrBool2(L"thickTop", m_oThickTop);
			WritingStringNullableAttrBool2(L"thickBottom", m_oThickBottom);
			WritingStringNullableAttrInt2(L"outlineLevelRow", m_oOutlineLevelRow);
			WritingStringNullableAttrInt2(L"outlineLevelCol", m_oOutlineLevelCol);
			writer.WriteString((L"/>"));
		}
		void CSheetFormatPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CSheetFormatPr::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CSheetFormatPr::toBin()
		{
			auto ptr(new XLSB::WsFmtInfo);
			XLS::BaseObjectPtr Castedptr(ptr);
			if(m_oBaseColWidth.IsInit())
				ptr->dxGCol = m_oBaseColWidth.get() * 256.;
            else
                ptr->dxGCol = 2304;
			if(m_oDefaultColWidth.IsInit())
			{
				ptr->cchDefColWidth = m_oDefaultColWidth.get();
				if(!m_oBaseColWidth.IsInit())
				{
                    ptr->dxGCol = m_oDefaultColWidth.get() * 256;
				}
			}
            else
            {
                ptr->cchDefColWidth = 9;
            }
				if (m_oDefaultRowHeight.IsInit())
					ptr->miyDefRwHeight = m_oDefaultRowHeight.get() * 20;
				else
                    ptr->miyDefRwHeight = 290;

                if (m_oOutlineLevelCol.IsInit())
                    ptr->iOutLevelCol =  m_oOutlineLevelCol.get();
				else
                    ptr->iOutLevelCol = 0;
                if (m_oOutlineLevelRow.IsInit())
                    ptr->iOutLevelRw = m_oOutlineLevelRow.get();
				else
                     ptr->iOutLevelRw = 0;

				if (m_oThickBottom.IsInit()) ptr->fExDesc = m_oThickBottom.get();
				else  ptr->fExDesc = false;
				if (m_oThickTop.IsInit()) ptr->fExAsc = m_oThickTop.get();
				else ptr->fExAsc = false;
				if (m_oZeroHeight.IsInit()) ptr->fDyZero = m_oZeroHeight.get();
				else ptr->fDyZero = false;
                ptr->fUnsynced = false;
			return Castedptr;
		}
        void CSheetFormatPr::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_WsFmtInfo);
            {
                _UINT32 dxGCol =  0xFFFFFFFF;
                if(m_oBaseColWidth.IsInit())
                    dxGCol = m_oBaseColWidth.get() * 256.;
                *record << dxGCol;
            }
            {
                _UINT16 cchDefColWidth = 9;
                if(m_oDefaultColWidth.IsInit())
                {
                    cchDefColWidth = m_oDefaultColWidth.get();

                }
                *record << cchDefColWidth;
            }
            {
                _UINT16 miyDefRwHeight =  290;
                if (m_oDefaultRowHeight.IsInit())
                    miyDefRwHeight = m_oDefaultRowHeight.get() * 20;
                *record << miyDefRwHeight;
            }
            {
                _UINT16 flags = 0;
                if (m_oZeroHeight.IsInit())
                    SETBIT(flags, 1, m_oZeroHeight.get())
                if (m_oThickTop.IsInit())
                    SETBIT(flags, 2, m_oThickTop.get())
                if (m_oThickBottom.IsInit())
                    SETBIT(flags, 3, m_oThickBottom.get())
                *record << flags;
            }
            {
                BYTE rowCol = 0;;
                if (m_oOutlineLevelCol.IsInit())
                    rowCol =  m_oOutlineLevelCol.get();
                *record << rowCol;
                if (m_oOutlineLevelRow.IsInit())
                    rowCol = m_oOutlineLevelRow.get();
                else
                     rowCol = 0;
                *record << rowCol;
            }
            writer->storeNextRecord(record);
        }
		EElementType CSheetFormatPr::getType() const
		{
			return et_x_SheetFormatPr;
		}
		void CSheetFormatPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"baseColWidth"), m_oBaseColWidth)	// ToDo Excel не воспринимает значения не uint (мы приводим к uint)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"customHeight"), m_oCustomHeight)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"defaultColWidth"), m_oDefaultColWidth)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"defaultRowHeight"), m_oDefaultRowHeight)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"outlineLevelCol"), m_oOutlineLevelCol)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"outlineLevelRow"), m_oOutlineLevelRow)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"thickBottom"), m_oThickBottom)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"thickTop"), m_oThickTop)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"zeroHeight"), m_oZeroHeight)
				WritingElement_ReadAttributes_End(oReader)
		}
		void CSheetFormatPr::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::WsFmtInfo*>(obj.get());
			if (ptr != nullptr)
			{
				if (ptr->dxGCol != 0xFFFFFFFF)
                {
					m_oDefaultColWidth =  ptr->dxGCol / 256.;
					m_oBaseColWidth = m_oDefaultColWidth.get();
                }
                else
                    m_oDefaultColWidth = ptr->cchDefColWidth;

				if (ptr->fUnsynced)
					m_oDefaultRowHeight = ptr->miyDefRwHeight;
				else
					m_oDefaultRowHeight = 14.4;

				if (ptr->iOutLevelCol > 0) m_oOutlineLevelCol = ptr->iOutLevelCol;
				if (ptr->iOutLevelRw > 0) m_oOutlineLevelRow = ptr->iOutLevelRw;

				if (ptr->fExDesc) m_oThickBottom = ptr->fExDesc;
				if (ptr->fExAsc) m_oThickTop = ptr->fExAsc;
				if (ptr->fDyZero) m_oZeroHeight = ptr->fDyZero;
			}
		}

		CPane::CPane()
		{
		}
		CPane::~CPane()
		{
		}
		void CPane::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CPane::toXML() const
		{
			return (L"");
		}
		void CPane::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<pane"));
			WritingStringNullableAttrDouble(L"xSplit", m_oXSplit, m_oXSplit->GetValue());
			WritingStringNullableAttrDouble(L"ySplit", m_oYSplit, m_oYSplit->GetValue());
			WritingStringNullableAttrString(L"topLeftCell", m_oTopLeftCell, m_oTopLeftCell.get());
			WritingStringNullableAttrString(L"activePane", m_oActivePane, m_oActivePane->ToString());
			WritingStringNullableAttrString(L"state", m_oState, m_oState->ToString());
			writer.WriteString((L"/>"));
		}
		void CPane::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CPane::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CPane::toBin()
		{
			auto ptr(new XLSB::Pane);
			XLS::BaseObjectPtr objectPtr(ptr);

			if(m_oActivePane.IsInit())
			{
				if(m_oActivePane == SimpleTypes::Spreadsheet::EActivePane::activepaneBottomRight)
					ptr->pnnAcct_xlsb = 0;
				else if(m_oActivePane == SimpleTypes::Spreadsheet::EActivePane::activepaneTopRight)
					ptr->pnnAcct_xlsb = 1;
				else if(m_oActivePane == SimpleTypes::Spreadsheet::EActivePane::activepaneBottomLeft)
					ptr->pnnAcct_xlsb = 2;
				else if(m_oActivePane == SimpleTypes::Spreadsheet::EActivePane::activepaneTopLeft)
					ptr->pnnAcct_xlsb = 3;
			}
            ptr->fFrozen = false;
            ptr->fFrozenNoSplit = false;
			if(m_oState.IsInit())
			{
				if(m_oState == SimpleTypes::Spreadsheet::EPaneState::panestateFrozenSplit)
					ptr->fFrozen = true;
				else if(m_oState == SimpleTypes::Spreadsheet::EPaneState::panestateFrozen)
					ptr->fFrozenNoSplit = true;
			}
			if(m_oTopLeftCell.IsInit())
				ptr->topLeftCell = m_oTopLeftCell.get();
			if(m_oXSplit.IsInit())
				ptr->xnumXSplit.data.value = m_oXSplit->GetValue();
			else
				ptr->xnumXSplit.data.value = 0;
			if(m_oYSplit.IsInit())
				ptr->xnumYSplit.data.value = m_oYSplit->GetValue();
			else
				ptr->xnumYSplit.data.value = 0;

			return objectPtr;
		}
        void CPane::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_Pane);
            {
                XLS::Xnum numsplit;
                if(m_oXSplit.IsInit())
                    numsplit.data.value = m_oXSplit->GetValue();
                else
                    numsplit.data.value = 0;
                *record << numsplit;
                if(m_oYSplit.IsInit())
                    numsplit.data.value = m_oYSplit->GetValue();
                else
                    numsplit.data.value = 0;
                *record << numsplit;
            }
            {
                _INT32 rowCol = 0;
                XLS::CellRef ref;
                if(m_oTopLeftCell.IsInit())
                    ref.fromString(m_oTopLeftCell.get());
                rowCol = ref.row;
                *record << rowCol;
                rowCol = ref.column;
                *record << rowCol;
            }
            {
                _UINT32 pnnAct = 0;
                if(m_oActivePane.IsInit())
                {
                    if(m_oActivePane == SimpleTypes::Spreadsheet::EActivePane::activepaneBottomRight)
                        pnnAct = 0;
                    else if(m_oActivePane == SimpleTypes::Spreadsheet::EActivePane::activepaneTopRight)
                        pnnAct = 1;
                    else if(m_oActivePane == SimpleTypes::Spreadsheet::EActivePane::activepaneBottomLeft)
                        pnnAct = 2;
                    else if(m_oActivePane == SimpleTypes::Spreadsheet::EActivePane::activepaneTopLeft)
                        pnnAct = 3;
                }
                *record << pnnAct;
            }
            BYTE flags = 0;
            if(m_oState.IsInit())
            {
                if(m_oState == SimpleTypes::Spreadsheet::EPaneState::panestateFrozen)
                    SETBIT(flags, 0, 1)
                else if(m_oState == SimpleTypes::Spreadsheet::EPaneState::panestateFrozenSplit)
                    SETBIT(flags, 1, 1)
            }
            *record << flags;
            writer->storeNextRecord(record);
        }
		EElementType CPane::getType() const
		{
			return et_x_Pane;
		}
		void CPane::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"activePane"), m_oActivePane)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"state"), m_oState)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"topLeftCell"), m_oTopLeftCell)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"xSplit"), m_oXSplit)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"ySplit"), m_oYSplit)
				WritingElement_ReadAttributes_End(oReader)
		}
		void CPane::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto pPane = static_cast<XLSB::Pane*>(obj.get());
			if (pPane != nullptr)
			{
				switch (pPane->pnnAcct_xlsb)
				{
				case 0:  m_oActivePane = SimpleTypes::Spreadsheet::EActivePane::activepaneBottomRight; break;
				case 1:  m_oActivePane = SimpleTypes::Spreadsheet::EActivePane::activepaneTopRight; break;
				case 2:  m_oActivePane = SimpleTypes::Spreadsheet::EActivePane::activepaneBottomLeft; break;
				case 3:  m_oActivePane = SimpleTypes::Spreadsheet::EActivePane::activepaneTopLeft; break;
				}

				if (pPane->fFrozen)
					m_oState = SimpleTypes::Spreadsheet::EPaneState::panestateFrozenSplit;
				else if (pPane->fFrozenNoSplit)
					m_oState = SimpleTypes::Spreadsheet::EPaneState::panestateFrozen;
				else
					m_oState = SimpleTypes::Spreadsheet::EPaneState::panestateSplit;

				m_oTopLeftCell = pPane->topLeftCell;
				m_oXSplit = pPane->xnumXSplit.data.value;
				m_oYSplit = pPane->xnumYSplit.data.value;
			}
		}

		CSelection::CSelection()
		{
		}
		CSelection::~CSelection()
		{
		}
		void CSelection::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSelection::toXML() const
		{
			return (L"");
		}
		void CSelection::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<selection"));
			WritingStringNullableAttrString(L"pane", m_oPane, m_oPane->ToString());
			WritingStringNullableAttrString(L"activeCell", m_oActiveCell, m_oActiveCell.get());
			WritingStringNullableAttrInt(L"activeCellId", m_oActiveCellId, m_oActiveCellId->GetValue());
			WritingStringNullableAttrString(L"sqref", m_oSqref, m_oSqref.get());
			writer.WriteString((L"/>"));
		}
		void CSelection::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CSelection::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CSelection::toBin()
		{
			auto ptr(new XLSB::Sel);
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_oActiveCell.IsInit())
				ptr->activeCell = m_oActiveCell.get();
			else
				ptr->activeCell = L"A1";
			if(m_oActiveCellId.IsInit())
				ptr->irefAct = m_oActiveCellId->GetValue();
			else
				ptr->irefAct = 0;
			if(m_oSqref.IsInit())
				ptr->sqref = m_oSqref.get();
			else
				ptr->sqref = L"A1";
			if(m_oPane.IsInit())
			{
				if(m_oPane == SimpleTypes::Spreadsheet::EActivePane::activepaneBottomRight)
					ptr->pnn_xlsb = 0;
				else if(m_oPane == SimpleTypes::Spreadsheet::EActivePane::activepaneTopRight)
					ptr->pnn_xlsb = 1;
				else if(m_oPane == SimpleTypes::Spreadsheet::EActivePane::activepaneBottomLeft)
					ptr->pnn_xlsb = 2;
				else if(m_oPane == SimpleTypes::Spreadsheet::EActivePane::activepaneTopLeft)
					ptr->pnn_xlsb = 3;
			}
			else
				ptr->pnn_xlsb = 3;
			ptr->rwAct = 0;
			ptr->colAct = 0;

			return objectPtr;
		}
        void CSelection::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_Sel);
            _UINT32 pnn = 3;
            if(m_oPane.IsInit())
            {
                if(m_oPane == SimpleTypes::Spreadsheet::EActivePane::activepaneBottomRight)
                    pnn = 0;
                else if(m_oPane == SimpleTypes::Spreadsheet::EActivePane::activepaneTopRight)
                    pnn = 1;
                else if(m_oPane == SimpleTypes::Spreadsheet::EActivePane::activepaneBottomLeft)
                    pnn = 2;
                else if(m_oPane == SimpleTypes::Spreadsheet::EActivePane::activepaneTopLeft)
                    pnn = 3;
            }
            *record << pnn;
            {
                XLS::CellRef ref;
                if(m_oActiveCell.IsInit())
                    ref.fromString(m_oActiveCell.get());
                _INT32 rowCol = 0;
                rowCol = ref.row;
                *record << rowCol;
                rowCol = ref.column;
                *record << rowCol;
            }
            _UINT32 irefAct = 0;
            if(m_oActiveCellId.IsInit())
                irefAct = m_oActiveCellId->GetValue();
            *record << irefAct;
            XLSB::UncheckedSqRfX    sqrfx;
            if(m_oSqref.IsInit())
                sqrfx.strValue = m_oSqref.get();
            else
                sqrfx.strValue = L"A1";
            *record << sqrfx;
            writer->storeNextRecord(record);
        }
		EElementType CSelection::getType() const
		{
			return et_x_Selection;
		}
		void CSelection::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"activeCell"), m_oActiveCell)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"activeCellId"), m_oActiveCellId)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"sqref"), m_oSqref)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"pane"), m_oPane)
				WritingElement_ReadAttributes_End(oReader)
		}
		void CSelection::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto pSel = static_cast<XLSB::Sel*>(obj.get());
			if (pSel != nullptr)
			{
				m_oActiveCell = pSel->activeCell;
				m_oActiveCellId = pSel->irefAct;
				m_oSqref = pSel->sqref;
				switch (pSel->pnn_xlsb)
				{
				case 0:  m_oPane = SimpleTypes::Spreadsheet::EActivePane::activepaneBottomRight; break;
				case 1:  m_oPane = SimpleTypes::Spreadsheet::EActivePane::activepaneTopRight; break;
				case 2:  m_oPane = SimpleTypes::Spreadsheet::EActivePane::activepaneBottomLeft; break;
				case 3:  m_oPane = SimpleTypes::Spreadsheet::EActivePane::activepaneTopLeft; break;
				}
			}
		}

		CSheetView::CSheetView()
		{
		}
		CSheetView::~CSheetView()
		{
		}
		void CSheetView::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSheetView::toXML() const
		{
			return (L"");
		}
		void CSheetView::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<sheetView"));
			WritingStringNullableAttrBool(L"windowProtection", m_oWindowProtection);
			WritingStringNullableAttrBool(L"showFormulas", m_oShowFormulas);
			WritingStringNullableAttrBool(L"showGridLines", m_oShowGridLines);
			WritingStringNullableAttrBool(L"showRowColHeaders", m_oShowRowColHeaders);
			WritingStringNullableAttrBool(L"showZeros", m_oShowZeros)
				WritingStringNullableAttrBool(L"rightToLeft", m_oRightToLeft);
			WritingStringNullableAttrBool(L"tabSelected", m_oTabSelected);
			WritingStringNullableAttrBool(L"showRuler", m_oShowRuler);
			WritingStringNullableAttrBool(L"showOutlineSymbols", m_oShowOutlineSymbols);
			WritingStringNullableAttrBool(L"defaultGridColor", m_oDefaultGridColor);
			WritingStringNullableAttrBool(L"showWhiteSpace", m_oShowWhiteSpace);
			WritingStringNullableAttrString(L"view", m_oView, m_oView->ToString());
			WritingStringNullableAttrString(L"topLeftCell", m_oTopLeftCell, m_oTopLeftCell.get());
			WritingStringNullableAttrInt(L"colorId", m_oColorId, m_oColorId->GetValue());
			WritingStringNullableAttrInt(L"zoomScale", m_oZoomScale, m_oZoomScale->GetValue());
			WritingStringNullableAttrInt(L"zoomScaleNormal", m_oZoomScaleNormal, m_oZoomScaleNormal->GetValue());
			WritingStringNullableAttrInt(L"zoomScaleSheetLayoutView", m_oZoomScaleSheetLayoutView, m_oZoomScaleSheetLayoutView->GetValue());
			WritingStringNullableAttrInt(L"zoomScalePageLayoutView", m_oZoomScalePageLayoutView, m_oZoomScalePageLayoutView->GetValue());
			WritingStringNullableAttrInt(L"workbookViewId", m_oWorkbookViewId, m_oWorkbookViewId->GetValue());
			writer.WriteString((L">"));

			if (m_oPane.IsInit())
				m_oPane->toXML(writer);

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString((L"</sheetView>"));
		}
		void CSheetView::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"pane" == sName)
					m_oPane = oReader;
				if (L"selection" == sName)
				{
					CSelection* pSelection = new CSelection();
					*pSelection = oReader;
					m_arrItems.push_back(pSelection);
				}
			}
		}
		void CSheetView::fromBin(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typeWSVIEW2)
			{
				auto pWSVIEW2 = static_cast<XLSB::WSVIEW2*>(obj.get());
				if (pWSVIEW2 == nullptr)
					return;

				ReadAttributes(pWSVIEW2->m_BrtBeginWsView);

				m_oPane = pWSVIEW2->m_BrtPane;

				if (pWSVIEW2->m_arBrtSel.empty())
					return;

				for (auto &pSel : pWSVIEW2->m_arBrtSel)
				{
					m_arrItems.push_back(new CSelection(pSel));
				}
			}
			else if (obj->get_type() == XLS::typeCSVIEW)
			{
				auto pCSVIEW = static_cast<XLSB::CSVIEW*>(obj.get());
				if (pCSVIEW == nullptr)
					return;

				ReadAttributes(pCSVIEW->m_BrtBeginCsView);
			}
		}
		XLS::BaseObjectPtr CSheetView::toBin()
		{
				auto ptr(new XLSB::WSVIEW2);
				XLS::BaseObjectPtr castedPtr(ptr);
				auto pWsView(new XLSB::BeginWsView);
				ptr->m_BrtBeginWsView = XLS::BaseObjectPtr{pWsView};

				if (m_oColorId.IsInit())
    				pWsView->icvHdr = m_oColorId->m_eValue;
                else
                    pWsView->icvHdr = 64;
				if (m_oDefaultGridColor.IsInit())
					pWsView->fDefaultHdr = m_oDefaultGridColor->m_eValue;
				else
                    pWsView->fDefaultHdr = true;
				if (m_oRightToLeft.IsInit())
					pWsView->fRightToLeft = m_oRightToLeft->m_eValue;
				else
					pWsView->fRightToLeft = false;
				if (m_oShowFormulas.IsInit())
					pWsView->fDspFmlaRt = m_oShowFormulas->m_eValue;
				else
                    pWsView->fDspFmlaRt = false;
				if (m_oShowGridLines.IsInit())
					pWsView->fDspGridRt = m_oShowGridLines->m_eValue;
				else
                    pWsView->fDspGridRt = true;
				if (m_oShowOutlineSymbols.IsInit())
					pWsView->fDspGuts = m_oShowOutlineSymbols->m_eValue;
				else
                    pWsView->fDspGuts = true;
				if (m_oShowRowColHeaders.IsInit())
					pWsView->fDspRwColRt = m_oShowRowColHeaders->m_eValue;
				else
					pWsView->fDspRwColRt = true;
				if (m_oShowRuler.IsInit())
					pWsView->fDspRuler = m_oShowRuler->m_eValue;
				else
					pWsView->fDspRuler = false;
				if (m_oShowWhiteSpace.IsInit())
					pWsView->fWhitespaceHidden = !m_oShowWhiteSpace->m_eValue;
				else
					pWsView->fWhitespaceHidden = false;
				if (m_oShowZeros.IsInit())
					pWsView->fDspZerosRt = m_oShowZeros->m_eValue;
				else
					pWsView->fDspZerosRt = true;
				if (m_oTabSelected.IsInit())
					pWsView->fSelected = m_oTabSelected->m_eValue;
				else
					pWsView->fSelected = false;
				if(m_oTopLeftCell.IsInit())
					pWsView->topLeftCell = m_oTopLeftCell.get();
				else
					pWsView->topLeftCell = L"A1";
				if (m_oView.IsInit())
					pWsView->xlView = m_oView->m_eValue;
				else
					pWsView->xlView = 0;
				if (m_oWindowProtection.IsInit())
					pWsView->fWnProt = m_oWindowProtection->m_eValue;
				else
					pWsView->fWnProt = false;
				if (m_oWorkbookViewId.IsInit())
					pWsView->iWbkView = m_oWorkbookViewId->m_eValue;
				if (m_oZoomScale.IsInit())
					pWsView->wScale = m_oZoomScale->m_eValue;
				else
					pWsView->wScale = 100;
				if (m_oZoomScaleNormal.IsInit())
					pWsView->wScaleNormal = m_oZoomScaleNormal->m_eValue;
				else
					pWsView->wScaleNormal = 0;
				if (m_oZoomScalePageLayoutView.IsInit())
					pWsView->wScalePLV = m_oZoomScalePageLayoutView->m_eValue;
				else
					pWsView->wScalePLV = 0;
				if (m_oZoomScaleSheetLayoutView.IsInit())
					pWsView->wScaleSLV = m_oZoomScaleSheetLayoutView->m_eValue;
				else
					pWsView->wScaleSLV = 0;
                pWsView->rwTop = 0;
                pWsView->colLeft = 0;
                if(m_oPane.IsInit())
					ptr->m_BrtPane = m_oPane->toBin();

				for(auto i:m_arrItems)
				{
                    ptr->m_arBrtSel.push_back(i->toBin());
				}
				return castedPtr;
           
		}
        void CSheetView::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            {
                auto beginView = writer->getNextRecord(XLSB::rt_BeginWsView);
                writeAttributes(beginView);
                writer->storeNextRecord(beginView);
            }
            if(m_oPane.IsInit())
                m_oPane->toBin(writer);
            for(auto i:m_arrItems)
            {
                i->toBin(writer);
            }
            {
                auto endView = writer->getNextRecord(XLSB::rt_EndWsView);
                writer->storeNextRecord(endView);
            }
        }
		XLS::BaseObjectPtr CSheetView::toBinCs()
		{
            auto ptr(new XLSB::CSVIEW);
			auto pWsView(new XLSB::BeginCsView);
            ptr->m_BrtBeginCsView = XLS::BaseObjectPtr{pWsView};
            XLS::BaseObjectPtr castedPtr(ptr);
			if(m_oTabSelected.IsInit())
				pWsView->fSelected = m_oTabSelected->m_eValue;
			else
				pWsView->fSelected = false;
			if(m_oWorkbookViewId.IsInit())
				pWsView->iWbkView = m_oWorkbookViewId->m_eValue;
			else
				pWsView->iWbkView = 0;
			if(m_oZoomScale.IsInit())
				pWsView->wScale = m_oZoomScale->m_eValue;
			else
				pWsView->wScale = 100;
			return castedPtr;
		}
		EElementType CSheetView::getType() const
		{
			return et_x_SheetView;
		}
		void CSheetView::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"colorId"), m_oColorId)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"defaultGridColor"), m_oDefaultGridColor)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"rightToLeft"), m_oRightToLeft)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"showFormulas"), m_oShowFormulas)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"showGridLines"), m_oShowGridLines)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"showOutlineSymbols"), m_oShowOutlineSymbols)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"showRowColHeaders"), m_oShowRowColHeaders)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"showRuler"), m_oShowRuler)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"showWhiteSpace"), m_oShowWhiteSpace)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"showZeros"), m_oShowZeros)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"tabSelected"), m_oTabSelected)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"topLeftCell"), m_oTopLeftCell)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"view"), m_oView)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"windowProtection"), m_oWindowProtection)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"workbookViewId"), m_oWorkbookViewId)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"zoomScale"), m_oZoomScale)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"zoomScaleNormal"), m_oZoomScaleNormal)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"zoomScalePageLayoutView"), m_oZoomScalePageLayoutView)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"zoomScaleSheetLayoutView"), m_oZoomScaleSheetLayoutView)
				WritingElement_ReadAttributes_End(oReader)
		}
		void CSheetView::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typeBeginWsView)
			{
				auto pWsView = static_cast<XLSB::BeginWsView*>(obj.get());
				if (pWsView != nullptr)
				{
					m_oColorId = pWsView->icvHdr;
					m_oDefaultGridColor = pWsView->fDefaultHdr;
					m_oRightToLeft = pWsView->fRightToLeft;
					m_oShowFormulas = pWsView->fDspFmlaRt;
					m_oShowGridLines = pWsView->fDspGridRt;
					m_oShowOutlineSymbols = pWsView->fDspGuts;
					m_oShowRowColHeaders = pWsView->fDspRwColRt;
					m_oShowRuler = pWsView->fDspRuler;
                    m_oShowWhiteSpace = !pWsView->fWhitespaceHidden;
					m_oShowZeros = pWsView->fDspZerosRt;
					m_oTabSelected = pWsView->fSelected;
					m_oTopLeftCell = pWsView->topLeftCell;
					m_oView = (SimpleTypes::Spreadsheet::ESheetViewType)pWsView->xlView;
					m_oWindowProtection = pWsView->fWnProt;
					m_oWorkbookViewId = pWsView->iWbkView;
					m_oZoomScale = pWsView->wScale;
					m_oZoomScaleNormal = pWsView->wScaleNormal;
					m_oZoomScalePageLayoutView = pWsView->wScalePLV;
					m_oZoomScaleSheetLayoutView = pWsView->wScaleSLV;
				}
			}
			else if (obj->get_type() == XLS::typeBeginCsView)
			{
				auto pWsView = static_cast<XLSB::BeginCsView*>(obj.get());
				if (pWsView != nullptr)
				{
					m_oTabSelected = pWsView->fSelected;
					m_oWorkbookViewId = pWsView->iWbkView;
					m_oZoomScale = pWsView->wScale;
				}
			}
		}
        void CSheetView::writeAttributes(XLS::CFRecordPtr& record)
        {
        {
                _UINT16 flags = 0;
                if (m_oWindowProtection.IsInit())
                    SETBIT(flags, 0, m_oWindowProtection->GetValue())
                if (m_oShowFormulas.IsInit())
                    SETBIT(flags, 1, m_oShowFormulas->GetValue())
                if (m_oShowGridLines.IsInit())
                    SETBIT(flags, 2, m_oShowGridLines->GetValue())
                 else
                    SETBIT(flags, 2, 1)
                if (m_oShowRowColHeaders.IsInit())
                    SETBIT(flags, 3, m_oShowRowColHeaders->GetValue())
                else
                    SETBIT(flags, 3, 1)
                if (m_oShowZeros.IsInit())
                    SETBIT(flags, 4, m_oShowZeros->GetValue())
                else
                    SETBIT(flags, 4, 1)
                if (m_oRightToLeft.IsInit())
                    SETBIT(flags, 5, m_oRightToLeft->GetValue())
                if (m_oTabSelected.IsInit())
                    SETBIT(flags, 6, m_oTabSelected->GetValue())
                if (m_oShowRuler.IsInit())
                    SETBIT(flags, 7, m_oShowRuler->GetValue())
                if (m_oShowOutlineSymbols.IsInit())
                    SETBIT(flags, 8, m_oShowOutlineSymbols->GetValue())
                else
                    SETBIT(flags, 8, 1)
                if (m_oDefaultGridColor.IsInit())
                    SETBIT(flags, 9, m_oDefaultGridColor->GetValue())
                else
                    SETBIT(flags, 9, 1)
                if (m_oShowWhiteSpace.IsInit())
                    SETBIT(flags, 10, !m_oShowWhiteSpace->GetValue())
                 _UINT32 xlView = 0;
                 if (m_oView.IsInit())
                        xlView = m_oView->m_eValue;
                *record <<flags << xlView;
            }
            {
                XLS::CellRef ref;
                if(m_oTopLeftCell.IsInit())
                    ref.fromString(m_oTopLeftCell.get());
                _INT32 rwtop = ref.row;
                _INT32 colLeft = ref.column;
                *record << rwtop << colLeft;
            }
            BYTE icvHdr = 64;
            if(m_oColorId.IsInit())
            icvHdr = m_oColorId->m_eValue;
            *record << icvHdr;
            record->reserveNunBytes(3);

            _UINT16 wScale = 100;
            if (m_oZoomScale.IsInit())
                wScale = m_oZoomScale->m_eValue;
            *record << wScale;
            if (m_oZoomScaleNormal.IsInit())
                wScale = m_oZoomScaleNormal->m_eValue;
            else
                wScale = 0;
            *record << wScale;
            if (m_oZoomScaleSheetLayoutView.IsInit())
                    wScale = m_oZoomScaleSheetLayoutView->m_eValue;
                else
                    wScale = 0;
            *record << wScale;
            if (m_oZoomScalePageLayoutView.IsInit())
                wScale = m_oZoomScalePageLayoutView->m_eValue;
            else
                wScale = 0;
            *record << wScale;
            _UINT32 iWbkView = 0;
            if (m_oWorkbookViewId.IsInit())
                    iWbkView = m_oWorkbookViewId->m_eValue;
            *record << iWbkView;
        }

		CSheetViews::CSheetViews()
		{
		}
		CSheetViews::~CSheetViews()
		{
		}
		void CSheetViews::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSheetViews::toXML() const
		{
			return (L"");
		}
		void CSheetViews::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString((L"<sheetViews>"));

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString((L"</sheetViews>"));
		}
		void CSheetViews::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"sheetView" == sName)
				{
					CSheetView* pSheetView = new CSheetView();
					*pSheetView = oReader;
					m_arrItems.push_back(pSheetView);
				}
			}
		}
		void CSheetViews::fromBin(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typeWSVIEWS2)
			{
				auto oWSVIEWS2 = static_cast<XLSB::WSVIEWS2*>(obj.get());
				for (auto &pView : oWSVIEWS2->m_arWSVIEW2)
				{
					CSheetView *pSheetView = new CSheetView(pView);
					m_arrItems.push_back(pSheetView);

				}
			}
			else if (obj->get_type() == XLS::typeCSVIEWS)
			{
				auto oCSVIEWS = static_cast<XLSB::CSVIEWS*>(obj.get());

				for (auto &pView : oCSVIEWS->m_arCSVIEW)
				{
					CSheetView *pSheetView = new CSheetView(pView);
					m_arrItems.push_back(pSheetView);

				}
			}
		}
		XLS::BaseObjectPtr CSheetViews::toBin()
		{
				auto castedPtr(new XLSB::WSVIEWS2);
				XLS::BaseObjectPtr ptr(castedPtr);
				for(auto i:m_arrItems)
					castedPtr->m_arWSVIEW2.push_back(i->toBin());
				return ptr;
		}
        void CSheetViews::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            {
                auto record = writer->getNextRecord(XLSB::rt_BeginWsViews);
                writer->storeNextRecord(record);
            }
            for(auto i:m_arrItems)
                i->toBin(writer);
            {
                auto record = writer->getNextRecord(XLSB::rt_EndWsViews);
                writer->storeNextRecord(record);
            }

        }
		XLS::BaseObjectPtr CSheetViews::toBinCs()
		{
			auto ptr(new XLSB::CSVIEWS);
			for(auto i:m_arrItems)
				ptr->m_arCSVIEW.push_back(i->toBinCs());
			return XLS::BaseObjectPtr{ptr};
		}
		EElementType CSheetViews::getType() const
		{
			return et_x_SheetViews;
		}

		CPageSetUpPr::CPageSetUpPr()
		{
		}
		CPageSetUpPr::~CPageSetUpPr()
		{
		}
		void CPageSetUpPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CPageSetUpPr::toXML() const
		{
			return (L"");
		}
		void CPageSetUpPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<pageSetUpPr"));
			WritingStringNullableAttrBool(L"autoPageBreaks", m_oAutoPageBreaks);
			WritingStringNullableAttrBool(L"fitToPage", m_oFitToPage);
			writer.WriteString((L"/>"));
		}
		void CPageSetUpPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CPageSetUpPr::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		void CPageSetUpPr::toBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::WsProp*>(obj.get());
			if(m_oAutoPageBreaks.IsInit())
				ptr->fShowAutoBreaks = m_oAutoPageBreaks->GetValue();
			else
				ptr->fShowAutoBreaks = true;
			if(m_oFitToPage.IsInit())
				ptr->fFitToPage = m_oFitToPage->GetValue();
			else
				ptr->fFitToPage = false;
		}
		EElementType CPageSetUpPr::getType() const
		{
			return et_x_PageSetUpPr;
		}
		void CPageSetUpPr::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::WsProp*>(obj.get());
			if (ptr != nullptr)
			{
				m_oAutoPageBreaks = ptr->fShowAutoBreaks;
				m_oFitToPage = ptr->fFitToPage;
			}
		}
		void CPageSetUpPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"autoPageBreaks"), m_oAutoPageBreaks)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"fitToPage"), m_oFitToPage)
				WritingElement_ReadAttributes_End(oReader)
		}

		COutlinePr::COutlinePr()
		{
		}
		COutlinePr::~COutlinePr()
		{
		}
		void COutlinePr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring COutlinePr::toXML() const
		{
			return (L"");
		}
		void COutlinePr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<outlinePr"));
			WritingStringNullableAttrBool(L"applyStyles", m_oApplyStyles);
			WritingStringNullableAttrBool(L"summaryBelow", m_oSummaryBelow);
			WritingStringNullableAttrBool(L"summaryRight", m_oSummaryRight);
			WritingStringNullableAttrBool(L"showOutlineSymbols", m_oShowOutlineSymbols);
			writer.WriteString((L"/>"));
		}
		void COutlinePr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void COutlinePr::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		void COutlinePr::toBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::WsProp*>(obj.get());
			if(m_oApplyStyles.IsInit())
				ptr->fApplyStyles = m_oApplyStyles->GetValue();
			else
				ptr->fApplyStyles = false;
			if(m_oShowOutlineSymbols.IsInit())
				ptr->fShowOutlineSymbols = m_oShowOutlineSymbols->GetValue();
			else
				ptr->fShowOutlineSymbols = true;
			if(m_oSummaryBelow.IsInit())
				ptr->fRowSumsBelow = m_oSummaryBelow->GetValue();
			else
				ptr->fRowSumsBelow = true;
			if(m_oSummaryRight.IsInit())
				ptr->fColSumsRight = m_oSummaryRight->GetValue();
			else
				ptr->fColSumsRight = true;
		}
		EElementType COutlinePr::getType() const
		{
			return et_x_OutlinePr;
		}
		void COutlinePr::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::WsProp*>(obj.get());
			if (ptr != nullptr)
			{
				m_oApplyStyles = ptr->fApplyStyles;
				m_oShowOutlineSymbols = ptr->fShowOutlineSymbols;
				m_oSummaryBelow = ptr->fRowSumsBelow;
				m_oSummaryRight = ptr->fColSumsRight;
			}
		}
		void COutlinePr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"applyStyles"), m_oApplyStyles)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"showOutlineSymbols"), m_oShowOutlineSymbols)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"summaryBelow"), m_oSummaryBelow)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"summaryRight"), m_oSummaryRight)
				WritingElement_ReadAttributes_End(oReader)
		}

		CSheetPr::CSheetPr()
		{
		}
		CSheetPr::~CSheetPr()
		{
		}
		void CSheetPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSheetPr::toXML() const
		{
			return (L"");
		}
		void CSheetPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<sheetPr"));
			WritingStringNullableAttrBool(L"syncHorizontal", m_oSyncHorizontal);
			WritingStringNullableAttrBool(L"syncVertical", m_oSyncVertical);
			WritingStringNullableAttrEncodeXmlString(L"syncRef", m_oSyncRef, m_oSyncRef.get());
			WritingStringNullableAttrBool(L"transitionEvaluation", m_oTransitionEvaluation);
			WritingStringNullableAttrBool(L"transitionEntry", m_oTransitionEntry);
			WritingStringNullableAttrBool(L"published", m_oPublished);
			WritingStringNullableAttrEncodeXmlString(L"codeName", m_oCodeName, m_oCodeName.get());
			WritingStringNullableAttrBool(L"filterMode", m_oFilterMode);
			WritingStringNullableAttrBool(L"enableFormatConditionsCalculation", m_oEnableFormatConditionsCalculation);
			writer.WriteString((L">"));
			if (m_oTabColor.IsInit())
			{
				m_oTabColor->toXML2(writer, (L"tabColor"));
			}
			if (m_oOutlinePr.IsInit())
			{
				m_oOutlinePr->toXML(writer);
			}
			if (m_oPageSetUpPr.IsInit())
			{
				m_oPageSetUpPr->toXML(writer);
			}
			writer.WriteString((L"</sheetPr>"));

		}
		void CSheetPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ((L"tabColor") == sName)
					m_oTabColor = oReader;
				else if ((L"pageSetUpPr") == sName)
					m_oPageSetUpPr = oReader;
				else if ((L"outlinePr") == sName)
					m_oOutlinePr = oReader;
			}
		}
		XLS::BaseObjectPtr CSheetPr::toBin()
		{
            auto ptr(new XLSB::WsProp);
            XLS::BaseObjectPtr objectPtr(ptr);
            if(m_oCodeName.IsInit())
                ptr->strName.value = m_oCodeName.get();
            else
                ptr->strName.value = L"";
            if(m_oEnableFormatConditionsCalculation.IsInit())
                ptr->fCondFmtCalc = m_oEnableFormatConditionsCalculation->GetValue();
            if(m_oFilterMode.IsInit())
                ptr->fFilterMode = m_oFilterMode->GetValue();
            else
                ptr->fFilterMode = false;
            if(m_oPublished.IsInit())
                ptr->fPublish = m_oPublished->GetValue();
            else
                ptr->fPublish = true;
            if(m_oSyncHorizontal.IsInit())
                ptr->fSyncHoriz = m_oSyncHorizontal->GetValue();
            else
                ptr->fSyncHoriz = false;
            if(m_oSyncVertical.IsInit())
                ptr->fSyncVert = m_oSyncVertical->GetValue();
            else
                ptr->fSyncVert = false;
            if(m_oTransitionEntry.IsInit())
                ptr->fAltFormulaEntry = m_oTransitionEntry->GetValue();
            else
                ptr->fAltFormulaEntry = false;
            if(m_oTransitionEvaluation.IsInit())
                ptr->fAltExprEval = m_oTransitionEvaluation->GetValue();
            else
                ptr->fAltExprEval = false;
            if (m_oSyncRef.IsInit())
                ptr->syncRef = m_oSyncRef.get();
            else
                ptr->syncRef = L"";
            if(m_oTabColor.IsInit())
                ptr->brtcolorTab = m_oTabColor->toColor();
            else
                ptr->brtcolorTab = m_oTabColor->GetDefaultColor();
            if(m_oPageSetUpPr.IsInit())
                m_oPageSetUpPr->toBin(objectPtr);
            else
            {
                ptr->fFitToPage = false;
                ptr->fShowAutoBreaks = true;
            }
			if(m_oOutlinePr.IsInit())
                m_oOutlinePr->toBin(objectPtr);
            else
            {
                ptr->fApplyStyles = false;
                ptr->fColSumsRight = true;
                ptr->fRowSumsBelow = true;
                ptr->fShowOutlineSymbols = true;
            }
            ptr->fDialog = false;
            ptr->fCondFmtCalc = false;
            return objectPtr;
		}
        void CSheetPr::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            _UINT16 flags1 = 0;
            BYTE flags2 = 0;
            if(m_oPageSetUpPr.IsInit())
            {
                if(m_oPageSetUpPr->m_oAutoPageBreaks.IsInit())
                    SETBIT(flags1, 0, m_oPageSetUpPr->m_oAutoPageBreaks->GetValue())
                else
                    SETBIT(flags1, 0, 1)
                if(m_oPageSetUpPr->m_oFitToPage.IsInit())
                    SETBIT(flags1, 8, m_oPageSetUpPr->m_oFitToPage->GetValue())
            }
            else
            {
                SETBIT(flags1, 0, 1)
            }
            if(m_oPublished.IsInit())
                SETBIT(flags1, 3, m_oPublished->GetValue())
            else
                SETBIT(flags1, 3, 1);
            if(m_oOutlinePr.IsInit())
            {
                if(m_oOutlinePr->m_oApplyStyles.IsInit())
                    SETBIT(flags1, 5, m_oOutlinePr->m_oApplyStyles->GetValue())

                if(m_oOutlinePr->m_oShowOutlineSymbols.IsInit())
                    SETBIT(flags1, 10, m_oOutlinePr->m_oShowOutlineSymbols->GetValue())
                else
                   SETBIT(flags1, 10, 1)
                if(m_oOutlinePr->m_oSummaryBelow.IsInit())
                   SETBIT(flags1, 6, m_oOutlinePr->m_oSummaryBelow->GetValue())
                else
                    SETBIT(flags1, 6, 1)
                if(m_oOutlinePr->m_oSummaryRight.IsInit())
                    SETBIT(flags1, 7, m_oOutlinePr->m_oSummaryRight->GetValue())
                else
                   SETBIT(flags1, 7, 1)
            }
            else
            {
                SETBIT(flags1, 10, 1)
                SETBIT(flags1, 6, 1)
                SETBIT(flags1, 7, 1)
            }
            if(m_oSyncHorizontal.IsInit())
                SETBIT(flags1, 12, m_oSyncHorizontal->GetValue())
            if(m_oSyncVertical.IsInit())
                SETBIT(flags1, 13, m_oSyncVertical->GetValue())
            if(m_oTransitionEvaluation.IsInit())
                SETBIT(flags1, 14, m_oTransitionEvaluation->GetValue())
            if(m_oTransitionEntry.IsInit())
                SETBIT(flags1, 15, m_oTransitionEntry->GetValue())
            if(m_oFilterMode.IsInit())
                SETBIT(flags2, 16, m_oFilterMode->GetValue())
            auto SheetPr = writer->getNextRecord(XLSB::rt_WsProp);
            *SheetPr << flags1 << flags2;
            XLSB::Color tabColor;
            if(m_oTabColor.IsInit())
                tabColor = m_oTabColor->toColor();
            else
                tabColor = m_oTabColor->GetDefaultColor();
            tabColor.writeFields(*SheetPr);
            _INT32 rwSync = 0xFFFFFFFF;
            _INT32 colSync = 0xFFFFFFFF;
            if (m_oSyncRef.IsInit())
            {
                XLS::CellRef ref(m_oSyncRef.get());
                rwSync = ref.getRow();
                colSync = ref.getColumn();
            }
            *SheetPr << rwSync <<colSync ;
            XLSB::XLWideString codeName;
            if(m_oCodeName.IsInit())
                codeName = m_oCodeName.get();
            else
                codeName = L"";
             *SheetPr << codeName;
            writer->storeNextRecord(SheetPr);
        }
		XLS::BaseObjectPtr CSheetPr::toBinCs()
		{
			auto ptr(new XLSB::CsProp);

			if(m_oCodeName.IsInit())
				ptr->strName = m_oCodeName.get();
			else
                ptr->strName.value.setSize(0);
			if(m_oPublished.IsInit())
				ptr->fPublish = m_oPublished->GetValue();
			else
				ptr->fPublish = false;
			if(m_oTabColor.IsInit())
				ptr->brtcolorTab = m_oTabColor->toColor();
			else
				ptr->brtcolorTab = m_oTabColor->GetDefaultColor();
			return XLS::BaseObjectPtr{ptr};
		}
		
		void CSheetPr::fromBin(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typeWsProp)
			{
				m_oPageSetUpPr = obj;
				m_oOutlinePr = obj;
			}
			ReadAttributes(obj);
		}
		EElementType CSheetPr::getType() const
		{
			return et_x_SheetPr;
		}
		void CSheetPr::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typeWsProp)
			{
				auto ptr = static_cast<XLSB::WsProp*>(obj.get());
				if (ptr != nullptr)
				{
					if (!ptr->strName.value.value().empty())
						m_oCodeName = ptr->strName.value.value();

					m_oEnableFormatConditionsCalculation = ptr->fCondFmtCalc;
					m_oFilterMode = ptr->fFilterMode;
					m_oPublished = ptr->fPublish;
					m_oSyncHorizontal = ptr->fSyncHoriz;
					m_oSyncVertical = ptr->fSyncVert;
					m_oTransitionEntry = ptr->fAltFormulaEntry;
					m_oTransitionEvaluation = ptr->fAltExprEval;

					if (!ptr->syncRef.empty())
						m_oSyncRef = ptr->syncRef;

					m_oTabColor.Init();
					m_oTabColor->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorTab));
				}
			}
			else if (obj->get_type() == XLS::typeCsProp)
			{
				auto ptr = static_cast<XLSB::CsProp*>(obj.get());
				if (ptr != nullptr)
				{
					if (!ptr->strName.value.value().empty())
						m_oCodeName = ptr->strName.value.value();

					m_oPublished = ptr->fPublish;

					m_oTabColor.Init();
					m_oTabColor->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorTab));
				}
			}

		}
		void CSheetPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"codeName"), m_oCodeName)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"enableFormatConditionsCalculation"), m_oEnableFormatConditionsCalculation)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"filterMode"), m_oFilterMode)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"published"), m_oPublished)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"syncHorizontal"), m_oSyncHorizontal)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"syncRef"), m_oSyncRef)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"syncVertical"), m_oSyncVertical)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"transitionEntry"), m_oTransitionEntry)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"transitionEvaluation"), m_oTransitionEvaluation)
				WritingElement_ReadAttributes_End(oReader)
		}

		CHeaderFooterElement::CHeaderFooterElement()
		{
		}
		CHeaderFooterElement::~CHeaderFooterElement()
		{
		}
		void CHeaderFooterElement::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CHeaderFooterElement::toXML() const
		{
			return (L"");
		}
		void CHeaderFooterElement::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CHeaderFooterElement::toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
		{
			if (m_sText.empty()) return;

			writer.WriteString((L"<"));
			writer.WriteString(sName);
			writer.WriteString((L">"));

			writer.WriteEncodeXmlString(m_sText);

			writer.WriteString((L"</"));
			writer.WriteString(sName);
			writer.WriteString((L">"));
		}
		void CHeaderFooterElement::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			m_sText = oReader.GetText3();
		}
		EElementType CHeaderFooterElement::getType() const
		{
			return et_x_HeaderFooterElementWorksheet;
		}
		void CHeaderFooterElement::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_End(oReader)
		}

		CHeaderFooter::CHeaderFooter()
		{
		}
		CHeaderFooter::~CHeaderFooter()
		{
		}
		void CHeaderFooter::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CHeaderFooter::toXML() const
		{
			return (L"");
		}
		void CHeaderFooter::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<headerFooter"));
			WritingStringNullableAttrBool(L"alignWithMargins", m_oAlignWithMargins);
			WritingStringNullableAttrBool(L"differentFirst", m_oDifferentFirst);
			WritingStringNullableAttrBool(L"differentOddEven", m_oDifferentOddEven);
			WritingStringNullableAttrBool(L"scaleWithDoc", m_oScaleWithDoc);
			writer.WriteString((L">"));
			if (m_oOddHeader.IsInit())
			{
				m_oOddHeader->toXML2(writer, (L"oddHeader"));
			}
			if (m_oOddFooter.IsInit())
			{
				m_oOddFooter->toXML2(writer, (L"oddFooter"));
			}
			if (m_oEvenHeader.IsInit())
			{
				m_oEvenHeader->toXML2(writer, (L"evenHeader"));
			}
			if (m_oEvenFooter.IsInit())
			{
				m_oEvenFooter->toXML2(writer, (L"evenFooter"));
			}
			if (m_oFirstHeader.IsInit())
			{
				m_oFirstHeader->toXML2(writer, (L"firstHeader"));
			}
			if (m_oFirstFooter.IsInit())
			{
				m_oFirstFooter->toXML2(writer, (L"firstFooter"));
			}
			writer.WriteString((L"</headerFooter>"));
		}
		void CHeaderFooter::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ((L"evenFooter") == sName)
					m_oEvenFooter = oReader;
				else if ((L"evenHeader") == sName)
					m_oEvenHeader = oReader;
				else if ((L"firstFooter") == sName)
					m_oFirstFooter = oReader;
				else if ((L"firstHeader") == sName)
					m_oFirstHeader = oReader;
				else if ((L"oddFooter") == sName)
					m_oOddFooter = oReader;
				else if ((L"oddHeader") == sName)
					m_oOddHeader = oReader;
			}
		}
		void CHeaderFooter::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto pHEADERFOOTER = static_cast<XLSB::HEADERFOOTER*>(obj.get());
			if (pHEADERFOOTER == nullptr)
				return;

			ReadAttributes(pHEADERFOOTER->m_BrtBeginHeaderFooter);

			auto ptr = static_cast<XLSB::BeginHeaderFooter*>(pHEADERFOOTER->m_BrtBeginHeaderFooter.get());
			if (ptr != nullptr)
			{
				if (!ptr->stHeader.value().empty())
				{
					m_oOddHeader = new CHeaderFooterElement();
					m_oOddHeader->m_sText = ptr->stHeader.value();
				}
				if (!ptr->stFooter.value().empty())
				{
					m_oOddFooter = new CHeaderFooterElement();
					m_oOddFooter->m_sText = ptr->stFooter.value();
				}
				if (ptr->fHFDiffOddEven && !ptr->stHeaderEven.value().empty())
				{
					m_oEvenHeader = new CHeaderFooterElement();
					m_oEvenHeader->m_sText = ptr->stHeaderEven.value();
				}
				if (ptr->fHFDiffOddEven && !ptr->stFooterEven.value().empty())
				{
					m_oEvenFooter = new CHeaderFooterElement();
					m_oEvenFooter->m_sText = ptr->stFooterEven.value();
				}
				if (ptr->fHFDiffFirst && !ptr->stHeaderFirst.value().empty())
				{
					m_oFirstHeader = new CHeaderFooterElement();
					m_oFirstHeader->m_sText = ptr->stHeaderFirst.value();
				}
				if (ptr->fHFDiffFirst && !ptr->stFooterFirst.value().empty())
				{
					m_oFirstFooter = new CHeaderFooterElement();
					m_oFirstFooter->m_sText = ptr->stFooterFirst.value();
				}
			}
		}
		XLS::BaseObjectPtr CHeaderFooter::toBin()
		{
			auto ptr(new XLSB::HEADERFOOTER);

			XLS::BaseObjectPtr objectPtr(ptr);

			auto castedBegin(new XLSB::BeginHeaderFooter);
			ptr->m_BrtBeginHeaderFooter = XLS::BaseObjectPtr{castedBegin};

			if(m_oAlignWithMargins.IsInit())
				castedBegin->fHFAlignMargins = m_oAlignWithMargins->m_eValue;
			else
				castedBegin->fHFAlignMargins = false;
			if(m_oDifferentFirst.IsInit())
				castedBegin->fHFDiffFirst = m_oDifferentFirst->m_eValue;
			else
				castedBegin->fHFDiffFirst = false;
			if(m_oDifferentOddEven.IsInit())
				castedBegin->fHFDiffOddEven = m_oDifferentOddEven->m_eValue;
			else
				castedBegin->fHFDiffOddEven = false;
			if(m_oScaleWithDoc.IsInit())
				castedBegin->fHFScaleWithDoc = m_oScaleWithDoc->m_eValue;
			else
				castedBegin->fHFScaleWithDoc = false;

			if(m_oOddHeader.IsInit())
				castedBegin->stHeader = m_oOddHeader->m_sText;
			else
				castedBegin->stHeader = false;
			if(m_oOddFooter.IsInit())
				castedBegin->stFooter = m_oOddFooter->m_sText;
			else
				castedBegin->stFooter = false;
			if(m_oEvenHeader.IsInit())
				castedBegin->stHeaderEven = m_oEvenHeader->m_sText;
			else
				castedBegin->stHeaderEven = false;
			if(m_oEvenFooter.IsInit())
				castedBegin->stFooterEven = m_oEvenFooter->m_sText;
			else
				castedBegin->stFooterEven = false;
			if(m_oFirstHeader.IsInit())
				castedBegin->stHeaderFirst = m_oFirstHeader->m_sText;
			else
				castedBegin->stHeaderFirst = false;
			if(m_oFirstFooter.IsInit())
			 	castedBegin->stFooterFirst = m_oFirstFooter->m_sText;
			else
				castedBegin->stFooterFirst = false;
			return objectPtr;
		}
        void CHeaderFooter::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            {
                auto begin = writer->getNextRecord(XLSB::rt_BeginHeaderFooter);
                {
                    _UINT16 flags = 0;
                    if(m_oDifferentOddEven.IsInit())
                        SETBIT(flags, 0, m_oDifferentOddEven->GetValue());
                    if(m_oDifferentFirst.IsInit())
                        SETBIT(flags, 1, m_oDifferentFirst->GetValue());
                    if(m_oScaleWithDoc.IsInit())
                        SETBIT(flags, 2, m_oScaleWithDoc->GetValue());
                    if(m_oAlignWithMargins.IsInit())
                        SETBIT(flags, 3, m_oAlignWithMargins->GetValue());
                    *begin << flags;
                }
                {
                    XLSB::XLNullableWideString dataString;
                    if(m_oOddHeader.IsInit())
                        dataString = m_oOddHeader->m_sText;
                    else
                        dataString.setSize(0xFFFFFFFF);
                    *begin << dataString;
                }
                {
                    XLSB::XLNullableWideString dataString;
                    if(m_oOddFooter.IsInit())
                        dataString = m_oOddFooter->m_sText;
                    else
                        dataString.setSize(0xFFFFFFFF);
                    *begin << dataString;
                }
                {
                    XLSB::XLNullableWideString dataString;
                    if(m_oEvenHeader.IsInit())
                        dataString = m_oEvenHeader->m_sText;
                    else
                        dataString.setSize(0xFFFFFFFF);
                    *begin << dataString;
                }
                {
                    XLSB::XLNullableWideString dataString;
                    if(m_oEvenFooter.IsInit())
                        dataString = m_oEvenFooter->m_sText;
                    else
                        dataString.setSize(0xFFFFFFFF);
                    *begin << dataString;
                }
                {
                    XLSB::XLNullableWideString dataString;

                    if(m_oFirstHeader.IsInit())
                        dataString = m_oFirstHeader->m_sText;
                    else
                        dataString.setSize(0xFFFFFFFF);
                    *begin << dataString;
                }
                {
                    XLSB::XLNullableWideString dataString;
                    if(m_oFirstFooter.IsInit())
                        dataString = m_oFirstFooter->m_sText;
                    else
                        dataString.setSize(0xFFFFFFFF);
                    *begin << dataString;
                }
                writer->storeNextRecord(begin);
            }
            {
                auto end = writer->getNextRecord(XLSB::rt_EndHeaderFooter);
                writer->storeNextRecord(end);
            }
        }
		EElementType CHeaderFooter::getType() const
		{
			return et_x_HeaderFooterWorksheet;
		}
		void CHeaderFooter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"alignWithMargins"), m_oAlignWithMargins)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"differentFirst"), m_oDifferentFirst)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"differentOddEven"), m_oDifferentOddEven)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"scaleWithDoc"), m_oScaleWithDoc)
				WritingElement_ReadAttributes_End(oReader)
		}
		void CHeaderFooter::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BeginHeaderFooter*>(obj.get());
			if (ptr != nullptr)
			{
				m_oAlignWithMargins = ptr->fHFAlignMargins;
				m_oDifferentFirst = ptr->fHFDiffFirst;
				m_oDifferentOddEven = ptr->fHFDiffOddEven;
				m_oScaleWithDoc = ptr->fHFScaleWithDoc;
			}
		}

		CLegacyDrawingHFWorksheet::CLegacyDrawingHFWorksheet()
		{
		}
		CLegacyDrawingHFWorksheet::~CLegacyDrawingHFWorksheet()
		{
		}
		void CLegacyDrawingHFWorksheet::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CLegacyDrawingHFWorksheet::toXML() const
		{
			return (L"");
		}
		void CLegacyDrawingHFWorksheet::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (!m_oId.IsInit()) return;

			writer.WriteString(L"<legacyDrawingHF");
			WritingStringAttrString(L"r:id", m_oId->ToString());
			WritingStringNullableAttrInt(L"cfe", m_oCfe, m_oCfe->GetValue());
			WritingStringNullableAttrInt(L"cff", m_oCff, m_oCff->GetValue());
			WritingStringNullableAttrInt(L"cfo", m_oCfo, m_oCfo->GetValue());
			WritingStringNullableAttrInt(L"che", m_oChe, m_oChe->GetValue());
			WritingStringNullableAttrInt(L"chf", m_oChf, m_oChf->GetValue());
			WritingStringNullableAttrInt(L"cho", m_oCho, m_oCho->GetValue());
			WritingStringNullableAttrInt(L"lfe", m_oLfe, m_oLfe->GetValue());
			WritingStringNullableAttrInt(L"lff", m_oLff, m_oLff->GetValue());
			WritingStringNullableAttrInt(L"lfo", m_oLfo, m_oLfo->GetValue());
			WritingStringNullableAttrInt(L"lhe", m_oLhe, m_oLhe->GetValue());
			WritingStringNullableAttrInt(L"lLhf", m_oLhf, m_oLhf->GetValue());
			WritingStringNullableAttrInt(L"lho", m_oLho, m_oLho->GetValue());
			WritingStringNullableAttrInt(L"rfe", m_oRfe, m_oRfe->GetValue());
			WritingStringNullableAttrInt(L"rff", m_oRff, m_oRff->GetValue());
			WritingStringNullableAttrInt(L"rfo", m_oRfo, m_oRfo->GetValue());
			WritingStringNullableAttrInt(L"rhe", m_oRhe, m_oRhe->GetValue());
			WritingStringNullableAttrInt(L"rhf", m_oRhf, m_oRhf->GetValue());
			WritingStringNullableAttrInt(L"rho", m_oRho, m_oRho->GetValue());
			writer.WriteString(L"/>");

		}
		void CLegacyDrawingHFWorksheet::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CLegacyDrawingHFWorksheet::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CLegacyDrawingHFWorksheet::toBin()
		{
			auto castedPtr(new XLSB::LegacyDrawingHF);
			XLS::BaseObjectPtr ptr(castedPtr);
			if (m_oId.IsInit())
				castedPtr->stRelId.value = m_oId->GetValue();
			return ptr;
		}
        void CLegacyDrawingHFWorksheet::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_LegacyDrawingHF);
            XLSB::RelID stRelId;
            if(m_oId.IsInit())
                stRelId = m_oId->GetValue();
            else
                stRelId.value.setSize(0xFFFFFFFF);
            *record << stRelId;
            writer->storeNextRecord(record);
        }
		EElementType CLegacyDrawingHFWorksheet::getType() const
		{
			return et_x_LegacyDrawingHFWorksheet;
		}
		void CLegacyDrawingHFWorksheet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"r:id"), m_oId)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"relationships:id"), m_oId)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"cfe"), m_oCfe)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"cff"), m_oCff)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"cfo"), m_oCfo)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"che"), m_oChe)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"chf"), m_oChf)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"cho"), m_oCho)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"lfe"), m_oLfe)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"lff"), m_oLff)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"lfo"), m_oLfo)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"lhe"), m_oLhe)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"lhf"), m_oLhf)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"lho"), m_oLho)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"rfe"), m_oRfe)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"rff"), m_oRff)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"rfo"), m_oRfo)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"rhe"), m_oRhe)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"rhf"), m_oRhf)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"rho"), m_oRho)
				WritingElement_ReadAttributes_End(oReader)
		}
		void CLegacyDrawingHFWorksheet::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::LegacyDrawingHF*>(obj.get());
			if (ptr != nullptr)
				m_oId = ptr->stRelId.value.value();
		}

		CPictureWorksheet::CPictureWorksheet()
		{
		}
		CPictureWorksheet::~CPictureWorksheet()
		{
		}
		void CPictureWorksheet::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CPictureWorksheet::toXML() const
		{
			return (L"");
		}
		void CPictureWorksheet::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_oId.IsInit())
			{
				writer.WriteString(L"<picture");
				WritingStringAttrString(L"r:id", m_oId->ToString());
				writer.WriteString(L"/>");
			}

		}
		void CPictureWorksheet::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		XLS::BaseObjectPtr CPictureWorksheet::toBin()
		{
			auto ptr(new XLSB::BkHim);
			XLS::BaseObjectPtr objectPtr(ptr);

			if(m_oId.IsInit())
				ptr->rgb.value = m_oId->GetValue();
			else
				ptr->rgb.value.setSize(0XFFFFFFFF);

			return objectPtr;
		}
        void CPictureWorksheet::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_BkHim);
            XLSB::RelID stRelId;
            if(m_oId.IsInit())
                stRelId = m_oId->GetValue();
            else
                stRelId.value.setSize(0xFFFFFFFF);
            *record << stRelId;
            writer->storeNextRecord(record);
        }
		void CPictureWorksheet::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CPictureWorksheet::getType() const
		{
			return et_x_PictureWorksheet;
		}
		void CPictureWorksheet::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BkHim*>(obj.get());
			if (ptr != nullptr)
			{
				if (!ptr->rgb.value.value().empty())
					m_oId = ptr->rgb.value.value();
			}
		}
		void CPictureWorksheet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				WritingElement_ReadAttributes_ReadSingle(oReader, L"id", m_oId)
				WritingElement_ReadAttributes_End_No_NS(oReader)
		}

		CBreak::CBreak()
		{
		}
		CBreak::~CBreak()
		{
		}
		void CBreak::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CBreak::toXML() const
		{
			return (L"");
		}
		void CBreak::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<brk"));
			WritingStringNullableAttrInt(L"id", m_oId, m_oId->GetValue());
			WritingStringNullableAttrBool(L"man", m_oMan);
			WritingStringNullableAttrInt(L"max", m_oMax, m_oMax->GetValue());
			WritingStringNullableAttrInt(L"min", m_oMin, m_oMin->GetValue());
			WritingStringNullableAttrBool(L"pt", m_oPt);
			writer.WriteString((L"/>"));
		}
		void CBreak::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		XLS::BaseObjectPtr CBreak::toBin()
		{
			auto ptr(new XLSB::Brk);
			XLS::BaseObjectPtr objectPtr(ptr);

			if(m_oId.IsInit())
				ptr->unRwCol = m_oId->GetValue();
			if(m_oMan.IsInit())
				ptr->fMan = m_oMan->GetValue();
            else
                ptr->fMan = false;
            if(m_oMin.IsInit())
                ptr->unColRwStrt = m_oMin->GetValue();
            else
                ptr->unColRwStrt  = 0;
            if(m_oMax.IsInit())
                ptr->unColRwEnd = m_oMax->GetValue();
            else
                ptr->unColRwEnd = 0;
			if(m_oPt.IsInit())
				ptr->fPivot = m_oPt->GetValue();
            else
                ptr->fPivot = false;

			return objectPtr;
		}
        void CBreak::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_Brk);
            _UINT32 unRwCol = 0;
            if(m_oId.IsInit())
                unRwCol = m_oId->GetValue();
            _UINT32 unColRwStrt = 0;
            if(m_oMin.IsInit())
                unColRwStrt = m_oMin->GetValue();
            _UINT32 unColRwEnd = 0;
            if(m_oMax.IsInit())
                unColRwEnd = m_oMax->GetValue();
            _UINT32 fMan = 0;
            if(m_oMan.IsInit())
                fMan = m_oMan->GetValue();
            _UINT32 fPivot = 0;
            if(m_oPt.IsInit())
                fPivot = m_oPt->GetValue();
            *record << unRwCol << unColRwStrt << unColRwEnd << fMan << fPivot;
            writer->storeNextRecord(record);
        }
		void CBreak::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CBreak::getType() const
		{
			return et_x_Break;
		}
		void CBreak::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::Brk*>(obj.get());
			if (ptr != nullptr)
			{
				m_oId = ptr->unRwCol;
				m_oMan = ptr->fMan;
				m_oMax = ptr->unColRwEnd;
				m_oMin = ptr->unColRwStrt;
				m_oPt = ptr->fPivot;
			}
		}
		void CBreak::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"id"), m_oId)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"man"), m_oMan)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"max"), m_oMax)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"min"), m_oMin)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"pt"), m_oPt)
			WritingElement_ReadAttributes_End(oReader)
		}

		CRowColBreaks::CRowColBreaks()
		{
		}
		CRowColBreaks::~CRowColBreaks()
		{
		}
		void CRowColBreaks::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CRowColBreaks::toXML() const
		{
			return L"";
		}
		void CRowColBreaks::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXML2(writer, L"rowBreaks");
		}
		void CRowColBreaks::toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sName);
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			WritingStringNullableAttrInt(L"manualBreakCount", m_oManualBreakCount, m_oManualBreakCount->GetValue());
			writer.WriteString(L">");
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</");
			writer.WriteString(sName);
			writer.WriteString(L">");
		}
		void CRowColBreaks::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"brk" == sName)
				{
					CBreak* pBreak = new CBreak();
					*pBreak = oReader;
					m_arrItems.push_back(pBreak);
				}
			}
		}
		XLS::BaseObjectPtr CRowColBreaks::toBinRow()
		{
			auto ptr(new XLSB::RWBRK);
			XLS::BaseObjectPtr objectPtr(ptr);
			auto rowPtr(new XLSB::BeginRwBrk);
			ptr->m_BrtBeginRwBrk = XLS::BaseObjectPtr{rowPtr};
			if(m_oCount.IsInit())
				rowPtr->ibrkMac = m_oCount->GetValue();
			else
				rowPtr->ibrkMac = m_arrItems.size();
			if(m_oManualBreakCount.IsInit())
				rowPtr->ibrkManMac = m_oManualBreakCount->GetValue();
			else
				rowPtr->ibrkManMac = rowPtr->ibrkMac;
			for(auto i:m_arrItems)
			{
				ptr->m_arBrtBrk.push_back(i->toBin());
			}
			return objectPtr;
		}

		XLS::BaseObjectPtr CRowColBreaks::toBinColumn()
		{
			auto ptr(new XLSB::COLBRK);
			XLS::BaseObjectPtr objectPtr(ptr);
			auto colPtr(new XLSB::BeginColBrk);
			ptr->m_BrtBeginColBrk = XLS::BaseObjectPtr{colPtr};
			if(m_oCount.IsInit())
				colPtr->ibrkMac = m_oCount->GetValue();
			if(m_oManualBreakCount.IsInit())
				colPtr->ibrkManMac = m_oManualBreakCount->GetValue();
			for(auto i:m_arrItems)
			{
				ptr->m_arBrtBrk.push_back(i->toBin());
			}
			return objectPtr;
		}
        void CRowColBreaks::toBinRow(XLS::StreamCacheWriterPtr& writer)
        {
            {
                auto begin = writer->getNextRecord(XLSB::rt_BeginRwBrk);
                _UINT32 ibrkMac = 0;
                if(m_oCount.IsInit())
                    ibrkMac = m_oCount->GetValue();
                _UINT32 ibrkManMac = 0;
                if(m_oManualBreakCount.IsInit())
                    ibrkManMac = m_oManualBreakCount->GetValue();
                *begin << ibrkMac << ibrkManMac;
                writer->storeNextRecord(begin);
            }
            for(auto i:m_arrItems)
                i->toBin(writer);
            {
                auto end = writer->getNextRecord(XLSB::rt_EndRwBrk);
                writer->storeNextRecord(end);
            }
        }
        void CRowColBreaks::toBinColumn(XLS::StreamCacheWriterPtr& writer)
        {
            {
                auto begin = writer->getNextRecord(XLSB::rt_BeginColBrk);
                _UINT32 ibrkMac = 0;
                if(m_oCount.IsInit())
                    ibrkMac = m_oCount->GetValue();
                _UINT32 ibrkManMac = 0;
                if(m_oManualBreakCount.IsInit())
                    ibrkManMac = m_oManualBreakCount->GetValue();
                *begin << ibrkMac << ibrkManMac;
                writer->storeNextRecord(begin);
            }
            for(auto i:m_arrItems)
                i->toBin(writer);
            {
                auto end = writer->getNextRecord(XLSB::rt_EndColBrk);
                writer->storeNextRecord(end);
            }
        }

		void CRowColBreaks::fromBin(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typeRWBRK)
			{
				auto ptr = static_cast<XLSB::RWBRK*>(obj.get());
				if (ptr != nullptr)
				{
					ReadAttributes(ptr->m_BrtBeginRwBrk);
					for (auto &brk : ptr->m_arBrtBrk)
						m_arrItems.push_back(new CBreak(brk));
				}
			}

			else if (obj->get_type() == XLS::typeCOLBRK)
			{
				auto ptr = static_cast<XLSB::COLBRK*>(obj.get());
				if (ptr != nullptr)
				{
					ReadAttributes(ptr->m_BrtBeginColBrk);
					for (auto &brk : ptr->m_arBrtBrk)
						m_arrItems.push_back(new CBreak(brk));
				}
			}
		}
		EElementType CRowColBreaks::getType() const
		{
			return et_x_RowColBreaks;
		}
		void CRowColBreaks::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typeBeginRwBrk)
			{
				auto ptr = static_cast<XLSB::BeginRwBrk*>(obj.get());
				if (ptr != nullptr)
				{
					m_oCount = ptr->ibrkMac;
					m_oManualBreakCount = ptr->ibrkManMac;
				}
			}

			else if (obj->get_type() == XLS::typeBeginColBrk)
			{
				auto ptr = static_cast<XLSB::BeginColBrk*>(obj.get());
				if (ptr != nullptr)
				{
					m_oCount = ptr->ibrkMac;
					m_oManualBreakCount = ptr->ibrkManMac;
				}
			}
		}
		void CRowColBreaks::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"count", m_oCount)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"manualBreakCount", m_oManualBreakCount)
				WritingElement_ReadAttributes_End(oReader)
		}

		CSheetProtection::CSheetProtection()
		{
		}
		CSheetProtection::~CSheetProtection()
		{
		}
		void CSheetProtection::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSheetProtection::toXML() const
		{
			return L"";
		}
		void CSheetProtection::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<sheetProtection");
			WritingStringNullableAttrString(L"password", m_oPassword, m_oPassword.get());
			WritingStringNullableAttrString(L"algorithmName", m_oAlgorithmName, m_oAlgorithmName->ToString());
			WritingStringNullableAttrString(L"hashValue", m_oHashValue, m_oHashValue.get());
			WritingStringNullableAttrString(L"saltValue", m_oSaltValue, m_oSaltValue.get());
			WritingStringNullableAttrInt(L"spinCount", m_oSpinCount, m_oSpinCount->GetValue());
			WritingStringNullableAttrInt(L"autoFilter", m_oAutoFilter, m_oAutoFilter->GetValue());
			WritingStringNullableAttrInt(L"content", m_oContent, m_oContent->GetValue());
			WritingStringNullableAttrInt(L"deleteColumns", m_oDeleteColumns, m_oDeleteColumns->GetValue());
			WritingStringNullableAttrInt(L"deleteRows", m_oDeleteRows, m_oDeleteRows->GetValue());
			WritingStringNullableAttrInt(L"formatCells", m_oFormatCells, m_oFormatCells->GetValue());
			WritingStringNullableAttrInt(L"formatColumns", m_oFormatColumns, m_oFormatColumns->GetValue());
			WritingStringNullableAttrInt(L"formatRows", m_oFormatRows, m_oFormatRows->GetValue());
			WritingStringNullableAttrInt(L"insertColumns", m_oInsertColumns, m_oInsertColumns->GetValue());
			WritingStringNullableAttrInt(L"insertHyperlinks", m_oInsertHyperlinks, m_oInsertHyperlinks->GetValue());
			WritingStringNullableAttrInt(L"insertRows", m_oInsertRows, m_oInsertRows->GetValue());
			WritingStringNullableAttrInt(L"objects", m_oObjects, m_oObjects->GetValue());
			WritingStringNullableAttrInt(L"pivotTables", m_oPivotTables, m_oPivotTables->GetValue());
			WritingStringNullableAttrInt(L"scenarios", m_oScenarios, m_oScenarios->GetValue());
			WritingStringNullableAttrInt(L"selectLockedCells", m_oSelectLockedCells, m_oSelectLockedCells->GetValue());
			WritingStringNullableAttrInt(L"selectUnlockedCells", m_oSelectUnlockedCells, m_oSelectUnlockedCells->GetValue());
			WritingStringNullableAttrInt(L"sheet", m_oSheet, m_oSheet->GetValue());
			WritingStringNullableAttrInt(L"sort", m_oSort, m_oSort->GetValue());
			writer.WriteString(L"/>");
		}
		void CSheetProtection::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CSheetProtection::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CSheetProtection::toBin()
		{
			if(m_oPassword.IsInit())
			{
				auto ptr(new XLSB::SheetProtection);
				XLS::BaseObjectPtr castedPtr(ptr);

				if (m_oPassword.IsInit())
                    ptr->protpwd = std::stoul(m_oPassword.get(),nullptr, 16);
				else	
					ptr->protpwd = 0;

				if (m_oAutoFilter.IsInit())
					ptr->fAutoFilter = !m_oAutoFilter->GetValue();
				else
					ptr->fAutoFilter = true;

				if (m_oDeleteColumns.IsInit())
					ptr->fDeleteColumns = !m_oDeleteColumns->GetValue();
				else
					ptr->fDeleteColumns= true;

				if (m_oDeleteRows.IsInit())
					ptr->fDeleteRows = !m_oDeleteRows->GetValue();
				else
					ptr->fDeleteRows = true;

				if (m_oFormatCells.IsInit())
					ptr->fFormatCells = !m_oFormatCells->GetValue();
				else
					ptr->fFormatCells = true;

				if (m_oFormatColumns.IsInit())
					ptr->fFormatColumns = !m_oFormatColumns->GetValue();
				else
					ptr->fFormatColumns = true;

				if (m_oFormatRows.IsInit())
					ptr->fFormatRows = !m_oFormatRows->GetValue();
				else
					ptr->fFormatRows = true;

				if (m_oInsertColumns.IsInit())
					ptr->fInsertColumns = !m_oInsertColumns->GetValue();
				else
					ptr->fInsertColumns = true;

				if (m_oInsertHyperlinks.IsInit())
					ptr->fInsertHyperlinks = !m_oInsertHyperlinks->GetValue();
				else
					ptr->fInsertHyperlinks = true;

				if (m_oInsertRows.IsInit())
					ptr->fInsertRows = !m_oInsertRows->GetValue();
				else
					ptr->fInsertRows = true;

				if (m_oObjects.IsInit())
					ptr->fObjects = !m_oObjects->GetValue();
				else
					ptr->fObjects = true;

				if (m_oPivotTables.IsInit())
					ptr->fPivotTables = !m_oPivotTables->GetValue();
				else
					ptr->fPivotTables = true;

				if (m_oScenarios.IsInit())
					ptr->fScenarios = !m_oScenarios->GetValue();
				else
					ptr->fScenarios = true;

				if (m_oSelectLockedCells.IsInit())
					ptr->fSelLockedCells = !m_oSelectLockedCells->GetValue();
				else
					ptr->fSelLockedCells = true;

				if (m_oSelectUnlockedCells.IsInit())
					ptr->fSelUnlockedCells = !m_oSelectUnlockedCells->GetValue();
				else
					ptr->fSelUnlockedCells = true;

				if (m_oSheet.IsInit())
					ptr->fLocked = m_oSheet->GetValue();
				else
					ptr->fLocked = false;

				if (m_oSort.IsInit())
					ptr->fSort = !m_oSort->GetValue();
				else
					ptr->fSort = true;

				return castedPtr;
			}
			else
			{
				auto ptr(new XLSB::SheetProtectionIso);
				XLS::BaseObjectPtr castedPtr(ptr);
				if(m_oAlgorithmName.IsInit())
                    ptr->ipdPasswordData.szAlgName = m_oAlgorithmName->ToString();
				if(m_oSpinCount.IsInit())
					ptr->dwSpinCount = m_oSpinCount->GetValue();
				if(m_oHashValue.IsInit())
				{
					BYTE * temp;
					auto tempSize = 0;
					NSFile::CBase64Converter::CBase64Converter::Decode(std::string{m_oHashValue.get().begin(),
						m_oHashValue.get().end()}.c_str(), m_oHashValue.get().size(), temp, tempSize);
					ptr->ipdPasswordData.rgbHash.cbLength = tempSize;
					ptr->ipdPasswordData.rgbHash.rgbData = std::vector<BYTE>(temp, temp + tempSize);
					delete[] temp; 
				}

				if(m_oSaltValue.IsInit())
				{
                    BYTE * temp;
                    auto tempSize = 0;
					NSFile::CBase64Converter::Decode(std::string{m_oSaltValue.get().begin(),
					m_oSaltValue.get().end()}.c_str(), m_oSaltValue.get().size(), temp, tempSize);
					ptr->ipdPasswordData.rgbSalt.cbLength = tempSize;
					ptr->ipdPasswordData.rgbSalt.rgbData = std::vector<BYTE>(temp, temp + tempSize);
					delete[] temp; 
				}

				if (m_oAutoFilter.IsInit())
					ptr->fAutoFilter = !m_oAutoFilter->GetValue();
				else
					ptr->fAutoFilter = true;

				if (m_oDeleteColumns.IsInit())
					ptr->fDeleteColumns = !m_oDeleteColumns->GetValue();
				else
					ptr->fDeleteColumns= true;

				if (m_oDeleteRows.IsInit())
					ptr->fDeleteRows = !m_oDeleteRows->GetValue();
				else
					ptr->fDeleteRows = true;

				if (m_oFormatCells.IsInit())
					ptr->fFormatCells = !m_oFormatCells->GetValue();
				else
					ptr->fFormatCells = true;

				if (m_oFormatColumns.IsInit())
					ptr->fFormatColumns = !m_oFormatColumns->GetValue();
				else
					ptr->fFormatColumns = true;

				if (m_oFormatRows.IsInit())
					ptr->fFormatRows = !m_oFormatRows->GetValue();
				else
					ptr->fFormatRows = true;

				if (m_oInsertColumns.IsInit())
					ptr->fInsertColumns = !m_oInsertColumns->GetValue();
				else
					ptr->fInsertColumns = true;

				if (m_oInsertHyperlinks.IsInit())
					ptr->fInsertHyperlinks = !m_oInsertHyperlinks->GetValue();
				else
					ptr->fInsertHyperlinks = true;

				if (m_oInsertRows.IsInit())
					ptr->fInsertRows = !m_oInsertRows->GetValue();
				else
					ptr->fInsertRows = true;

				if (m_oObjects.IsInit())
					ptr->fObjects = !m_oObjects->GetValue();
				else
					ptr->fObjects = true;

				if (m_oPivotTables.IsInit())
					ptr->fPivotTables = !m_oPivotTables->GetValue();
				else
					ptr->fPivotTables = true;

				if (m_oScenarios.IsInit())
					ptr->fScenarios = !m_oScenarios->GetValue();
				else
					ptr->fScenarios = true;

				if (m_oSelectLockedCells.IsInit())
					ptr->fSelLockedCells = !m_oSelectLockedCells->GetValue();
				else
					ptr->fSelLockedCells = true;

				if (m_oSelectUnlockedCells.IsInit())
					ptr->fSelUnlockedCells = !m_oSelectUnlockedCells->GetValue();
				else
					ptr->fSelUnlockedCells = true;

				if (m_oSheet.IsInit())
					ptr->fLocked = m_oSheet->GetValue();
				else
					ptr->fLocked = false;

				if (m_oSort.IsInit())
					ptr->fSort = !m_oSort->GetValue();
				else
					ptr->fSort = true;

				return castedPtr;
			}
		}
        void CSheetProtection::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            XLS::CFRecordPtr record;
            unsigned char *flagBuf = 0;
            if(m_oSpinCount.IsInit() || m_oHashValue.IsInit() || m_oSaltValue.IsInit())
            {
                record = writer->getNextRecord(XLSB::rt_SheetProtectionIso);
                _UINT32 spinCount = 0;
                if(m_oSpinCount.IsInit())
                    spinCount = m_oSpinCount->GetValue();
                *record << spinCount;
            }
            else
            {
                record = writer->getNextRecord(XLSB::rt_SheetProtection);
                _UINT16 protPwd = 0;
                if(m_oPassword.IsInit())
                    protPwd = std::stoul(m_oPassword.get(),nullptr, 16);
                *record <<  protPwd;
            }
            {
                _UINT32 flag = 0;
                if(m_oSheet.IsInit())
                    flag = m_oSheet->GetValue();
                *record << flag;
                if(!flag)
                    record->reserveNunBytes(60);
                else
                {
                    if(m_oObjects.IsInit())
                        flag = !m_oObjects->GetValue();
                    *record << flag;
                    flag = 1;
                    if(m_oScenarios.IsInit())
                        flag = !m_oScenarios->GetValue();
                    *record << flag;
                    flag = 0;
                    if(m_oFormatCells.IsInit())
                        flag = !m_oFormatCells->GetValue();
                    *record << flag;
                    flag = 0;
                    if(m_oFormatColumns.IsInit())
                        flag = !m_oFormatColumns->GetValue();
                    *record << flag;
                    flag = 0;
                    if(m_oFormatRows.IsInit())
                        flag = !m_oFormatRows->GetValue();
                    *record << flag;
                    flag = 0;
                    if(m_oInsertColumns.IsInit())
                        flag = !m_oInsertColumns->GetValue();
                    *record << flag;
                    flag = 0;
                    if(m_oInsertRows.IsInit())
                        flag = !m_oInsertRows->GetValue();
                    *record << flag;
                    flag = 0;
                    if(m_oInsertHyperlinks.IsInit())
                        flag = !m_oInsertHyperlinks->GetValue();
                    *record << flag;
                    flag = 0;
                    if(m_oDeleteColumns.IsInit())
                        flag = !m_oDeleteColumns->GetValue();
                    *record << flag;
                    flag = 0;
                    if(m_oDeleteRows.IsInit())
                        flag = !m_oDeleteRows->GetValue();
                    *record << flag;
                    flag = 1;
                    if(m_oSelectLockedCells.IsInit())
                        flag = !m_oSelectLockedCells->GetValue();
                    *record << flag;
                    flag = 0;
                    if(m_oSort.IsInit())
                        flag = !m_oSort->GetValue();
                    *record << flag;
                    flag = 0;
                    if(m_oAutoFilter.IsInit())
                        flag = !m_oAutoFilter->GetValue();
                    *record << flag;
                     flag = 0;
                    if(m_oPivotTables.IsInit())
                        flag = !m_oPivotTables->GetValue();
                    *record << flag;
                    flag = 1;
                    if(m_oSelectUnlockedCells.IsInit())
                        flag = !m_oSelectUnlockedCells->GetValue();
                    *record << flag;
                }

            }
            if(record->getTypeId() == XLSB::rt_SheetProtectionIso)
            {
                {
                    XLSB::IsoPasswordData ipdPasswordData;
                    if(m_oHashValue.IsInit())
                    {
                        BYTE * temp;
                        auto tempSize = 0;
                        NSFile::CBase64Converter::CBase64Converter::Decode(std::string{m_oHashValue.get().begin(),
                            m_oHashValue.get().end()}.c_str(), m_oHashValue.get().size(), temp, tempSize);
                        ipdPasswordData.rgbHash.cbLength = tempSize;
                        ipdPasswordData.rgbHash.rgbData = std::vector<BYTE>(temp, temp + tempSize);
                        delete[] temp;
                    }

                    if(m_oSaltValue.IsInit())
                    {
                        BYTE * temp;
                        auto tempSize = 0;
                        NSFile::CBase64Converter::Decode(std::string{m_oSaltValue.get().begin(),
                        m_oSaltValue.get().end()}.c_str(), m_oSaltValue.get().size(), temp, tempSize);
                        ipdPasswordData.rgbSalt.cbLength = tempSize;
                        ipdPasswordData.rgbSalt.rgbData = std::vector<BYTE>(temp, temp + tempSize);
                        delete[] temp;
                    }
                    if(m_oAlgorithmName.IsInit())
                        ipdPasswordData.szAlgName = m_oAlgorithmName->ToString();
                    else
                        ipdPasswordData.szAlgName = L"";
                    *record << ipdPasswordData;
                }
                flagBuf = new unsigned char[64];
                auto RecordData = record->getData();
                std::memcpy(flagBuf, RecordData + 4, 64);
            }
            if(record)
                writer->storeNextRecord(record);
            if(record)
                record.reset();
            if(flagBuf)
            {
                record = writer->getNextRecord(XLSB::rt_SheetProtection);
                _UINT16 protPwd = 0;
                *record <<protPwd;
                record->appendRawDataToStatic(flagBuf, 64);
                writer->storeNextRecord(record);
            }
        }
		XLS::BaseObjectPtr CSheetProtection::toBinCS()
		{
			XLS::BaseObjectPtr objectPtr;
			if(m_oPassword.IsInit())
			{
				auto ptr(new XLSB::CsProtection);
				objectPtr =  XLS::BaseObjectPtr{ptr};
                ptr->protpwd = std::stoul(m_oPassword.get(), nullptr, 16);
				if(m_oObjects.IsInit())
					ptr->fObjects  = m_oObjects->GetValue();
				else
					ptr->fObjects = false;
				if(m_oSheet.IsInit())
					ptr->fLocked = m_oSheet->GetValue();
				else
					ptr->fObjects = false;
			}
			else
			{
				auto ptr(new XLSB::CsProtectionIso);
				objectPtr =  XLS::BaseObjectPtr{ptr};
				
				if(m_oAlgorithmName.IsInit())
					ptr->ipdPasswordData.szAlgName = m_oAlgorithmName->ToString();
				else
					ptr->ipdPasswordData.szAlgName = false;
				if(m_oSpinCount.IsInit())
					ptr->dwSpinCount = m_oSpinCount->GetValue();
				else
					ptr->dwSpinCount = 0;
					
				if(m_oHashValue.IsInit())
				{
					BYTE * temp;
					auto tempSize = 0;
					NSFile::CBase64Converter::CBase64Converter::Decode(std::string{m_oHashValue.get().begin(),
						m_oHashValue.get().end()}.c_str(), m_oHashValue.get().size(), temp, tempSize);
					ptr->ipdPasswordData.rgbHash.cbLength = tempSize;
					ptr->ipdPasswordData.rgbHash.rgbData = std::vector<BYTE>(temp, temp + tempSize);
					delete[] temp; 
				}

				if(m_oSaltValue.IsInit())
				{
					BYTE * temp;
					auto tempSize = 0;
					NSFile::CBase64Converter::Decode(std::string{m_oSaltValue.get().begin(),
					m_oSaltValue.get().end()}.c_str(), m_oSaltValue.get().size(), temp, tempSize);
					ptr->ipdPasswordData.rgbSalt.cbLength = tempSize;
					ptr->ipdPasswordData.rgbSalt.rgbData = std::vector<BYTE>(temp, temp + tempSize);
					delete[] temp; 
				}

				if(m_oObjects.IsInit())
					ptr->fObjects  = m_oObjects->GetValue();
				else
					ptr->fObjects = false;
				if(m_oSheet.IsInit())
					ptr->fLocked = m_oSheet->GetValue();
				else
					ptr->fObjects = false;
				
			}

			return objectPtr;
		}
        void CSheetProtection::toBinCS(XLS::StreamCacheWriterPtr& writer)
        {
            XLS::CFRecordPtr record;
            bool iso = false;
            if(m_oSpinCount.IsInit() || m_oHashValue.IsInit() || m_oSaltValue.IsInit())
            {
                record = writer->getNextRecord(XLSB::rt_CsProtectionIso);
                _UINT32 spinCount = 0;
                if(m_oSpinCount.IsInit())
                    spinCount = m_oSpinCount->GetValue();
                *record << spinCount;
            }
            else
            {
                record = writer->getNextRecord(XLSB::rt_CsProtection);
                _UINT16 protPwd = 0;
                if(m_oPassword.IsInit())
                    protPwd = std::stoul(m_oPassword.get(),nullptr, 16);
                *record <<  protPwd;
            }

            {
                _UINT32 flag = 0;
                if(m_oContent.IsInit())
                    flag = m_oContent->GetValue();
                *record << flag;
                if(!flag)
                    record->reserveNunBytes(4);
                else if(m_oObjects.IsInit())
                {
                    flag = m_oObjects->GetValue();
                    *record << flag;
                }
             }

            if(record->getTypeId() == XLSB::rt_CsProtectionIso)
            {
                {
                    XLSB::IsoPasswordData ipdPasswordData;
                    if(m_oHashValue.IsInit())
                    {
                        BYTE * temp;
                        auto tempSize = 0;
                        NSFile::CBase64Converter::CBase64Converter::Decode(std::string{m_oHashValue.get().begin(),
                            m_oHashValue.get().end()}.c_str(), m_oHashValue.get().size(), temp, tempSize);
                        ipdPasswordData.rgbHash.cbLength = tempSize;
                        ipdPasswordData.rgbHash.rgbData = std::vector<BYTE>(temp, temp + tempSize);
                        delete[] temp;
                    }

                    if(m_oSaltValue.IsInit())
                    {
                        BYTE * temp;
                        auto tempSize = 0;
                        NSFile::CBase64Converter::Decode(std::string{m_oSaltValue.get().begin(),
                        m_oSaltValue.get().end()}.c_str(), m_oSaltValue.get().size(), temp, tempSize);
                        ipdPasswordData.rgbSalt.cbLength = tempSize;
                        ipdPasswordData.rgbSalt.rgbData = std::vector<BYTE>(temp, temp + tempSize);
                        delete[] temp;
                    }
                    if(m_oAlgorithmName.IsInit())
                        ipdPasswordData.szAlgName = m_oAlgorithmName->ToString();
                    else
                        ipdPasswordData.szAlgName = L"";
                    *record << ipdPasswordData;
                }
                iso = true;
            }
            if(record)
                writer->storeNextRecord(record);
            if(record)
                record.reset();
            if(iso)
            {
                record = writer->getNextRecord(XLSB::rt_CsProtection);
                _UINT16 protPwd = 0;
                *record <<protPwd;
                {
                    _UINT32 flag = 0;
                    if(m_oContent.IsInit())
                        flag = m_oContent->GetValue();
                    *record << flag;
                    if(!flag)
                        record->reserveNunBytes(4);
                    else if(m_oObjects.IsInit())
                    {
                        flag = m_oObjects->GetValue();
                        *record << flag;
                    }
                }
                writer->storeNextRecord(record);
            }
        }
		EElementType CSheetProtection::getType() const
		{
			return et_x_SheetProtection;
		}
		void CSheetProtection::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"password"), m_oPassword)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"algorithmName"), m_oAlgorithmName)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"hashValue"), m_oHashValue)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"saltValue"), m_oSaltValue)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"spinCount"), m_oSpinCount)

				WritingElement_ReadAttributes_Read_else_if(oReader, (L"autoFilter"), m_oAutoFilter)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"content"), m_oContent)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"deleteColumns"), m_oDeleteColumns)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"deleteRows"), m_oDeleteRows)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"formatCells"), m_oFormatCells)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"formatColumns"), m_oFormatColumns)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"formatRows"), m_oFormatRows)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"insertColumns"), m_oInsertColumns)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"insertHyperlinks"), m_oInsertHyperlinks)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"insertRows"), m_oInsertRows)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"objects"), m_oObjects)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"pivotTables"), m_oPivotTables)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"scenarios"), m_oScenarios)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"selectLockedCells"), m_oSelectLockedCells)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"selectUnlockedCells"), m_oSelectUnlockedCells)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"sheet"), m_oSheet)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"sort"), m_oSort)
				WritingElement_ReadAttributes_End(oReader)
		}
		void CSheetProtection::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			if (obj->get_type() == XLS::typeSheetProtection)
			{
				auto ptr = static_cast<XLSB::SheetProtection*>(obj.get());
				if (ptr != nullptr)
				{
					std::wstringstream hexStream;
					hexStream << std::hex << ptr->protpwd;
					m_oPassword = hexStream.str();
					m_oAutoFilter = (bool)ptr->fAutoFilter;
					m_oContent = true;
					m_oDeleteColumns = (bool)ptr->fDeleteColumns;
					m_oDeleteRows = (bool)ptr->fDeleteRows;
					m_oFormatCells = (bool)ptr->fFormatCells;
					m_oFormatColumns = (bool)ptr->fFormatColumns;
					m_oFormatRows = (bool)ptr->fFormatRows;
					m_oInsertColumns = (bool)ptr->fInsertColumns;
					m_oInsertHyperlinks = (bool)ptr->fInsertHyperlinks;
					m_oInsertRows = (bool)ptr->fInsertRows;
					m_oObjects = (bool)ptr->fObjects;
					m_oPivotTables = (bool)ptr->fPivotTables;
					m_oScenarios = (bool)ptr->fScenarios;
					m_oSelectLockedCells = (bool)ptr->fSelLockedCells;
					m_oSelectUnlockedCells = (bool)ptr->fSelUnlockedCells;
					m_oSheet = (bool)ptr->fLocked;
					m_oSort = (bool)ptr->fSort;
				}
			}
			else if (obj->get_type() == XLS::typeSheetProtectionIso)
			{
				auto ptr = static_cast<XLSB::SheetProtectionIso*>(obj.get());
				if (ptr != nullptr)
				{
					m_oAlgorithmName = ptr->ipdPasswordData.szAlgName.value();
					m_oSpinCount = ptr->dwSpinCount;
					m_oHashValue = ptr->ipdPasswordData.rgbHash.GetBase64();
					m_oSaltValue = ptr->ipdPasswordData.rgbSalt.GetBase64();
					m_oAutoFilter = (bool)ptr->fAutoFilter;
					m_oContent = true;
					m_oDeleteColumns = (bool)ptr->fDeleteColumns;
					m_oDeleteRows = (bool)ptr->fDeleteRows;
					m_oFormatCells = (bool)ptr->fFormatCells;
					m_oFormatColumns = (bool)ptr->fFormatColumns;
					m_oFormatRows = (bool)ptr->fFormatRows;
					m_oInsertColumns = (bool)ptr->fInsertColumns;
					m_oInsertHyperlinks = (bool)ptr->fInsertHyperlinks;
					m_oInsertRows = (bool)ptr->fInsertRows;
					m_oObjects = (bool)ptr->fObjects;
					m_oPivotTables = (bool)ptr->fPivotTables;
					m_oScenarios = (bool)ptr->fScenarios;
					m_oSelectLockedCells = (bool)ptr->fSelLockedCells;
					m_oSelectUnlockedCells = (bool)ptr->fSelUnlockedCells;
					m_oSheet = (bool)ptr->fLocked;
					m_oSort = (bool)ptr->fSort;
				}
			}
			else if (obj->get_type() == XLS::typeCsProtection)
			{
				auto ptr = static_cast<XLSB::CsProtection*>(obj.get());
				if (ptr != nullptr)
				{
					m_oPassword = std::to_wstring(ptr->protpwd);
					m_oObjects = (bool)ptr->fObjects;
					m_oSheet = (bool)ptr->fLocked;
				}
			}
			else if (obj->get_type() == XLS::typeCsProtectionIso)
			{
				auto ptr = static_cast<XLSB::CsProtectionIso*>(obj.get());
				if (ptr != nullptr)
				{
					m_oAlgorithmName = ptr->ipdPasswordData.szAlgName.value();
					m_oSpinCount = ptr->dwSpinCount;
					m_oHashValue = ptr->ipdPasswordData.rgbHash.GetBase64();
					m_oSaltValue = ptr->ipdPasswordData.rgbSalt.GetBase64();
					m_oObjects = (bool)ptr->fObjects;
					m_oSheet = (bool)ptr->fLocked;
				}
			}

		}

		CDataRef::CDataRef()
		{
		}
		CDataRef::~CDataRef()
		{
		}
		void CDataRef::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDataRef::toXML() const
		{
			return L"";
		}
		void CDataRef::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<sheetProtection");
			WritingStringAttrString(L"r:id", m_oId->ToString());
			WritingStringNullableAttrString(L"name", m_oName, m_oName.get());
			WritingStringNullableAttrString(L"ref", m_oRef, m_oRef.get());
			WritingStringNullableAttrString(L"sheet", m_oSheet, m_oSheet.get());
			writer.WriteString(L"/>");
		}
		void CDataRef::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CDataRef::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CDataRef::toBin()
		{
			auto ptr(new XLSB::DRef);
			XLS::BaseObjectPtr objectPtr(ptr);

			if (m_oId.IsInit())
				ptr->relId.value = m_oId->GetValue();
			else
				ptr->relId.value = L"";

			if (m_oName.IsInit())
				ptr->xstrName = m_oName.get();
            else
            {
				ptr->xstrName = L"";
                ptr->fName = false;
                ptr->fBuiltin = false;
            }
			if (m_oRef.IsInit())
				ptr->rfx = m_oRef.get();

			if (m_oSheet.IsInit())
				ptr->xstrSheet = m_oSheet.get();
            else
                ptr->xstrSheet = L"";

			return objectPtr;
		}
        void CDataRef::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_DRef);
            BYTE fname = 0;
            {
                BYTE fbuiltin = 0;
                if (m_oName.IsInit() && m_oName->size())
                {
                    fname = 1;
                    std::set<std::wstring> defaultNames =
                    {
                        L"Consolidate_Area", L"Auto_Open", L"Auto_Close", L"Extract", L"Database", L"Criteria", L"Sheet_Title",
                        L"Print_Area", L"Print_Titles", L"Recorder", L"Data_Form", L"Auto_Activate", L"Auto_Deactivate", L"_FilterDatabase"
                    };
                    if(defaultNames.find(m_oName.get()) != defaultNames.end())
                    {
                        fbuiltin = 1;
                    }
                }
                *record << fname << fbuiltin;
            }
            if(!fname)
            {
                XLSB::UncheckedRfX ref;
                if (m_oRef.IsInit())
                    ref = m_oRef.get();
                 *record << ref;
            }
            else
            {
                _UINT32 gap = 1048576;
                *record << gap << gap;
                gap = 16384;
                *record << gap << gap;
            }
            {
                XLSB::XLWideString xstrName;
                XLSB::XLWideString xstrSheet;
                XLSB::XLNullableWideString relId;
                if(fname)
                    xstrName = m_oName.get();
                else
                    xstrName = L"";
                if (m_oSheet.IsInit())
                    xstrSheet = m_oSheet.get();
                else
                    xstrSheet = L"";
                if (m_oId.IsInit())
                    relId = m_oId->GetValue();
                else
                    relId = L"";
                *record << xstrName << xstrSheet << relId;
            }
            writer->storeNextRecord(record);
        }
		EElementType CDataRef::getType() const
		{
			return et_x_DataRef;
		}
		void CDataRef::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::DRef*>(obj.get());
			if (ptr != nullptr)
			{
				if (!ptr->relId.value.value().empty())
					m_oId = ptr->relId.value.value();

				if (!ptr->xstrName.value().empty())
					m_oName = ptr->xstrName.value();

				if (!ptr->rfx.toString().empty())
					m_oRef = ptr->rfx.toString(true, true);

				if (!ptr->xstrSheet.value().empty())
					m_oSheet = ptr->xstrSheet.value();
			}
		}
		void CDataRef::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"r:id"), m_oId)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"relationships:id"), m_oId)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"name"), m_oName)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"ref"), m_oRef)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"sheet"), m_oSheet)
				WritingElement_ReadAttributes_End(oReader)
		}

		CDataRefs::CDataRefs()
		{
		}
		CDataRefs::~CDataRefs()
		{
		}
		void CDataRefs::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDataRefs::toXML() const
		{
			return (L"");
		}
		void CDataRefs::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<dataRefs");
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			writer.WriteString(L">");
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</dataRefs>");
		}
		void CDataRefs::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"dataRef" == sName)
				{
					CDataRef* pDataRef = new CDataRef();
					*pDataRef = oReader;
					m_arrItems.push_back(pDataRef);
				}
			}
		}
		void CDataRefs::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::DREFS*>(obj.get());
			if (ptr != nullptr)
			{
				m_oCount = (_UINT32)ptr->m_arBrtDRef.size();

				for (auto &dref : ptr->m_arBrtDRef)
					m_arrItems.push_back(new CDataRef(dref));
			}
		}
		XLS::BaseObjectPtr CDataRefs::toBin()
		{
			auto ptr(new XLSB::DREFS);
			XLS::BaseObjectPtr objectPtr(ptr);
            auto beginRefs(new XLSB::BeginDRefs);
            ptr->m_BrtBeginDRefs = XLS::BaseObjectPtr{beginRefs};

			for(auto i:m_arrItems)
			{
				ptr->m_arBrtDRef.push_back(i->toBin());
			}
            beginRefs->cdref = ptr->m_arBrtDRef.size();
			return objectPtr;
		}
        void CDataRefs::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            {
                auto begin = writer->getNextRecord(XLSB::rt_BeginDRefs);
                _UINT32 size = m_arrItems.size();
                *begin << size;
                writer->storeNextRecord(begin);
            }
            for(auto i:m_arrItems)
                i->toBin(writer);
            {
                auto end = writer->getNextRecord(XLSB::rt_EndDRefs);
                writer->storeNextRecord(end);
            }
        }
		EElementType CDataRefs::getType() const
		{
			return et_x_DataRefs;
		}
		void CDataRefs::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_ReadSingle(oReader, L"count", m_oCount)
			WritingElement_ReadAttributes_End(oReader)
		}

		CDataConsolidate::CDataConsolidate()
		{
		}
		CDataConsolidate::~CDataConsolidate()
		{
		}
		void CDataConsolidate::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDataConsolidate::toXML() const
		{
			return L"";
		}
		void CDataConsolidate::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<dataConsolidate");
			WritingStringNullableAttrString(L"function", m_oFunction, m_oFunction->ToString());
			WritingStringNullableAttrInt(L"link", m_oLink, m_oLink->GetValue());
			WritingStringNullableAttrInt(L"startLabels", m_oStartLabels, m_oStartLabels->GetValue());
			WritingStringNullableAttrInt(L"topLabels", m_oTopLabels, m_oTopLabels->GetValue());
			writer.WriteString(L">");

			writer.WriteString(L"</dataConsolidate>");
		}
		void CDataConsolidate::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"dataRefs" == sName)
					m_oDataRefs = oReader;
			}
		}
		XLS::BaseObjectPtr CDataConsolidate::toBin()
		{
			auto ptr(new XLSB::DCON);
			XLS::BaseObjectPtr objectPtr(ptr);
			auto beginPtr(new XLSB::BeginDCon);
			ptr->m_BrtBeginDCon = XLS::BaseObjectPtr{beginPtr};

			if(m_oFunction.IsInit())
				beginPtr->iiftab = m_oFunction->GetValue();
            else
                beginPtr->iiftab = 0;
            if(m_oLink.IsInit())
				beginPtr->fLinkConsol = m_oLink->GetValue();
            else
                beginPtr->fLinkConsol = false;
            if(m_oStartLabels.IsInit())
				beginPtr->fLeftCat = m_oStartLabels->GetValue();
            else
                beginPtr->fLeftCat = false;
            if(m_oTopLabels.IsInit())
				beginPtr->fTopCat = m_oTopLabels->GetValue();
            else
                beginPtr->fTopCat = false;

			if(m_oDataRefs.IsInit())
			{
				ptr->m_DREFS = m_oDataRefs->toBin();
			}
			return objectPtr;
		}
        void CDataConsolidate::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            {
                auto begin = writer->getNextRecord(XLSB::rt_BeginDCon);
                BYTE flags = 0;
                BYTE iftab = 0;
                if(m_oFunction.IsInit())
                    iftab = m_oFunction->GetValue();
                if(m_oStartLabels.IsInit())
                    SETBIT(flags, 0, m_oStartLabels->GetValue())
                if(m_oTopLabels.IsInit())
                    SETBIT(flags, 1, m_oTopLabels->GetValue())
                if(m_oLink.IsInit())
                    SETBIT(flags, 2, m_oLink->GetValue())
                *begin << iftab << flags;
                writer->storeNextRecord(begin);
            }
            if(m_oDataRefs.IsInit())
                m_oDataRefs->toBin(writer);
            {
                auto end = writer->getNextRecord(XLSB::rt_EndDCon);

                writer->storeNextRecord(end);
            }
        }
		void CDataConsolidate::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::DCON*>(obj.get());
			if (ptr != nullptr)
			{
				ReadAttributes(ptr->m_BrtBeginDCon);
				if (ptr->m_DREFS != nullptr)
					m_oDataRefs = ptr->m_DREFS;
			}
		}
		EElementType CDataConsolidate::getType() const
		{
			return et_x_DataConsolidate;
		}
		void CDataConsolidate::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BeginDCon*>(obj.get());
			if (ptr != nullptr)
			{
				m_oFunction = (SimpleTypes::Spreadsheet::EDataConsolidateFunction)ptr->iiftab.value().get();
				m_oLink = ptr->fLinkConsol;
				m_oStartLabels = ptr->fLeftCat;
				m_oTopLabels = ptr->fTopCat;
			}
		}
		void CDataConsolidate::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"function"), m_oFunction)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"link"), m_oLink)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"startLabels"), m_oStartLabels)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"topLabels"), m_oTopLabels)
			WritingElement_ReadAttributes_End(oReader)
		}
//------------------------------------------------------------------------------------------------
		CUserProtectedRange::CUserProtectedRange()
		{
		}
		CUserProtectedRange::~CUserProtectedRange()
		{
		}
		void CUserProtectedRange::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CUserProtectedRange::toXML() const
		{
			return L"";
		}
		EElementType CUserProtectedRange::getType() const
		{
			return et_x_UserProtectedRange;
		}
		void CUserProtectedRange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"name", m_oName)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"sqref", m_oSqref)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"type", m_oType)
			WritingElement_ReadAttributes_End(oReader)
		}
		void CUserProtectedRange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"text" == sName)
				{
					m_oText = oReader.GetText2();
				}
				else if (L"users" == sName)
				{
					int nCurDepth1 = oReader.GetDepth();
					while (oReader.ReadNextSiblingNode(nCurDepth1))
					{
						std::wstring sName2 = XmlUtils::GetNameNoNS(oReader.GetName());
						if (L"user" == sName2)
						{
							_UsersGroupsDesc desc;
							WritingElement_ReadAttributes_Start(oReader)
								WritingElement_ReadAttributes_Read_if(oReader, L"id", desc.id)
								WritingElement_ReadAttributes_Read_else_if(oReader, L"name", desc.name)
								WritingElement_ReadAttributes_Read_else_if(oReader, L"type", desc.type)
							WritingElement_ReadAttributes_End(oReader)
							m_arUsers.push_back(desc);
						}
					}
				}
				else if (L"usersGroups" == sName)
				{
					int nCurDepth1 = oReader.GetDepth();
					while (oReader.ReadNextSiblingNode(nCurDepth1))
					{
						std::wstring sName2 = XmlUtils::GetNameNoNS(oReader.GetName());
						if (L"usersGroup" == sName2)
						{
							_UsersGroupsDesc desc;
							WritingElement_ReadAttributes_Start(oReader)
								WritingElement_ReadAttributes_Read_if(oReader, L"id", desc.id)
								WritingElement_ReadAttributes_Read_else_if(oReader, L"name", desc.name)
								WritingElement_ReadAttributes_Read_else_if(oReader, L"type", desc.type)
							WritingElement_ReadAttributes_End(oReader)
							m_arUsersGroups.push_back(desc);
						}
					}
				}
			}
		}
		void CUserProtectedRange::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<userProtectedRange");
			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
			WritingStringNullableAttrString(L"sqref", m_oSqref, *m_oSqref);
			WritingStringNullableAttrString(L"type", m_oType, m_oType->ToString())
			writer.WriteString(L">");

			if (m_oText.IsInit())
			{
				writer.WriteString(L"<text>");
				writer.WriteString(*m_oText);
				writer.WriteString(L"</text>");
			}
			if (false == m_arUsers.empty())
			{
				writer.WriteString(L"<users>");
				for (size_t i = 0; i < m_arUsers.size(); ++i)
				{
					writer.WriteString(L"<user");
					WritingStringNullableAttrEncodeXmlString2(L"id", m_arUsers[i].id);
					WritingStringNullableAttrEncodeXmlString2(L"name", m_arUsers[i].name);
					WritingStringNullableAttrString(L"type", m_arUsers[i].type, m_arUsers[i].type->ToString())
					writer.WriteString(L"/>");
				}
				writer.WriteString(L"</users>");
			}
			if (false == m_arUsersGroups.empty())
			{
				writer.WriteString(L"<usersGroups>");
				for (size_t i = 0; i < m_arUsersGroups.size(); ++i)
				{
					writer.WriteString(L"<usersGroup");
					WritingStringNullableAttrEncodeXmlString2(L"id", m_arUsersGroups[i].id);
					WritingStringNullableAttrEncodeXmlString2(L"name", m_arUsersGroups[i].name);
					WritingStringNullableAttrString(L"type", m_arUsersGroups[i].type, m_arUsersGroups[i].type->ToString())
					writer.WriteString(L"/>");
				}
				writer.WriteString(L"</usersGroups>");
			}
			writer.WriteString(L"</userProtectedRange>");
		}
		CUserProtectedRanges::CUserProtectedRanges()
		{
		}
		CUserProtectedRanges::~CUserProtectedRanges()
		{
		}
		void CUserProtectedRanges::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CUserProtectedRanges::toXML() const
		{
			return L"";
		}
		EElementType CUserProtectedRanges::getType() const
		{
			return et_x_UserProtectedRanges;
		}
		void CUserProtectedRanges::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"userProtectedRange" == sName)
				{
					CUserProtectedRange *newItem = new CUserProtectedRange();
					newItem->fromXML(oReader);
					m_arrItems.push_back(newItem);
				}
			}
		}
		void CUserProtectedRanges::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;

			writer.WriteString(L"<userProtectedRanges>");
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</userProtectedRanges>");
		}
	} //Spreadsheet
} // namespace OOX
