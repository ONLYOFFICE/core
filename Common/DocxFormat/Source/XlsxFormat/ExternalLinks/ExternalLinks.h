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
#ifndef OOX_EXTERNALLINKS_FILE_INCLUDE_H_
#define OOX_EXTERNALLINKS_FILE_INCLUDE_H_

#include "../CommonInclude.h"
#include "../../XlsbFormat/Xlsb.h"
#include "../../XlsbFormat/ExternalLinkStream.h"
#include "../SharedStrings/Text.h"

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

namespace OOX
{
	namespace Spreadsheet
	{
		class CExternalSheetNames : public WritingElementWithChilds<ComplexTypes::Spreadsheet::String>
		{
		public:
			WritingElement_AdditionConstructors(CExternalSheetNames)
            WritingElement_XlsbConstructors(CExternalSheetNames)
			CExternalSheetNames()
			{
			}
			virtual ~CExternalSheetNames()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if (L"sheetName" == sName)
					{
						m_arrItems.push_back(new ComplexTypes::Spreadsheet::String(oReader));
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual std::wstring toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::SupTabs*>(obj.get());
                if(ptr != nullptr)
                {
                    for(auto &item : ptr->sheetNames)
                    {
                        auto str = new ComplexTypes::Spreadsheet::String;
                        str->m_sVal = item.value();
                        m_arrItems.push_back(str);
                    }
                }
            }
			virtual EElementType getType() const
			{
				return et_x_ExternalSheetNames;
			}
		};

