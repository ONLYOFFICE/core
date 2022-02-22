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

#include "../CommonInclude.h"
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
#include "../../XlsbFormat/Biff12_records/DRef.h"
#include "../../XlsbFormat/Biff12_unions/CSVIEWS.h"
#include "../../XlsbFormat/Biff12_unions/CSVIEW.h"
#include "../../XlsbFormat/Biff12_records/CsProp.h"
#include "../../XlsbFormat/Biff12_records/CsProtectionIso.h"
#include "../../XlsbFormat/Biff12_records/CsProtection.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CProtectedRange : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CProtectedRange)
            WritingElement_XlsbConstructors(CProtectedRange)
			CProtectedRange()
			{
			}
			virtual ~CProtectedRange()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType() const
			{
				return et_x_ProtectedRange;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                if(obj->get_type() == XLS::typeRangeProtectionIso)
                {
                    auto ptr = static_cast<XLSB::RangeProtectionIso*>(obj.get());
                    if(ptr != nullptr)
                    {
                        m_oSpinCount            = ptr->dwSpinCount;
                        m_oSqref                = ptr->sqRfX.strValue;

                        if(!ptr->ipdPasswordData.szAlgName.value().empty())
                            m_oAlgorithmName    = ptr->ipdPasswordData.szAlgName.value();

                        if(!ptr->rangeProtectionTitleSDRel.rgchTitle.value().empty())
                                m_oName         = ptr->rangeProtectionTitleSDRel.rgchTitle.value();

                        m_oHashValue            = std::wstring(ptr->ipdPasswordData.rgbHash.rgbData.begin(),
                                                                  ptr->ipdPasswordData.rgbHash.rgbData.end());
                        m_oSaltValue            = std::wstring(ptr->ipdPasswordData.rgbSalt.rgbData.begin(),
                                                                  ptr->ipdPasswordData.rgbSalt.rgbData.end());
                    }
                }

                else if(obj->get_type() == XLS::typeRangeProtection)
                {
                    auto ptr = static_cast<XLSB::RangeProtection*>(obj.get());
                    if(ptr != nullptr)
                    {
                        m_oSqref                = ptr->sqRfX.strValue;

                        if(!ptr->rangeProtectionTitleSDRel.rgchTitle.value().empty())
                                m_oName         = ptr->rangeProtectionTitleSDRel.rgchTitle.value();
                    }
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
			nullable<SimpleTypes::CCryptAlgoritmName<>>		m_oAlgorithmName;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oSpinCount;
			nullable_string				m_oHashValue;
			nullable_string				m_oSaltValue;
			nullable_string				m_oName;
			nullable_string				m_oSqref;
			std::vector<std::wstring>	m_arSecurityDescriptors;
		};
		class CProtectedRanges : public WritingElementWithChilds<CProtectedRange>
		{
		public:
			WritingElement_AdditionConstructors(CProtectedRanges)
            WritingElement_XlsbVectorConstructors(CProtectedRanges)
			CProtectedRanges()
			{
			}
			virtual ~CProtectedRanges()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if (L"protectedRange" == sName)
						m_arrItems.push_back(new CProtectedRange(oReader));
				}
			}

            void fromBin(std::vector<XLS::BaseObjectPtr>& obj)
            {
                for(auto &protRange : obj)
                    m_arrItems.push_back(new CProtectedRange(protRange));
            }

			virtual EElementType getType() const
			{
				return et_x_ProtectedRanges;
			}
		};
		class CPageMargins : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageMargins)
            WritingElement_XlsbConstructors(CPageMargins)
			CPageMargins()
			{
			}
			virtual ~CPageMargins()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oLeft.IsInit() || m_oTop.IsInit() || m_oRight.IsInit() || m_oBottom.IsInit() || m_oHeader.IsInit() || m_oFooter.IsInit())
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType () const
			{
				return et_x_PageMargins;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"left"),	m_oLeft)
                    WritingElement_ReadAttributes_Read_else_if	( oReader, (L"top"),	m_oTop)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"right"),	m_oRight)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"bottom"),	m_oBottom)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"header"),	m_oHeader)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"footer"),	m_oFooter)
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::Margins*>(obj.get());
                if(ptr != nullptr)
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

		public:
			nullable<SimpleTypes::CInch>	m_oLeft;
			nullable<SimpleTypes::CInch>	m_oTop;
			nullable<SimpleTypes::CInch>	m_oRight;
			nullable<SimpleTypes::CInch>	m_oBottom;
			nullable<SimpleTypes::CInch>	m_oHeader;
			nullable<SimpleTypes::CInch>	m_oFooter;
		};
		class CPageSetup : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageSetup)
            WritingElement_XlsbConstructors(CPageSetup)
			CPageSetup()
			{
			}
			virtual ~CPageSetup()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oPaperSize.IsInit() || m_oOrientation.IsInit())
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType () const
			{
				return et_x_PageSetup;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"blackAndWhite"),	m_oBlackAndWhite)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"cellComments"),	m_oCellComments)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"copies"),			m_oCopies)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"draft"),			m_oDraft)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"errors"),			m_oErrors)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"firstPageNumber"),m_oFirstPageNumber)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"fitToHeight"),	m_oFitToHeight)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"fitToWidth"),		m_oFitToWidth)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"horizontalDpi"),	m_oHorizontalDpi)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"r:id"),			m_oRId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"relationships:id"), m_oRId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"orientation"),	m_oOrientation)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"pageOrder"),		m_oPageOrder)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"paperHeight"),	m_oPaperHeight)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"paperSize"),		m_oPaperSize)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"paperWidth"),		m_oPaperWidth)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"paperUnits"),		m_oPaperUnits)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"scale"),			m_oScale)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"useFirstPageNumber"),	m_oUseFirstPageNumber)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"usePrinterDefaults"),	m_oUsePrinterDefaults)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"verticalDpi"),	m_oVerticalDpi)
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                if(obj->get_type() == XLS::typePageSetup)
                {
                    auto ptr = static_cast<XLSB::PageSetup*>(obj.get());
                    if(ptr != nullptr)
                    {
                        m_oBlackAndWhite        = ptr->fNoColor;
                        if(ptr->fNoColor)
                        {
                            if(ptr->fNotes)
                                m_oCellComments = SimpleTypes::Spreadsheet::ECellComments::cellcommentsAtEnd;
                            else
                                m_oCellComments = SimpleTypes::Spreadsheet::ECellComments::cellcommentsAsDisplayed;
                        }
                        else
                            m_oCellComments     = SimpleTypes::Spreadsheet::ECellComments::cellcommentsNone;

                        m_oCopies               = ptr->iCopies;
                        m_oDraft                = ptr->fDraft;
                        m_oErrors               = (SimpleTypes::Spreadsheet::EPrintError)ptr->iErrors;
                        m_oFirstPageNumber      = ptr->iPageStart;
                        m_oFitToHeight          = ptr->iFitHeight;
                        m_oFitToWidth           = ptr->iFitWidth;
                        m_oHorizontalDpi        = ptr->iRes;
                        m_oRId                  = ptr->szRelID;

                        if(ptr->fLandscape)
                            m_oOrientation      = SimpleTypes::EPageOrientation::pageorientLandscape;
                        else
                            m_oOrientation      = SimpleTypes::EPageOrientation::pageorientPortrait;

                        if(ptr->fLeftToRight)
                            m_oPageOrder        = SimpleTypes::Spreadsheet::EPageOrder::pageorderOverThenDown;
                        else
                            m_oPageOrder        = SimpleTypes::Spreadsheet::EPageOrder::pageorderDownThenOver;

                        m_oPaperSize            = (SimpleTypes::Spreadsheet::EPageSize)ptr->iPaperSize;
                        m_oScale                = ptr->iScale;
                        m_oUseFirstPageNumber   = ptr->fUsePage;
                        m_oVerticalDpi          = ptr->iVRes;

                    }
                }
                else if(obj->get_type() == XLS::typeCsPageSetup)
                {
                    auto ptr = static_cast<XLSB::CsPageSetup*>(obj.get());
                    if(ptr != nullptr)
                    {
                        m_oBlackAndWhite        = ptr->fNoColor;
                        if(ptr->fNoColor)
                        {
                            if(ptr->fNotes)
                                m_oCellComments = SimpleTypes::Spreadsheet::ECellComments::cellcommentsAtEnd;
                            else
                                m_oCellComments = SimpleTypes::Spreadsheet::ECellComments::cellcommentsAsDisplayed;
                        }
                        else
                            m_oCellComments     = SimpleTypes::Spreadsheet::ECellComments::cellcommentsNone;

                        m_oCopies               = ptr->iCopies;
                        m_oDraft                = ptr->fDraft;
                        m_oFirstPageNumber      = ptr->iPageStart;
                        m_oHorizontalDpi        = ptr->iRes;
                        m_oRId                  = ptr->szRelID;

                        if(ptr->fLandscape)
                            m_oOrientation      = SimpleTypes::EPageOrientation::pageorientLandscape;
                        else
                            m_oOrientation      = SimpleTypes::EPageOrientation::pageorientPortrait;

                        m_oPaperSize            = (SimpleTypes::Spreadsheet::EPageSize)ptr->iPaperSize;
                        m_oUseFirstPageNumber   = ptr->fUsePage;
                        m_oVerticalDpi          = ptr->iVRes;
                    }
                }
            }
			nullable<SimpleTypes::COnOff<>>							m_oBlackAndWhite;
			nullable<SimpleTypes::Spreadsheet::CCellComments<>>		m_oCellComments;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oCopies;
			nullable<SimpleTypes::COnOff<>>							m_oDraft;
			nullable<SimpleTypes::Spreadsheet::CPrintError<>>		m_oErrors;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oFirstPageNumber;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oFitToHeight;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oFitToWidth;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oHorizontalDpi;
			nullable<SimpleTypes::CRelationshipId>					m_oRId;
			nullable<SimpleTypes::CPageOrientation<>>				m_oOrientation;
			nullable<SimpleTypes::Spreadsheet::CPageOrder<>>		m_oPageOrder;
			nullable<SimpleTypes::CDouble>							m_oPaperHeight;
			nullable<SimpleTypes::Spreadsheet::CPageSize<>>			m_oPaperSize;
			nullable<SimpleTypes::CDouble>							m_oPaperWidth;
			nullable<SimpleTypes::Spreadsheet::CPageUnits<>>		m_oPaperUnits;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oScale;
			nullable<SimpleTypes::COnOff<>>							m_oUseFirstPageNumber;
			nullable<SimpleTypes::COnOff<>>							m_oUsePrinterDefaults;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oVerticalDpi;
		};
		class CPrintOptions : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPrintOptions)
            WritingElement_XlsbConstructors(CPrintOptions)
			CPrintOptions()
			{
			}
			virtual ~CPrintOptions()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oGridLines.IsInit() || m_oGridLinesSet.IsInit() || m_oHeadings.IsInit())
				{
					writer.WriteString(L"<printOptions");
					WritingStringNullableAttrBool(L"headings",	m_oHeadings);
					WritingStringNullableAttrBool(L"gridLines",	m_oGridLines);
					WritingStringNullableAttrBool(L"gridLinesSet", m_oGridLinesSet);
					writer.WriteString(L"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType () const
			{
				return et_x_PrintOptions;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"gridLines"),			m_oGridLines)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"gridLinesSet"),		m_oGridLinesSet)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"headings"),			m_oHeadings)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"horizontalCentered"),	m_oHorizontalCentered)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"verticalCentered"),	m_oVerticalCentered)
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::PrintOptions*>(obj.get());
                if(ptr != nullptr)
                {
                    m_oGridLines                = ptr->fPrintGrid;
                    m_oGridLinesSet             = ptr->fPrintGrid;
                    m_oHeadings                 = ptr->fPrintHeaders;
                    m_oHorizontalCentered       = ptr->fHCenter;
                    m_oVerticalCentered         = ptr->fVCenter;
                }
            }

		public:
			nullable<SimpleTypes::COnOff<>>	m_oGridLines;
			nullable<SimpleTypes::COnOff<>>	m_oGridLinesSet;
			nullable<SimpleTypes::COnOff<>>	m_oHeadings;
			nullable<SimpleTypes::COnOff<>>	m_oHorizontalCentered;
			nullable<SimpleTypes::COnOff<>>	m_oVerticalCentered;
		};
		class CDimension : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDimension)
            WritingElement_XlsbConstructors(CDimension)
			CDimension()
			{
			}
			virtual ~CDimension()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			virtual EElementType getType () const
			{
				return et_x_Dimension;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, (L"ref"), m_oRef)
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::WsDim*>(obj.get());
                if(ptr != nullptr)
                    m_oRef                  = ptr->rfx.toString();
            }

		public:
			nullable_string	m_oRef;
		};
		class CSheetFormatPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSheetFormatPr)
            WritingElement_XlsbConstructors(CSheetFormatPr)
			CSheetFormatPr()
			{
			}
			virtual ~CSheetFormatPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

                if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
            }

            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			virtual EElementType getType () const
			{
				return et_x_SheetFormatPr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"baseColWidth"),		m_oBaseColWidth)	// ToDo Excel не воспринимает значения не uint (мы приводим к uint)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"customHeight"),		m_oCustomHeight )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"defaultColWidth"),		m_oDefaultColWidth )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"defaultRowHeight"),	m_oDefaultRowHeight )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"outlineLevelCol"),		m_oOutlineLevelCol )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"outlineLevelRow"),		m_oOutlineLevelRow )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"thickBottom"),			m_oThickBottom )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"thickTop"),			m_oThickTop )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"zeroHeight"),			m_oZeroHeight )
				WritingElement_ReadAttributes_End( oReader )
            }

            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::WsFmtInfo*>(obj.get());
                if(ptr != nullptr)
                {
                    if(ptr->dxGCol != 0xFFFFFFFF)
                        m_oBaseColWidth = ptr->dxGCol / 256.;

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

		public:
				nullable_uint		m_oBaseColWidth;
				nullable_bool		m_oCustomHeight;
				nullable_double		m_oDefaultColWidth;
				nullable_double		m_oDefaultRowHeight;
				nullable_uint		m_oOutlineLevelCol;
				nullable_uint		m_oOutlineLevelRow;
				nullable_bool		m_oThickBottom;
				nullable_bool		m_oThickTop;
				nullable_bool		m_oZeroHeight;
		};
		class CPane : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPane)
            WritingElement_XlsbConstructors(CPane)
			CPane()
			{
			}
			virtual ~CPane()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString((L"<pane"));
					WritingStringNullableAttrDouble(L"xSplit", m_oXSplit, m_oXSplit->GetValue());
					WritingStringNullableAttrDouble(L"ySplit", m_oYSplit, m_oYSplit->GetValue());
					WritingStringNullableAttrString(L"topLeftCell", m_oTopLeftCell, m_oTopLeftCell.get());
					WritingStringNullableAttrString(L"activePane", m_oActivePane, m_oActivePane->ToString());
					WritingStringNullableAttrString(L"state", m_oState, m_oState->ToString());
				writer.WriteString((L"/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			virtual EElementType getType () const
			{
				return et_x_Pane;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"activePane")	, m_oActivePane)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"state")		, m_oState)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"topLeftCell"), m_oTopLeftCell)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"xSplit")		, m_oXSplit)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"ySplit")		, m_oYSplit)	
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto pPane = static_cast<XLSB::Pane*>(obj.get());
                if(pPane != nullptr)
                {
                    switch (pPane->pnnAcct_xlsb)
                    {
                        case 0:  m_oActivePane = SimpleTypes::Spreadsheet::EActivePane::activepaneBottomRight; break;
                        case 1:  m_oActivePane = SimpleTypes::Spreadsheet::EActivePane::activepaneTopRight; break;
                        case 2:  m_oActivePane = SimpleTypes::Spreadsheet::EActivePane::activepaneBottomLeft; break;
                        case 3:  m_oActivePane = SimpleTypes::Spreadsheet::EActivePane::activepaneTopLeft; break;
                    }

                    if(pPane->fFrozen)
                        m_oState         = SimpleTypes::Spreadsheet::EPaneState::panestateFrozenSplit;
                    else if(pPane->fFrozenNoSplit)
                        m_oState         = SimpleTypes::Spreadsheet::EPaneState::panestateFrozen;
                    else
                        m_oState         = SimpleTypes::Spreadsheet::EPaneState::panestateSplit;

                    m_oTopLeftCell   = pPane->topLeftCell;
                    m_oXSplit        = pPane->xnumXSplit.data.value;
                    m_oYSplit        = pPane->xnumYSplit.data.value;
                }
            }


		public:
            nullable<SimpleTypes::Spreadsheet::CActivePane<>>   m_oActivePane;
            nullable<SimpleTypes::Spreadsheet::CPaneState<>>    m_oState;
            nullable_string										m_oTopLeftCell;
            nullable<SimpleTypes::CDouble>                      m_oXSplit;
            nullable<SimpleTypes::CDouble>                      m_oYSplit;
		};
		class CSelection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSelection)
            WritingElement_XlsbConstructors(CSelection)
			CSelection()
			{
			}
			virtual ~CSelection()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString((L"<selection"));
				WritingStringNullableAttrString(L"pane", m_oPane, m_oPane->ToString());
				WritingStringNullableAttrString(L"activeCell", m_oActiveCell, m_oActiveCell.get());
				WritingStringNullableAttrInt(L"activeCellId", m_oActiveCellId, m_oActiveCellId->GetValue());
				WritingStringNullableAttrString(L"sqref", m_oSqref, m_oSqref.get());
				writer.WriteString((L"/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			virtual EElementType getType () const
			{
				return et_x_Selection;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"activeCell"),	m_oActiveCell)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"activeCellId"),	m_oActiveCellId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"sqref"),			m_oSqref)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"pane"),			m_oPane)
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto pSel = static_cast<XLSB::Sel*>(obj.get());
                if(pSel != nullptr)
                {
                    m_oActiveCell    = pSel->activeCell;
                    m_oActiveCellId  = pSel->irefAct;
                    m_oSqref         = pSel->sqref;
                    switch (pSel->pnn_xlsb)
                    {
                        case 0:  m_oPane = SimpleTypes::Spreadsheet::EActivePane::activepaneBottomRight; break;
                        case 1:  m_oPane = SimpleTypes::Spreadsheet::EActivePane::activepaneTopRight; break;
                        case 2:  m_oPane = SimpleTypes::Spreadsheet::EActivePane::activepaneBottomLeft; break;
                        case 3:  m_oPane = SimpleTypes::Spreadsheet::EActivePane::activepaneTopLeft; break;
                    }
                }
            }


		public:
			nullable_string										m_oActiveCell;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oActiveCellId;
			nullable_string										m_oSqref;
			nullable<SimpleTypes::Spreadsheet::CActivePane<>>	m_oPane;
		};

		//необработано:
		//<extLst>
		//<pivotSelection>
		class CSheetView : public WritingElementWithChilds<CSelection>
		{
		public:
			WritingElement_AdditionConstructors(CSheetView)
            WritingElement_XlsbConstructors(CSheetView)
			CSheetView()
			{
			}
			virtual ~CSheetView()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
                    if ( m_arrItems[i] )
					{
                        m_arrItems[i]->toXML(writer);
					}
				}

				writer.WriteString((L"</sheetView>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

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
						m_arrItems.push_back(new CSelection(oReader));
					}
				}
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                if(obj->get_type() == XLS::typeWSVIEW2)
                {
                    auto pWSVIEW2 = static_cast<XLSB::WSVIEW2*>(obj.get());
                    if (pWSVIEW2 == nullptr)
                        return;

                    ReadAttributes(pWSVIEW2->m_BrtBeginWsView);

                    m_oPane = pWSVIEW2->m_BrtPane;

                    if (pWSVIEW2->m_arBrtSel.empty())
                        return;

                    for(auto &pSel : pWSVIEW2->m_arBrtSel)
                    {
                        m_arrItems.push_back(new CSelection(pSel));
                    }
                }
                else if(obj->get_type() == XLS::typeCSVIEW)
                {
                    auto pCSVIEW = static_cast<XLSB::CSVIEW*>(obj.get());
                    if (pCSVIEW == nullptr)
                        return;

                    ReadAttributes(pCSVIEW->m_BrtBeginCsView);
                }
            }
			virtual EElementType getType () const
			{
				return et_x_SheetView;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"colorId"),				m_oColorId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"defaultGridColor"),	m_oDefaultGridColor)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"rightToLeft"),			m_oRightToLeft)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"showFormulas"),		m_oShowFormulas)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"showGridLines"),		m_oShowGridLines)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"showOutlineSymbols"),	m_oShowOutlineSymbols)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"showRowColHeaders"),	m_oShowRowColHeaders)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"showRuler"),			m_oShowRuler)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"showWhiteSpace"),		m_oShowWhiteSpace)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"showZeros"),			m_oShowZeros)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"tabSelected"),			m_oTabSelected)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"topLeftCell"),			m_oTopLeftCell)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"view"),				m_oView)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"windowProtection"),	m_oWindowProtection)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"workbookViewId"),		m_oWorkbookViewId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"zoomScale"),			m_oZoomScale)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"zoomScaleNormal"),		m_oZoomScaleNormal)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"zoomScalePageLayoutView"),		m_oZoomScalePageLayoutView)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"zoomScaleSheetLayoutView"),	m_oZoomScaleSheetLayoutView)		
				WritingElement_ReadAttributes_End( oReader )
			}

            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                if(obj->get_type() == XLS::typeBeginWsView)
                {
                    auto pWsView = static_cast<XLSB::BeginWsView*>(obj.get());
                    if(pWsView != nullptr)
                    {
                        m_oColorId                  = pWsView->icvHdr;
                        m_oDefaultGridColor         = pWsView->fDefaultHdr;
                        m_oRightToLeft              = pWsView->fRightToLeft;
                        m_oShowFormulas             = pWsView->fDspFmlaRt;
                        m_oShowGridLines            = pWsView->fDspGridRt;
                        m_oShowOutlineSymbols       = pWsView->fDspGuts;
                        m_oShowRowColHeaders        = pWsView->fDspRwColRt;
                        m_oShowRuler                = pWsView->fDspRuler;
                        m_oShowWhiteSpace           = pWsView->fWhitespaceHidden;
                        m_oShowZeros                = pWsView->fDspZerosRt;
                        m_oTabSelected              = pWsView->fSelected;
                        m_oTopLeftCell              = pWsView->topLeftCell;
                        m_oView                     = (SimpleTypes::Spreadsheet::ESheetViewType)pWsView->xlView;
                        m_oWindowProtection         = pWsView->fWnProt;
                        m_oWorkbookViewId           = pWsView->iWbkView;
                        m_oZoomScale                = pWsView->wScale;
                        m_oZoomScaleNormal          = pWsView->wScaleNormal;
                        m_oZoomScalePageLayoutView  = pWsView->wScalePLV;
                        m_oZoomScaleSheetLayoutView = pWsView->wScaleSLV;
                    }
                }
                else if(obj->get_type() == XLS::typeBeginCsView)
                {
                    auto pWsView = static_cast<XLSB::BeginCsView*>(obj.get());
                    if(pWsView != nullptr)
                    {
                        m_oTabSelected              = pWsView->fSelected;
                        m_oWorkbookViewId           = pWsView->iWbkView;
                        m_oZoomScale                = pWsView->wScale;
                    }
                }
            }

		public:
				nullable<CPane>										m_oPane;

				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oColorId;
				nullable<SimpleTypes::COnOff<>>						m_oDefaultGridColor;
				nullable<SimpleTypes::COnOff<>>						m_oRightToLeft;
				nullable<SimpleTypes::COnOff<>>						m_oShowFormulas;
				nullable<SimpleTypes::COnOff<>>						m_oShowGridLines;
				nullable<SimpleTypes::COnOff<>>						m_oShowOutlineSymbols;
				nullable<SimpleTypes::COnOff<>>						m_oShowRowColHeaders;
				nullable<SimpleTypes::COnOff<>>						m_oShowRuler;
				nullable<SimpleTypes::COnOff<>>						m_oShowWhiteSpace;
				nullable<SimpleTypes::COnOff<>>						m_oShowZeros;
				nullable<SimpleTypes::COnOff<>>						m_oTabSelected;
				nullable_string										m_oTopLeftCell;
				nullable<SimpleTypes::Spreadsheet::CSheetViewType<>>m_oView;
				nullable<SimpleTypes::COnOff<>>						m_oWindowProtection;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oWorkbookViewId;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScale;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScaleNormal;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScalePageLayoutView;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScaleSheetLayoutView;
		};
		class CSheetViews : public WritingElementWithChilds<CSheetView>
		{
		public:
			WritingElement_AdditionConstructors(CSheetViews)
            WritingElement_XlsbConstructors(CSheetViews)
			CSheetViews()
			{
			}
			virtual ~CSheetViews()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_arrItems.empty()) return;

				writer.WriteString((L"<sheetViews>"));
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
                    if ( m_arrItems[i] )
					{
                        m_arrItems[i]->toXML(writer);
					}
				}
				
				writer.WriteString((L"</sheetViews>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"sheetView" == sName )
						m_arrItems.push_back( new CSheetView( oReader ));
				}
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                if(obj->get_type() == XLS::typeWSVIEWS2)
                {
                    auto oWSVIEWS2 = static_cast<XLSB::WSVIEWS2*>(obj.get());
                    for(auto &pView : oWSVIEWS2->m_arWSVIEW2)
                    {
                        CSheetView *pSheetView = new CSheetView(pView);
                        m_arrItems.push_back(pSheetView);

                    }
                }
                else if(obj->get_type() == XLS::typeCSVIEWS)
                {
                    auto oCSVIEWS = static_cast<XLSB::CSVIEWS*>(obj.get());

                    for(auto &pView : oCSVIEWS->m_arCSVIEW)
                    {
                        CSheetView *pSheetView = new CSheetView(pView);
                        m_arrItems.push_back(pSheetView);

                    }
                }
            }
			virtual EElementType getType () const
			{
				return et_x_SheetViews;
			}
		};

		class CPageSetUpPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageSetUpPr)
            WritingElement_XlsbConstructors(CPageSetUpPr)
			CPageSetUpPr()
			{
			}
			virtual ~CPageSetUpPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString((L"<pageSetUpPr"));
				WritingStringNullableAttrBool(L"autoPageBreaks", m_oAutoPageBreaks);
				WritingStringNullableAttrBool(L"fitToPage", m_oFitToPage);
				writer.WriteString((L"/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType () const
			{
				return et_x_PageSetUpPr;
			}

		private:
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::WsProp*>(obj.get());
                if(ptr != nullptr)
                {
                    m_oAutoPageBreaks = ptr->fShowAutoBreaks;
                    m_oFitToPage      = ptr->fFitToPage;;
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, (L"autoPageBreaks"), m_oAutoPageBreaks )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"fitToPage"),	m_oFitToPage )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::COnOff<>>		m_oAutoPageBreaks;
			nullable<SimpleTypes::COnOff<>>		m_oFitToPage;
		};
		class COutlinePr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COutlinePr)
            WritingElement_XlsbConstructors(COutlinePr)
			COutlinePr()
			{
			}
			virtual ~COutlinePr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString((L"<outlinePr"));
					WritingStringNullableAttrBool(L"applyStyles", m_oApplyStyles);
					WritingStringNullableAttrBool(L"summaryBelow", m_oSummaryBelow);
					WritingStringNullableAttrBool(L"summaryRight", m_oSummaryRight);
					WritingStringNullableAttrBool(L"showOutlineSymbols", m_oShowOutlineSymbols);
				writer.WriteString((L"/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType () const
			{
				return et_x_OutlinePr;
			}

		private:
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::WsProp*>(obj.get());
                if(ptr != nullptr)
                {
                    m_oApplyStyles          = ptr->fApplyStyles;
                    m_oShowOutlineSymbols   = ptr->fShowOutlineSymbols;
                    m_oSummaryBelow         = ptr->fRowSumsBelow;
                    m_oSummaryRight         = ptr->fColSumsRight;
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, (L"applyStyles"), m_oApplyStyles )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"showOutlineSymbols"), m_oShowOutlineSymbols )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"summaryBelow"), m_oSummaryBelow )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"summaryRight"), m_oSummaryRight )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::COnOff<>>		m_oApplyStyles;
			nullable<SimpleTypes::COnOff<>>		m_oShowOutlineSymbols;
			nullable<SimpleTypes::COnOff<>>		m_oSummaryBelow;
			nullable<SimpleTypes::COnOff<>>		m_oSummaryRight;
		};
		class CSheetPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSheetPr)
            WritingElement_XlsbConstructors(CSheetPr)
			CSheetPr()
			{
			}
			virtual ~CSheetPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( (L"tabColor") == sName )
						m_oTabColor = oReader;
					else if ( (L"pageSetUpPr") == sName )
						m_oPageSetUpPr = oReader;
					else if ( (L"outlinePr") == sName )
						m_oOutlinePr = oReader;
				}
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                if(obj->get_type() == XLS::typeWsProp)
                {
                    m_oPageSetUpPr = obj;
                    m_oOutlinePr   = obj;
                }
                ReadAttributes(obj);
            }

			virtual EElementType getType () const
			{
				return et_x_SheetPr;
			}

		private:
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                if(obj->get_type() == XLS::typeWsProp)
                {
                    auto ptr = static_cast<XLSB::WsProp*>(obj.get());
                    if(ptr != nullptr)
                    {
                        if(!ptr->strName.value.value().empty())
                            m_oCodeName = ptr->strName.value.value();

                        m_oEnableFormatConditionsCalculation = ptr->fCondFmtCalc;
                        m_oFilterMode                        = ptr->fFilterMode;
                        m_oPublished                         = ptr->fPublish;
                        m_oSyncHorizontal                    = ptr->fSyncHoriz;
                        m_oSyncVertical                      = ptr->fSyncVert;
                        m_oTransitionEntry                   = ptr->fAltFormulaEntry;
                        m_oTransitionEvaluation              = ptr->fAltExprEval;

                        if(!ptr->syncRef.empty())
                            m_oSyncRef = ptr->syncRef;

                        m_oTabColor.Init();
                        m_oTabColor->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorTab));
                    }
                }
                else if(obj->get_type() == XLS::typeCsProp)
                {
                    auto ptr = static_cast<XLSB::CsProp*>(obj.get());
                    if(ptr != nullptr)
                    {
                        if(!ptr->strName.value.value().empty())
                            m_oCodeName = ptr->strName.value.value();

                        m_oPublished                         = ptr->fPublish;

                        m_oTabColor.Init();
                        m_oTabColor->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorTab));
                    }
                }

            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, (L"codeName"),							m_oCodeName )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"enableFormatConditionsCalculation"),	m_oEnableFormatConditionsCalculation )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"filterMode"),						m_oFilterMode )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"published"),							m_oPublished )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"syncHorizontal"),					m_oSyncHorizontal )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"syncRef"),							m_oSyncRef )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"syncVertical"),						m_oSyncVertical )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"transitionEntry"),					m_oTransitionEntry )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"transitionEvaluation"),				m_oTransitionEvaluation )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<CColor>					m_oTabColor;
			nullable<CPageSetUpPr>				m_oPageSetUpPr;
			nullable<COutlinePr>				m_oOutlinePr;

			nullable_string						m_oCodeName;
			nullable<SimpleTypes::COnOff<>>		m_oEnableFormatConditionsCalculation;
			nullable<SimpleTypes::COnOff<>>		m_oFilterMode;
			nullable<SimpleTypes::COnOff<>>		m_oPublished;
			nullable<SimpleTypes::COnOff<>>		m_oSyncHorizontal;
			nullable_string						m_oSyncRef;
			nullable<SimpleTypes::COnOff<>>		m_oSyncVertical;
			nullable<SimpleTypes::COnOff<>>		m_oTransitionEntry;
			nullable<SimpleTypes::COnOff<>>		m_oTransitionEvaluation;
		};
		class CHeaderFooterElement : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CHeaderFooterElement)
			CHeaderFooterElement()
			{
			}
			virtual ~CHeaderFooterElement()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sText = oReader.GetText3();
			}
			virtual EElementType getType () const
			{
				return et_x_HeaderFooterElementWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			std::wstring	m_sText;
		};

		class CHeaderFooter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CHeaderFooter)
            WritingElement_XlsbConstructors(CHeaderFooter)
			CHeaderFooter()
			{
			}
			virtual ~CHeaderFooter()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( (L"evenFooter") == sName )
						m_oEvenFooter = oReader;
					else if ( (L"evenHeader") == sName )
						m_oEvenHeader = oReader;
					else if ( (L"firstFooter") == sName )
						m_oFirstFooter = oReader;
					else if ( (L"firstHeader") == sName )
						m_oFirstHeader = oReader;
					else if ( (L"oddFooter") == sName )
						m_oOddFooter = oReader;
					else if ( (L"oddHeader") == sName )
						m_oOddHeader = oReader;
				}
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto pHEADERFOOTER = static_cast<XLSB::HEADERFOOTER*>(obj.get());
                if (pHEADERFOOTER == nullptr)
                    return;

                ReadAttributes(pHEADERFOOTER->m_BrtBeginHeaderFooter);

                auto ptr = static_cast<XLSB::BeginHeaderFooter*>(pHEADERFOOTER->m_BrtBeginHeaderFooter.get());
                if(ptr != nullptr)
                {
                    if(!ptr->stHeader.value().empty())
                    {
                        m_oOddHeader = new CHeaderFooterElement();
                        m_oOddHeader->m_sText = ptr->stHeader.value();
                    }
                    if(!ptr->stFooter.value().empty())
                    {
                        m_oOddFooter = new CHeaderFooterElement();
                        m_oOddFooter->m_sText = ptr->stFooter.value();
                    }
                    if(ptr->fHFDiffOddEven && !ptr->stHeaderEven.value().empty())
                    {
                        m_oEvenHeader = new CHeaderFooterElement();
                        m_oEvenHeader->m_sText = ptr->stHeaderEven.value();
                    }
                    if(ptr->fHFDiffOddEven && !ptr->stFooterEven.value().empty())
                    {
                        m_oEvenFooter = new CHeaderFooterElement();
                        m_oEvenFooter->m_sText = ptr->stFooterEven.value();
                    }
                    if(ptr->fHFDiffFirst && !ptr->stHeaderFirst.value().empty())
                    {
                        m_oFirstHeader = new CHeaderFooterElement();
                        m_oFirstHeader->m_sText = ptr->stHeaderFirst.value();
                    }
                    if(ptr->fHFDiffFirst && !ptr->stFooterFirst.value().empty())
                    {
                        m_oFirstFooter = new CHeaderFooterElement();
                        m_oFirstFooter->m_sText = ptr->stFooterFirst.value();
                    }
                }
            }
			virtual EElementType getType () const
			{
				return et_x_HeaderFooterWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, (L"alignWithMargins"),	m_oAlignWithMargins)
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"differentFirst"),	m_oDifferentFirst)
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"differentOddEven"),	m_oDifferentOddEven)
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"scaleWithDoc"),		m_oScaleWithDoc)
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::BeginHeaderFooter*>(obj.get());
                if(ptr != nullptr)
                {
                    m_oAlignWithMargins = ptr->fHFAlignMargins;
                    m_oDifferentFirst   = ptr->fHFDiffFirst;
                    m_oDifferentOddEven = ptr->fHFDiffOddEven;
                    m_oScaleWithDoc     = ptr->fHFScaleWithDoc;
                }
            }

		public:
			nullable<CHeaderFooterElement>		m_oEvenFooter;
			nullable<CHeaderFooterElement>		m_oEvenHeader;
			nullable<CHeaderFooterElement>		m_oFirstFooter;
			nullable<CHeaderFooterElement>		m_oFirstHeader;
			nullable<CHeaderFooterElement>		m_oOddFooter;
			nullable<CHeaderFooterElement>		m_oOddHeader;

			nullable<SimpleTypes::COnOff<>>		m_oAlignWithMargins;
			nullable<SimpleTypes::COnOff<>>		m_oDifferentFirst;
			nullable<SimpleTypes::COnOff<>>		m_oDifferentOddEven;
			nullable<SimpleTypes::COnOff<>>		m_oScaleWithDoc;

		};

		class CLegacyDrawingHFWorksheet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLegacyDrawingHFWorksheet)
            WritingElement_XlsbConstructors(CLegacyDrawingHFWorksheet)
			CLegacyDrawingHFWorksheet()
			{
			}
			virtual ~CLegacyDrawingHFWorksheet()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring  toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(!m_oId.IsInit()) return;

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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType () const
			{
				return et_x_LegacyDrawingHFWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"r:id"),	m_oId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"relationships:id"), m_oId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"cfe"),     m_oCfe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"cff"),     m_oCff )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"cfo"),	 m_oCfo )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"che"),	 m_oChe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"chf"),     m_oChf )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"cho"),     m_oCho )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"lfe"),     m_oLfe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"lff"),     m_oLff )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"lfo"),     m_oLfo )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"lhe"),     m_oLhe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"lhf"),     m_oLhf )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"lho"),     m_oLho )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"rfe"),     m_oRfe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"rff"),     m_oRff )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"rfo"),     m_oRfo )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"rhe"),     m_oRhe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"rhf"),     m_oRhf )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"rho"),     m_oRho )
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::LegacyDrawingHF*>(obj.get());
                if(ptr != nullptr)
                    m_oId = ptr->stRelId.value.value();
            }
		public:
			nullable<SimpleTypes::CRelationshipId >				m_oId;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCfe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCff;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCfo;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oChe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oChf;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCho;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLfe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLff;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLfo;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLhe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLhf;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLho;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRfe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRff;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRfo;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRhe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRhf;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRho;
		};

		class CPictureWorksheet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPictureWorksheet)
            WritingElement_XlsbConstructors(CPictureWorksheet)
			CPictureWorksheet()
			{
			}
			virtual ~CPictureWorksheet()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring  toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oId.IsInit())
				{
					writer.WriteString(L"<picture");
					WritingStringAttrString(L"r:id", m_oId->ToString());
					writer.WriteString(L"/>");
				}
				
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			virtual EElementType getType () const
			{
				return et_x_PictureWorksheet;
			}

		private:
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::BkHim*>(obj.get());
                if(ptr != nullptr)
                {
                    if(!ptr->rgb.value.value().empty())
                        m_oId = ptr->rgb.value.value();
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, L"id", m_oId )
				WritingElement_ReadAttributes_End_No_NS(oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oId;
		};

		class CBreak : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBreak)
            WritingElement_XlsbConstructors(CBreak)
			CBreak()
			{
			}
			virtual ~CBreak()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString((L"<brk"));
				WritingStringNullableAttrInt(L"id", m_oId, m_oId->GetValue());
				WritingStringNullableAttrBool(L"man", m_oMan);
				WritingStringNullableAttrInt(L"max", m_oMax, m_oMax->GetValue());
				WritingStringNullableAttrInt(L"min", m_oMin, m_oMin->GetValue());
				WritingStringNullableAttrBool(L"pt", m_oPt);
				writer.WriteString((L"/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType () const
			{
				return et_x_Break;
			}

		private:
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::Brk*>(obj.get());
                if(ptr != nullptr)
                {
                    m_oId   = ptr->unRwCol;
                    m_oMan  = ptr->fMan;
                    m_oMax  = ptr->unColRwStrt;
                    m_oMin  = ptr->unColRwEnd;
                    m_oPt   = ptr->fPivot;
                }
            }

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, (L"id"),	m_oId)
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"man"),	m_oMan)
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"max"),	m_oMax)
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"min"),	m_oMin)
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"pt"),	m_oPt)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oId;
			nullable<SimpleTypes::COnOff<>>					m_oMan;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMax;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMin;
			nullable<SimpleTypes::COnOff<>>					m_oPt;
		};

		class CRowColBreaks : public WritingElementWithChilds<CBreak>
		{
		public:
			WritingElement_AdditionConstructors(CRowColBreaks)
            WritingElement_XlsbConstructors(CRowColBreaks)
			CRowColBreaks()
			{
			}
			virtual ~CRowColBreaks()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXML2(writer, L"rowBreaks");
			}
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
			{
				writer.WriteString(L"<");
				writer.WriteString(sName);
				WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
				WritingStringNullableAttrInt(L"manualBreakCount", m_oManualBreakCount, m_oManualBreakCount->GetValue());
				writer.WriteString(L">");
				for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if ( m_arrItems[i] )
					{
						m_arrItems[i]->toXML(writer);
					}
				}
				writer.WriteString(L"</");
				writer.WriteString(sName);
				writer.WriteString(L">");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"brk" == sName )
						m_arrItems.push_back( new CBreak( oReader ));
				}
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                if(obj->get_type() == XLS::typeRWBRK)
                {
                    auto ptr = static_cast<XLSB::RWBRK*>(obj.get());
                    if(ptr != nullptr)
                    {
                        ReadAttributes(ptr->m_BrtBeginRwBrk);
                        for(auto &brk : ptr->m_arBrtBrk)
                            m_arrItems.push_back( new CBreak(brk));
                    }
                }

                else if(obj->get_type() == XLS::typeCOLBRK)
                {
                    auto ptr = static_cast<XLSB::COLBRK*>(obj.get());
                    if(ptr != nullptr)
                    {
                        ReadAttributes(ptr->m_BrtBeginColBrk);
                        for(auto &brk : ptr->m_arBrtBrk)
                            m_arrItems.push_back( new CBreak(brk));
                    }
                }
            }
			virtual EElementType getType () const
			{
				return et_x_RowColBreaks;
			}

		private:
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                if(obj->get_type() == XLS::typeBeginRwBrk)
                {
                    auto ptr = static_cast<XLSB::BeginRwBrk*>(obj.get());
                    if(ptr != nullptr)
                    {
                        m_oCount            = ptr->ibrkMac;
                        m_oManualBreakCount = ptr->ibrkManMac;
                    }
                }

                else if(obj->get_type() == XLS::typeBeginColBrk)
                {
                    auto ptr = static_cast<XLSB::BeginColBrk*>(obj.get());
                    if(ptr != nullptr)
                    {
                        m_oCount            = ptr->ibrkMac;
                        m_oManualBreakCount = ptr->ibrkManMac;
                    }
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, L"count",	m_oCount)
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"manualBreakCount",	m_oManualBreakCount)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oCount;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oManualBreakCount;
		};
		class CSheetProtection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSheetProtection)
            WritingElement_XlsbConstructors(CSheetProtection)
			CSheetProtection()
			{
			}
			virtual ~CSheetProtection()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<sheetProtection");
					WritingStringNullableAttrString(L"password",	m_oPassword,		m_oPassword.get());
					WritingStringNullableAttrString(L"algorithmName",m_oAlgorithmName,	m_oAlgorithmName->ToString());
					WritingStringNullableAttrString(L"hashValue",	m_oHashValue,		m_oHashValue.get());
					WritingStringNullableAttrString(L"saltValue",	m_oSaltValue,		m_oSaltValue.get());
					WritingStringNullableAttrInt(L"spinCount",		m_oSpinCount,		m_oSpinCount->GetValue());
					WritingStringNullableAttrInt(L"autoFilter",		m_oAutoFilter,		m_oAutoFilter->GetValue());
					WritingStringNullableAttrInt(L"content",		m_oContent,			m_oContent->GetValue());
					WritingStringNullableAttrInt(L"deleteColumns",	m_oDeleteColumns,	m_oDeleteColumns->GetValue());
					WritingStringNullableAttrInt(L"deleteRows",		m_oDeleteRows,		m_oDeleteRows->GetValue());
					WritingStringNullableAttrInt(L"formatCells",	m_oFormatCells,		m_oFormatCells->GetValue());
					WritingStringNullableAttrInt(L"formatColumns",	m_oFormatColumns,	m_oFormatColumns->GetValue());
					WritingStringNullableAttrInt(L"formatRows",		m_oFormatRows,		m_oFormatRows->GetValue());
					WritingStringNullableAttrInt(L"insertColumns",	m_oInsertColumns,	m_oInsertColumns->GetValue());
					WritingStringNullableAttrInt(L"insertHyperlinks", m_oInsertHyperlinks, m_oInsertHyperlinks->GetValue());
					WritingStringNullableAttrInt(L"insertRows",		m_oInsertRows,		m_oInsertRows->GetValue());
					WritingStringNullableAttrInt(L"objects",		m_oObjects,			m_oObjects->GetValue());
					WritingStringNullableAttrInt(L"pivotTables",	m_oPivotTables,		m_oPivotTables->GetValue());
					WritingStringNullableAttrInt(L"scenarios",		m_oScenarios,		m_oScenarios->GetValue());
					WritingStringNullableAttrInt(L"selectLockedCells",	m_oSelectLockedCells,	m_oSelectLockedCells->GetValue());
					WritingStringNullableAttrInt(L"selectUnlockedCells",	m_oSelectUnlockedCells,	m_oSelectUnlockedCells->GetValue());
					WritingStringNullableAttrInt(L"sheet",			m_oSheet,			m_oSheet->GetValue());
					WritingStringNullableAttrInt(L"sort",			m_oSort,			m_oSort->GetValue());
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			virtual EElementType getType () const
			{
				return et_x_SheetProtection;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"password"),		m_oPassword)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"algorithmName"),	m_oAlgorithmName)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"hashValue"),		m_oHashValue)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"saltValue"),		m_oSaltValue)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"spinCount"),		m_oSpinCount)
					
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"autoFilter"),	m_oAutoFilter)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"content"),		m_oContent)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"deleteColumns"),	m_oDeleteColumns)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"deleteRows"),	m_oDeleteRows)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"formatCells"),	m_oFormatCells)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"formatColumns"),	m_oFormatColumns)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"formatRows"),	m_oFormatRows)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"insertColumns"),	m_oInsertColumns)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"insertHyperlinks"),m_oInsertHyperlinks)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"insertRows"),	m_oInsertRows)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"objects"),		m_oObjects)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"pivotTables"),	m_oPivotTables)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"scenarios"),		m_oScenarios)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"selectLockedCells"),m_oSelectLockedCells)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"selectUnlockedCells"),m_oSelectUnlockedCells)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"sheet"),			m_oSheet)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"sort"),			m_oSort)
				WritingElement_ReadAttributes_End( oReader )
			}

            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                if(obj->get_type() == XLS::typeSheetProtection)
                {
                    auto ptr = static_cast<XLSB::SheetProtection*>(obj.get());
                    if(ptr != nullptr)
                    {
                        m_oPassword                = std::to_wstring(ptr->protpwd);
                        m_oAutoFilter              = (bool)ptr->fAutoFilter;
                        m_oContent                 = true;
                        m_oDeleteColumns           = (bool)ptr->fDeleteColumns;
                        m_oDeleteRows              = (bool)ptr->fDeleteRows;
                        m_oFormatCells             = (bool)ptr->fFormatCells;
                        m_oFormatColumns           = (bool)ptr->fFormatColumns;
                        m_oFormatRows              = (bool)ptr->fFormatRows;
                        m_oInsertColumns           = (bool)ptr->fInsertColumns;
                        m_oInsertHyperlinks        = (bool)ptr->fInsertHyperlinks;
                        m_oInsertRows              = (bool)ptr->fInsertRows;
                        m_oObjects                 = (bool)ptr->fObjects;
                        m_oPivotTables             = (bool)ptr->fPivotTables;
                        m_oScenarios               = (bool)ptr->fScenarios;
                        m_oSelectLockedCells       = (bool)ptr->fSelLockedCells;
                        m_oSelectUnlockedCells     = (bool)ptr->fSelUnlockedCells;
                        m_oSheet                   = (bool)ptr->fLocked;
                        m_oSort                    = (bool)ptr->fSort;
                    }
                }
                else if(obj->get_type() == XLS::typeSheetProtectionIso)
                {
                    auto ptr = static_cast<XLSB::SheetProtectionIso*>(obj.get());
                    if(ptr != nullptr)
                    {
                        m_oAlgorithmName           = ptr->ipdPasswordData.szAlgName.value();
                        m_oSpinCount               = ptr->dwSpinCount;
                        m_oHashValue               = std::wstring(ptr->ipdPasswordData.rgbHash.rgbData.begin(),
                                                                  ptr->ipdPasswordData.rgbHash.rgbData.end());
                        m_oSaltValue               = std::wstring(ptr->ipdPasswordData.rgbSalt.rgbData.begin(),
                                                                  ptr->ipdPasswordData.rgbSalt.rgbData.end());
                        m_oAutoFilter              = (bool)ptr->fAutoFilter;
                        m_oContent                 = true;
                        m_oDeleteColumns           = (bool)ptr->fDeleteColumns;
                        m_oDeleteRows              = (bool)ptr->fDeleteRows;
                        m_oFormatCells             = (bool)ptr->fFormatCells;
                        m_oFormatColumns           = (bool)ptr->fFormatColumns;
                        m_oFormatRows              = (bool)ptr->fFormatRows;
                        m_oInsertColumns           = (bool)ptr->fInsertColumns;
                        m_oInsertHyperlinks        = (bool)ptr->fInsertHyperlinks;
                        m_oInsertRows              = (bool)ptr->fInsertRows;
                        m_oObjects                 = (bool)ptr->fObjects;
                        m_oPivotTables             = (bool)ptr->fPivotTables;
                        m_oScenarios               = (bool)ptr->fScenarios;
                        m_oSelectLockedCells       = (bool)ptr->fSelLockedCells;
                        m_oSelectUnlockedCells     = (bool)ptr->fSelUnlockedCells;
                        m_oSheet                   = (bool)ptr->fLocked;
                        m_oSort                    = (bool)ptr->fSort;
                    }
                }
                else if(obj->get_type() == XLS::typeCsProtection)
                {
                    auto ptr = static_cast<XLSB::CsProtection*>(obj.get());
                    if(ptr != nullptr)
                    {
                        m_oPassword                = std::to_wstring(ptr->protpwd);
                        m_oObjects                 = (bool)ptr->fObjects;
                        m_oSheet                   = (bool)ptr->fLocked;
                    }
                }
                else if(obj->get_type() == XLS::typeCsProtectionIso)
                {
                    auto ptr = static_cast<XLSB::CsProtectionIso*>(obj.get());
                    if(ptr != nullptr)
                    {
                        m_oAlgorithmName           = ptr->ipdPasswordData.szAlgName.value();
                        m_oSpinCount               = ptr->dwSpinCount;
                        m_oHashValue               = std::wstring(ptr->ipdPasswordData.rgbHash.rgbData.begin(),
                                                                  ptr->ipdPasswordData.rgbHash.rgbData.end());
                        m_oSaltValue               = std::wstring(ptr->ipdPasswordData.rgbSalt.rgbData.begin(),
                                                                  ptr->ipdPasswordData.rgbSalt.rgbData.end());
                        m_oObjects                 = (bool)ptr->fObjects;
                        m_oSheet                   = (bool)ptr->fLocked;
                    }
                }

            }

			nullable<SimpleTypes::CCryptAlgoritmName<>>		m_oAlgorithmName;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oSpinCount;
			nullable_string		m_oHashValue;
			nullable_string		m_oSaltValue;
			nullable_string		m_oPassword; //for old wrike protection

			nullable<SimpleTypes::COnOff<>>		m_oAutoFilter;
			nullable<SimpleTypes::COnOff<>>		m_oContent;
			nullable<SimpleTypes::COnOff<>>		m_oDeleteColumns;
			nullable<SimpleTypes::COnOff<>>		m_oDeleteRows;
			nullable<SimpleTypes::COnOff<>>		m_oFormatCells;
			nullable<SimpleTypes::COnOff<>>		m_oFormatColumns;
			nullable<SimpleTypes::COnOff<>>		m_oFormatRows;
			nullable<SimpleTypes::COnOff<>>		m_oInsertColumns;
			nullable<SimpleTypes::COnOff<>>		m_oInsertHyperlinks;
			nullable<SimpleTypes::COnOff<>>		m_oInsertRows;
			nullable<SimpleTypes::COnOff<>>		m_oObjects;
			nullable<SimpleTypes::COnOff<>>		m_oPivotTables;
			nullable<SimpleTypes::COnOff<>>		m_oScenarios;
			nullable<SimpleTypes::COnOff<>>		m_oSelectLockedCells;
			nullable<SimpleTypes::COnOff<>>		m_oSelectUnlockedCells;
			nullable<SimpleTypes::COnOff<>>		m_oSheet;
			nullable<SimpleTypes::COnOff<>>		m_oSort;
		};
		class CDataRef : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataRef)
            WritingElement_XlsbConstructors(CDataRef)
			CDataRef()
			{
			}
			virtual ~CDataRef()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<sheetProtection");
					WritingStringAttrString(L"r:id", m_oId->ToString());
					WritingStringNullableAttrString(L"name",	m_oName,	m_oName.get());
					WritingStringNullableAttrString(L"ref",		m_oRef,		m_oRef.get());
					WritingStringNullableAttrString(L"sheet",	m_oSheet,	m_oSheet.get());
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType () const
			{
				return et_x_DataRef;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::DRef*>(obj.get());
                if(ptr != nullptr)
                {
                    if(!ptr->relId.value.value().empty())
                        m_oId = ptr->relId.value.value();

                    if(!ptr->xstrName.value().empty())
                        m_oName = ptr->xstrName.value();

                    if(!ptr->rfx.toString().empty())
                        m_oRef = ptr->rfx.toString();

                    if(!ptr->xstrSheet.value().empty())
                        m_oSheet = ptr->xstrSheet.value();
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"r:id"),		m_oId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"relationships:id"), m_oId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"name"),		m_oName)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"ref"),		m_oRef)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"sheet"),		m_oSheet)
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable<SimpleTypes::CRelationshipId>	m_oId;
			nullable_string							m_oName;
			nullable_string							m_oRef;
			nullable_string							m_oSheet;
		};
		class CDataRefs : public WritingElementWithChilds<CDataRef>
		{
		public:
			WritingElement_AdditionConstructors(CDataRefs)
            WritingElement_XlsbConstructors(CDataRefs)
			CDataRefs()
			{
			}
			virtual ~CDataRefs()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<dataRefs");
				WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
				writer.WriteString(L">");
				for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if ( m_arrItems[i] )
					{
						m_arrItems[i]->toXML(writer);
					}
				}
				writer.WriteString(L"</dataRefs>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"dataRef" == sName )
						m_arrItems.push_back( new CDataRef( oReader ));
				}
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::DREFS*>(obj.get());
                if(ptr != nullptr)
                {
                    m_oCount = (_UINT32)ptr->m_arBrtDRef.size();

                    for(auto &dref : ptr->m_arBrtDRef)
                        m_arrItems.push_back( new CDataRef( dref ));
                }
            }
			virtual EElementType getType () const
			{
				return et_x_DataRefs;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"count",	m_oCount)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oCount;
		};
		class CDataConsolidate : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataConsolidate)
            WritingElement_XlsbConstructors(CDataConsolidate)
			CDataConsolidate()
			{
			}
			virtual ~CDataConsolidate()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<dataConsolidate");
					WritingStringNullableAttrString(L"function",	m_oFunction,	m_oFunction->ToString());
					WritingStringNullableAttrInt(L"link",			m_oLink,		m_oLink->GetValue());
					WritingStringNullableAttrInt(L"startLabels",	m_oStartLabels,	m_oStartLabels->GetValue());
					WritingStringNullableAttrInt(L"topLabels",		m_oTopLabels,	m_oTopLabels->GetValue());
				writer.WriteString(L">");

				writer.WriteString(L"</dataConsolidate>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"dataRefs" == sName )
						m_oDataRefs = oReader;
				}
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::DCON*>(obj.get());
                if(ptr != nullptr)
                {
                    ReadAttributes(ptr->m_BrtBeginDCon);
                    if(ptr->m_DREFS != nullptr)
                        m_oDataRefs = ptr->m_DREFS;
                }
            }
			virtual EElementType getType () const
			{
				return et_x_DataConsolidate;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::BeginDCon*>(obj.get());
                if(ptr != nullptr)
                {
                    m_oFunction     = (SimpleTypes::Spreadsheet::EDataConsolidateFunction)ptr->iiftab.value().get();
                    m_oLink         = ptr->fLinkConsol;
                    m_oStartLabels  = ptr->fLeftCat;
                    m_oTopLabels    = ptr->fTopCat;
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"function"),		m_oFunction)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"link"),			m_oLink)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"startLabels"),	m_oStartLabels)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"topLabels"),		m_oTopLabels)
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable<SimpleTypes::Spreadsheet::CDataConsolidateFunction<>>	m_oFunction;
			nullable<SimpleTypes::COnOff<>>									m_oLink; 
			nullable<SimpleTypes::COnOff<>>									m_oStartLabels;
			nullable<SimpleTypes::COnOff<>>									m_oTopLabels;

			nullable<CDataRefs>	m_oDataRefs;

		};
	} //Spreadsheet
} // namespace OOX