		class CExternalDefinedName : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExternalDefinedName)
            WritingElement_XlsbConstructors(CExternalDefinedName)
			CExternalDefinedName()
			{
			}
			virtual ~CExternalDefinedName()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<definedName");
				WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
				WritingStringNullableAttrEncodeXmlString(L"refersTo", m_oRefersTo, m_oRefersTo.get());
				WritingStringNullableAttrInt(L"sheetId", m_oSheetId, m_oSheetId->GetValue());
				writer.WriteString(L"/>");
			}
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType() const
			{
				return et_x_ExternalDefinedName;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::EXTERNNAME*>(obj.get());
                if(ptr != nullptr)
                {
                    auto ptrSupNameStart = static_cast<XLSB::SupNameStart*>(ptr->m_BrtSupNameStart.get());
                    if(ptrSupNameStart != nullptr)
                    {
                        if(!ptrSupNameStart->name.value().empty())
                            m_oName = ptrSupNameStart->name.value();
                    }

                    auto ptrSupNameFmla = static_cast<XLSB::SupNameFmla*>(ptr->m_BrtSupNameFmla.get());
                    if(ptrSupNameFmla != nullptr)
                    {
                        auto ref = ptrSupNameFmla->fmla.getAssembledFormula();
                        if(!ref.empty())
                            m_oRefersTo = ref;
                    }

                    auto ptrSupNameBits = static_cast<XLSB::SupNameBits*>(ptr->m_BrtSupNameBits.get());
                    if(ptrSupNameBits != nullptr)
                    {
                        m_oSheetId = ptrSupNameBits->contentsExtName.iSheet;
                    }

                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, (L"name"), m_oName )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"refersTo"), m_oRefersTo )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"sheetId"), m_oSheetId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<std::wstring> m_oName;
			nullable<std::wstring> m_oRefersTo;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oSheetId;
		};

		class CExternalDefinedNames : public WritingElementWithChilds<CExternalDefinedName>
		{
		public:
			WritingElement_AdditionConstructors(CExternalDefinedNames)
            WritingElement_XlsbVectorConstructors(CExternalDefinedNames)
			CExternalDefinedNames()
			{
			}
			virtual ~CExternalDefinedNames()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if (L"definedName" == sName)
					{
						m_arrItems.push_back(new CExternalDefinedName(oReader));
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<definedNames>");
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
                    if ( m_arrItems[i] )
					{
                        m_arrItems[i]->toXML(writer);
					}
				}
				writer.WriteString(L"</definedNames>");
			}
			virtual std::wstring toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(std::vector<XLS::BaseObjectPtr>& obj)
            {
                for(auto &item : obj)
                {
                    m_arrItems.push_back(new CExternalDefinedName(item));
                }
            }
			virtual EElementType getType() const
			{
				return et_x_ExternalDefinedNames;
			}
		};

		class CExternalCell : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExternalCell)
            WritingElement_XlsbConstructors(CExternalCell)
			CExternalCell()
			{
			}
			virtual ~CExternalCell()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if (L"v" == sName)
					{
						m_oValue = oReader;
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<cell");
				WritingStringNullableAttrString(L"r", m_oRef, m_oRef.get());
				WritingStringNullableAttrString(L"t", m_oType, m_oType->ToString());
				WritingStringNullableAttrInt(L"vm", m_oValueMetadata, m_oValueMetadata->GetValue());
				writer.WriteString(L">");
				if(m_oValue.IsInit())
					m_oValue->toXML2(writer, (L"v"));
				writer.WriteString(L"</cell>");
			}
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType() const
			{
				return et_x_ExternalCell;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::EXTERNVALUE*>(obj.get());
                if(ptr != nullptr)
                {
                    auto ptrExternValueMeta = static_cast<XLSB::ExternValueMeta*>(ptr->m_BrtExternValueMeta.get());
                    if(ptrExternValueMeta != nullptr)
                    {
                        m_oValueMetadata = ptrExternValueMeta->ivmb;
                    }

                    auto ptrEXTERNVALUEDATA = static_cast<XLSB::EXTERNVALUEDATA*>(ptr->m_EXTERNVALUEDATA.get());
                    if(ptrEXTERNVALUEDATA != nullptr)
                    {
                        if(!ptrEXTERNVALUEDATA->m_Ref.empty())
                            m_oRef = ptrEXTERNVALUEDATA->m_Ref;

                        if(ptrEXTERNVALUEDATA->m_source != nullptr)
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
                                        m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeDate);
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
                                        switch(pError)
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
                                        m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeInlineStr);
                                        m_oValue.Init();
                                        m_oValue->m_sText = pSt;
                                    }
                                   break;
                            }
                        }

                    }

                }               
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, (L"r"), m_oRef )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"t"), m_oType )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"vm"), m_oValueMetadata )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<std::wstring>								m_oRef;
			nullable<SimpleTypes::Spreadsheet::CCellTypeType<>>	m_oType;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oValueMetadata;

			nullable<CText>		m_oValue;
		};

		class CExternalRow : public WritingElementWithChilds<CExternalCell>
		{
		public:
			WritingElement_AdditionConstructors(CExternalRow)
            WritingElement_XlsbConstructors(CExternalRow)
			CExternalRow()
			{
			}
			virtual ~CExternalRow()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if (L"cell" == sName)
					{
						m_arrItems.push_back(new CExternalCell(oReader));
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<row");
				WritingStringNullableAttrInt(L"r", m_oR, m_oR->GetValue());
				writer.WriteString(L">");
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if ( m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				
				writer.WriteString(L"</row>");
			}
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType() const
			{
				return et_x_ExternalRow;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::EXTERNROW*>(obj.get());
                if(ptr != nullptr)
                {
                    auto ptrExternRowHdr = static_cast<XLSB::ExternRowHdr*>(ptr->m_BrtExternRowHdr.get());
                    if(ptrExternRowHdr != nullptr)
                    {
                        m_oR = ptrExternRowHdr->rw;
                    }

                    for(auto &item : ptr->m_arEXTERNVALUE)
                        m_arrItems.push_back(new CExternalCell(item));
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, (L"r"), m_oR )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oR;
		};

		class CExternalSheetData : public WritingElementWithChilds<CExternalRow>
		{
		public:
			WritingElement_AdditionConstructors(CExternalSheetData)
            WritingElement_XlsbConstructors(CExternalSheetData)
			CExternalSheetData()
			{
			}
			virtual ~CExternalSheetData()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if (L"row" == sName)
					{
						m_arrItems.push_back(new CExternalRow(oReader));
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<sheetData");
				WritingStringNullableAttrInt(L"sheetId", m_oSheetId, m_oSheetId->GetValue());
				WritingStringNullableAttrBool(L"refreshError", m_oRefreshError);
				writer.WriteString(L">");
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if ( m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }

				writer.WriteString(L"</sheetData>");
			}
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType() const
			{
				return et_x_ExternalSheetData;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::EXTERNTABLE*>(obj.get());
                if(ptr != nullptr)
                {
                    auto ptrExternTableStart = static_cast<XLSB::ExternTableStart*>(ptr->m_BrtExternTableStart.get());
                    if(ptrExternTableStart != nullptr)
                    {
                        m_oSheetId = ptrExternTableStart->iTab;
                        m_oRefreshError = ptrExternTableStart->fRefreshError;
                    }

                    for(auto &item : ptr->m_arEXTERNROW)
                        m_arrItems.push_back(new CExternalRow(item));
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, (L"sheetId"), m_oSheetId )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"refreshError"), m_oRefreshError )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oSheetId;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffTrue>> m_oRefreshError;
		};

		class CExternalSheetDataSet : public WritingElementWithChilds<CExternalSheetData>
		{
		public:
			WritingElement_AdditionConstructors(CExternalSheetDataSet)
            WritingElement_XlsbVectorConstructors(CExternalSheetDataSet)
			CExternalSheetDataSet()
			{
			}
			virtual ~CExternalSheetDataSet()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if (L"sheetData" == sName)
					{
						m_arrItems.push_back(new CExternalSheetData(oReader));
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<sheetDataSet>");
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if ( m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				writer.WriteString(L"</sheetDataSet>");
			}
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(std::vector<XLS::BaseObjectPtr>& obj)
            {
                for(auto &item : obj)
                {
                    m_arrItems.push_back(new CExternalSheetData(item));
                }
            }
			virtual EElementType getType() const
			{
				return et_x_ExternalSheetDataSet;
			}
		};

		class CExternalBook : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExternalBook)
            WritingElement_XlsbConstructors(CExternalBook)
			CExternalBook()
			{
			}
			virtual ~CExternalBook()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);
				
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    std::wstring sName = oReader.GetName();

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
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<externalBook");
				if (m_oRid.IsInit())
				{
					writer.WriteString(L" r:id=\"");
					writer.WriteString(m_oRid->ToString());
					writer.WriteString(L"\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"");
				}
				writer.WriteString(L">");
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
            virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::EXTERNALLINK*>(obj.get());
                if(ptr != nullptr)
                {
                    ReadAttributes(ptr->m_BrtBeginSupBook);

                    auto ptr1 = static_cast<XLSB::EXTERNALBOOK*>(ptr->m_EXTERNALBOOK.get());
                    if(ptr1 != nullptr)
                    {
                        if(ptr1->m_BrtSupTabs != nullptr)
                            m_oSheetNames = ptr1->m_BrtSupTabs;
                        if(!ptr1->m_arEXTERNNAME.empty())
                            m_oDefinedNames = ptr1->m_arEXTERNNAME;
                        if(!ptr1->m_arEXTERNTABLE.empty())
                            m_oSheetDataSet = ptr1->m_arEXTERNTABLE;
                    }
                }
            }
			virtual EElementType getType() const
			{
				return et_x_ExternalBook;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::BeginSupBook*>(obj.get());
                if(ptr != nullptr)
                {
                    if(!ptr->string1.empty())
                        m_oRid = ptr->string1;
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"id", m_oRid )
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}
		public:

			nullable<SimpleTypes::CRelationshipId > m_oRid;

			nullable<CExternalSheetNames > m_oSheetNames;
			nullable<CExternalDefinedNames > m_oDefinedNames;
			nullable<CExternalSheetDataSet > m_oSheetDataSet;
		};

		class CDdeValue : public WritingElementWithChilds<CText>
		{
		public:
			WritingElement_AdditionConstructors(CDdeValue)
            WritingElement_XlsbConstructors(CDdeValue)
			CDdeValue()
			{
			}
			virtual ~CDdeValue()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if (L"val" == sName)
					{
						m_arrItems.push_back(new CText(oReader));
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<value");
				WritingStringNullableAttrString(L"t", m_oType, m_oType->ToString());
				writer.WriteString(L">");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if ( m_arrItems[i] )
                    {
						m_arrItems[i]->toXML2(writer, L"val");
                    }
                }
				writer.WriteString(L"</value>");
			}
			virtual std::wstring toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType() const
			{
				return et_x_DdeValue;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::DDEOLEITEMVALUE*>(obj.get());
                if(ptr != nullptr)
                {

                    if(ptr->m_source != nullptr)
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
                                    switch(pError)
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
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, (L"t"), m_oType )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CDdeValueType<> > m_oType;
		};

		class CDdeValues : public WritingElementWithChilds<CDdeValue>
		{
		public:
			WritingElement_AdditionConstructors(CDdeValues)
            WritingElement_XlsbConstructors(CDdeValues)
			CDdeValues()
			{
			}
			virtual ~CDdeValues()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if (L"value" == sName)
					{
						m_arrItems.push_back(new CDdeValue(oReader));
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<values");
				WritingStringNullableAttrInt(L"rows", m_oRows, m_oRows->GetValue());
				WritingStringNullableAttrInt(L"cols", m_oCols, m_oCols->GetValue());
				writer.WriteString(L">");
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if ( m_arrItems[i] )
                    {
						m_arrItems[i]->toXML(writer);
                    }
                }
				writer.WriteString(L"</values>");
			}
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::DDEOLEITEMVALUES*>(obj.get());
                if(ptr != nullptr)
                {
                    ReadAttributes(ptr->m_BrtSupNameValueStart);

                    for(auto &item : ptr->m_arDDEOLEITEMVALUE)
                        m_arrItems.push_back(new CDdeValue(item));
                }
            }
			virtual EElementType getType() const
			{
				return et_x_DdeValues;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::SupNameValueStart*>(obj.get());
                if(ptr != nullptr)
                {
                    m_oRows = ptr->cRw;
                    m_oCols = ptr->cCol;
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, (L"rows"), m_oRows )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"cols"), m_oCols )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oRows;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oCols;
		};

		class CDdeItem : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDdeItem)
            WritingElement_XlsbConstructors(CDdeItem)
			CDdeItem()
			{
			}
			virtual ~CDdeItem()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if (L"values" == sName)
					{
						m_oDdeValues = oReader;
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType() const
			{
				return et_x_OleItem;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::DDEOLEITEM*>(obj.get());
                if(ptr != nullptr)
                {
                    if(ptr->m_DDEOLEITEMVALUES != nullptr)
                        m_oDdeValues = ptr->m_DDEOLEITEMVALUES;

                    auto ptrSupNameStart = static_cast<XLSB::SupNameStart*>(ptr->m_BrtSupNameStart.get());
                    if(ptrSupNameStart != nullptr)
                    {
                        if(!ptrSupNameStart->name.value().empty())
                            m_oName = ptrSupNameStart->name.value();
                    }

                    auto ptrSupNameBits = static_cast<XLSB::SupNameBits*>(ptr->m_BrtSupNameBits.get());
                    if(ptrSupNameBits != nullptr)
                    {
                        m_oOle       = ptrSupNameBits->contentsDDE.fOLE;
                        m_oAdvise    = ptrSupNameBits->contentsDDE.fWantAdvise;
                        m_oPreferPic = ptrSupNameBits->contentsDDE.fWantPict;
                    }
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, (L"name"), m_oName )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"ole"), m_oOle )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"advise"), m_oAdvise )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"preferPic"), m_oPreferPic )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<std::wstring > m_oName;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffTrue>> m_oOle;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffTrue>> m_oAdvise;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffTrue>> m_oPreferPic;

			nullable<CDdeValues> m_oDdeValues;
		};

		class CDdeItems : public WritingElementWithChilds<CDdeItem>
		{
		public:
			WritingElement_AdditionConstructors(CDdeItems)
            WritingElement_XlsbConstructors(CDdeItems)
			CDdeItems()
			{
			}
			virtual ~CDdeItems()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if (L"ddeItem" == sName)
					{
						m_arrItems.push_back(new CDdeItem(oReader));
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<ddeItems>");
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
                    if ( m_arrItems[i] )
					{
                        m_arrItems[i]->toXML(writer);
					}
				}
				writer.WriteString(L"</ddeItems>");
			}
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::DDEOLELINK*>(obj.get());
                if(ptr != nullptr)
                {
                    for(auto &item : ptr->m_arDDEOLEITEM)
                        m_arrItems.push_back(new CDdeItem(item));
                }
            }
			virtual EElementType getType() const
			{
				return et_x_DdeItems;
			}
		};

		class CDdeLink : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDdeLink)
            WritingElement_XlsbConstructors(CDdeLink)
			CDdeLink()
			{
			}
			virtual ~CDdeLink()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if (L"ddeItems" == sName)
					{
						m_oDdeItems = oReader;
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
            }
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::EXTERNALLINK*>(obj.get());
                if(ptr != nullptr)
                {
                    ReadAttributes(ptr->m_BrtBeginSupBook);

                    if(ptr->m_DDEOLELINK != nullptr)
                        m_oDdeItems = ptr->m_DDEOLELINK;
                }
            }
			virtual EElementType getType() const
			{
				return et_x_DdeLink;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::BeginSupBook*>(obj.get());
                if(ptr != nullptr)
                {
                    if(!ptr->string1.empty())
                        m_oDdeService = ptr->string1;
                    if(!ptr->string2.empty())
                        m_oDdeTopic   = ptr->string2;
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, (L"ddeService"), m_oDdeService )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"ddeTopic"), m_oDdeTopic )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:

			nullable<std::wstring > m_oDdeService;
			nullable<std::wstring > m_oDdeTopic;

			nullable<CDdeItems > m_oDdeItems;
		};

		class COleItem : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COleItem)
            WritingElement_XlsbConstructors(COleItem)
			COleItem()
			{
			}
			virtual ~COleItem()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<oleItem");
				WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
				WritingStringNullableAttrBool(L"icon", m_oIcon);
				WritingStringNullableAttrBool(L"advise", m_oAdvise);
				WritingStringNullableAttrBool(L"preferPic", m_oPreferPic);
				writer.WriteString(L"/>");
			}
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType() const
			{
				return et_x_OleItem;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::DDEOLEITEM*>(obj.get());
                if(ptr != nullptr)
                {
                    auto ptrSupNameStart = static_cast<XLSB::SupNameStart*>(ptr->m_BrtSupNameStart.get());
                    if(ptrSupNameStart != nullptr)
                    {
                        if(!ptrSupNameStart->name.value().empty())
                            m_oName = ptrSupNameStart->name.value();
                    }

                    auto ptrSupNameBits = static_cast<XLSB::SupNameBits*>(ptr->m_BrtSupNameBits.get());
                    if(ptrSupNameBits != nullptr)
                    {
                        m_oIcon      = ptrSupNameBits->contentsOLE.fIcon;
                        m_oAdvise    = ptrSupNameBits->contentsOLE.fWantAdvise;
                        m_oPreferPic = ptrSupNameBits->contentsOLE.fWantPict;
                    }
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, (L"name"), m_oName )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"icon"), m_oIcon )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"advise"), m_oAdvise )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"preferPic"), m_oPreferPic )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<std::wstring > m_oName;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffTrue>> m_oIcon;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffTrue>> m_oAdvise;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffTrue>> m_oPreferPic;
		};

		class COleItems : public WritingElementWithChilds<COleItem>
		{
		public:
			WritingElement_AdditionConstructors(COleItems)
            WritingElement_XlsbConstructors(COleItems)
			COleItems()
			{
			}
			virtual ~COleItems()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if (L"oleItem" == sName)
					{
						m_arrItems.push_back(new COleItem(oReader));
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<oleItems>");
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if ( m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				writer.WriteString(L"</oleItems>");
			}
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::DDEOLELINK*>(obj.get());
                if(ptr != nullptr)
                {
                    for(auto &item : ptr->m_arDDEOLEITEM)
                        m_arrItems.push_back(new COleItem(item));
                }
            }
			virtual EElementType getType() const
			{
				return et_x_OleItems;
			}
		};

		class COleLink : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COleLink)
            WritingElement_XlsbConstructors(COleLink)
			COleLink()
			{
			}
			virtual ~COleLink()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if (L"oleItems" == sName)
					{
						m_oOleItems = oReader;
					}
				}
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::EXTERNALLINK*>(obj.get());
                if(ptr != nullptr)
                {
                    ReadAttributes(ptr->m_BrtBeginSupBook);

                    if(ptr->m_DDEOLELINK != nullptr)
                        m_oOleItems = ptr->m_DDEOLELINK;
                }
            }
			virtual EElementType getType() const
			{
				return et_x_OleLink;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::BeginSupBook*>(obj.get());
                if(ptr != nullptr)
                {
                    if(!ptr->string1.empty())
                        m_oRid = ptr->string1;
                    if(!ptr->string2.empty())
                        m_oProgId = ptr->string2;
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, (L"r:id"), m_oRid )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"relationships:id"), m_oRid )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"progId"), m_oProgId )
				WritingElement_ReadAttributes_End( oReader )
			}

			nullable<SimpleTypes::CRelationshipId > m_oRid;
			nullable<std::wstring > m_oProgId;

			nullable<COleItems > m_oOleItems;
		};

		class CExternalLink : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CExternalLink(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
			}
            CExternalLink(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath, const std::wstring & rId) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
                m_rId = rId;
				read( oRootPath, oPath );
			}
			virtual ~CExternalLink()
			{
			}
            void readBin(const CPath& oPath)
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
                            if(ptr != nullptr)
                            {
                                auto ptr1 = static_cast<XLSB::BeginSupBook*>(ptr->m_BrtBeginSupBook.get());
                                if(ptr1 != nullptr)
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
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
			{
				m_oReadPath = oPath;
				IFileContainer::Read( oRootPath, oPath );

                if( m_oReadPath.GetExtention() == _T(".bin"))
                {
                    readBin(m_oReadPath);
                    return;
                }

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;
				
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if ( (L"externalLink") == sName )
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
					{
						int nStylesDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
						{
							sName = XmlUtils::GetNameNoNS(oReader.GetName());

							if ( (L"externalBook") == sName )
							{
								m_oExternalBook = oReader;
							}
							else if ( (L"oleLink") == sName )
							{
								m_oOleLink = oReader;
							}
							else if ( (L"ddeLink") == sName )
							{
								m_oDdeLink = oReader;
							}
						}
					}
				}		

			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
				sXml.WriteString((L"<externalLink xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">"));
			
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
				
				sXml.WriteString((L"</externalLink>"));

                std::wstring sPath = oPath.GetPath();
                NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write(oPath, oDirectory, oContent);
			}

			bool isValid() const
			{
				return true;
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::ExternalLinks;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}

			nullable<CExternalBook>					m_oExternalBook;
			nullable<COleLink>						m_oOleLink;
			nullable<CDdeLink>						m_oDdeLink;
		private:
			CPath									m_oReadPath;
            std::wstring                            m_rId;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //ExternalLink
} // namespace OOX

#endif // OOX_EXTERNALLINKS_FILE_INCLUDE_H_
