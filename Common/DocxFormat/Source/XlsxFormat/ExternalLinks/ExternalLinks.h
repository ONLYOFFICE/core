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
#include "../../XlsbFormat/Biff12_structures/CellRef.h"

namespace OOX
{
	namespace Spreadsheet
	{
		// FNV-1a hash, 32-bit 
		static constexpr std::uint32_t fnv1a2(const wchar_t* str, std::uint32_t hash = 2166136261UL) {
			return *str ? fnv1a2(str + 1, (hash ^ *str) * 16777619ULL) : hash;
		}
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				if (obj == nullptr)
					obj = XLS::BaseObjectPtr(new XLSB::SupTabs());

				auto ptr = static_cast<XLSB::SupTabs*>(obj.get());

				if (ptr != nullptr)
				{
					ptr->sheetNames.reserve(m_arrItems.size());
					for (auto &item : m_arrItems)
					{
						ptr->sheetNames.push_back(item->m_sVal.get());
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::EXTERNNAME*>(obj.get());
				if (ptr != nullptr)
				{
					if(m_oName.IsInit())
					{
						ptr->m_BrtSupNameStart = XLS::BaseObjectPtr(new XLSB::SupNameStart);

						auto ptrSupNameStart = static_cast<XLSB::SupNameStart*>(ptr->m_BrtSupNameStart.get());

						if (ptrSupNameStart != nullptr)
						{
							ptrSupNameStart->name = m_oName.get();
						}
					}

					if (m_oRefersTo.IsInit())
					{
						ptr->m_BrtSupNameFmla = XLS::BaseObjectPtr(new XLSB::SupNameFmla);

						auto ptrSupNameFmla = static_cast<XLSB::SupNameFmla*>(ptr->m_BrtSupNameFmla.get());

						if (ptrSupNameFmla != nullptr)
						{
							ptrSupNameFmla->fmla = m_oRefersTo.get();
						}
					}

					if (m_oSheetId.IsInit())
					{
						XLSB::ExternalReferenceType type;
						type = (_UINT16)XLSB::ExternalReferenceType::WORKBOOK;

						ptr->m_BrtSupNameBits = XLS::BaseObjectPtr(new XLSB::SupNameBits(type));

						auto ptrSupNameBits = static_cast<XLSB::SupNameBits*>(ptr->m_BrtSupNameBits.get());

						if (ptrSupNameBits != nullptr)
						{
							ptrSupNameBits->contentsExtName.iSheet = m_oSheetId->GetValue();
							ptrSupNameBits->contentsExtName.fBuiltIn = false;
						}
					}
				}
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
			void toBin(std::vector<XLS::BaseObjectPtr>& obj)
			{
				XLSB::ExternalReferenceType type;
				type = (_UINT16)XLSB::ExternalReferenceType::WORKBOOK;

				obj.reserve(m_arrItems.size());
				for (auto &oExternalDefinedName : m_arrItems)
				{
					XLS::BaseObjectPtr item(new XLSB::EXTERNNAME(type));
					oExternalDefinedName->toBin(item);
					obj.push_back(item);
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::EXTERNVALUE*>(obj.get());
				if (ptr != nullptr)
				{
					if (m_oValueMetadata.IsInit())
					{
						ptr->m_BrtExternValueMeta = XLS::BaseObjectPtr(new XLSB::ExternValueMeta());

						auto ptrExternValueMeta = static_cast<XLSB::ExternValueMeta*>(ptr->m_BrtExternValueMeta.get());

						if (ptrExternValueMeta != nullptr)
							ptrExternValueMeta->ivmb = m_oValueMetadata->GetValue();
					}

					_INT32 column = 0;
					if (m_oRef.IsInit())
					{
						auto wRef = XLSB::RgceLoc(std::wstring(m_oRef->begin(), m_oRef->end()));
						column = wRef.getColumn();
					}

					_INT16 nType = XLSB::rt_ExternCellBlank;

					if (m_oValue.IsInit())
					{
						if (m_oType.IsInit())
						{
							switch (m_oType->GetValue())
							{
							case SimpleTypes::Spreadsheet::celltypeDate: nType = XLSB::rt_ExternCellReal; break;
							case SimpleTypes::Spreadsheet::celltypeBool: nType = XLSB::rt_ExternCellBool; break;
							case SimpleTypes::Spreadsheet::celltypeError: nType = XLSB::rt_ExternCellError; break;
							case SimpleTypes::Spreadsheet::celltypeInlineStr:
							case SimpleTypes::Spreadsheet::celltypeStr: nType = XLSB::rt_ExternCellString; break;
							}
						}
						else
							nType = XLSB::rt_ExternCellReal;
					}

					XLS::BaseObjectPtr pExternCellBase = nullptr;
					
					if (m_oValue.IsInit())
					{
						switch (nType)
						{
						case XLSB::rt_ExternCellBlank:
							pExternCellBase = XLS::BaseObjectPtr(new XLSB::ExternCellBlank());
							if (pExternCellBase != nullptr)
							{
								static_cast<XLSB::ExternCellBlank*>(pExternCellBase.get())->col = column;
							}
							break;

						case XLSB::rt_ExternCellReal:
							pExternCellBase = XLS::BaseObjectPtr(new XLSB::ExternCellReal());
							if (pExternCellBase != nullptr)
							{
								static_cast<XLSB::ExternCellReal*>(pExternCellBase.get())->col = column;
								static_cast<XLSB::ExternCellReal*>(pExternCellBase.get())->value.data.value = std::stod(m_oValue->m_sText);
							}
							break;

						case XLSB::rt_ExternCellBool:
							pExternCellBase = XLS::BaseObjectPtr(new XLSB::ExternCellBool());
							if (pExternCellBase != nullptr)
							{
								static_cast<XLSB::ExternCellBool*>(pExternCellBase.get())->col = column;
								static_cast<XLSB::ExternCellBool*>(pExternCellBase.get())->value = m_oValue->m_sText == L"1" ? 1 : 0;
							}								
							break;

						case XLSB::rt_ExternCellError:
							pExternCellBase = XLS::BaseObjectPtr(new XLSB::ExternCellError());
							if (pExternCellBase != nullptr)
							{
								static_cast<XLSB::ExternCellError*>(pExternCellBase.get())->col = column;

								auto& errCode = static_cast<XLSB::ExternCellError*>(pExternCellBase.get())->value;
								switch (fnv1a2(m_oValue->m_sText.c_str()))
								{
								case fnv1a2(L"#NULL!"): errCode = 0x00; break;
								case fnv1a2(L"#DIV/0!"): errCode = 0x07; break;
								case fnv1a2(L"#VALUE!"): errCode = 0x0F; break;
								case fnv1a2(L"#REF!"): errCode = 0x17; break;
								case fnv1a2(L"#NAME?"): errCode = 0x1D; break;
								case fnv1a2(L"#NUM!"): errCode = 0x24; break;
								case fnv1a2(L"#N/A"): errCode = 0x2A; break;
								case fnv1a2(L"#GETTING_DATA"): errCode = 0x2B; break;
								}
							}
							break;
						case XLSB::rt_ExternCellString:
							pExternCellBase = XLS::BaseObjectPtr(new XLSB::ExternCellString());
							if (pExternCellBase != nullptr)
							{
								static_cast<XLSB::ExternCellString*>(pExternCellBase.get())->col = column;
								static_cast<XLSB::ExternCellString*>(pExternCellBase.get())->value = m_oValue->m_sText;
							}
							break;
						}
					}
					else
					{
						pExternCellBase = XLS::BaseObjectPtr(new XLSB::ExternCellBlank());
						if (pExternCellBase != nullptr)
						{
							static_cast<XLSB::ExternCellBlank*>(pExternCellBase.get())->col = column;
						}
					}

					ptr->m_EXTERNVALUEDATA = XLS::BaseObjectPtr(new XLSB::EXTERNVALUEDATA(0));
					auto ptrEXTERNVALUEDATA = static_cast<XLSB::EXTERNVALUEDATA*>(ptr->m_EXTERNVALUEDATA.get());

					if (ptrEXTERNVALUEDATA != nullptr)
					{
						ptrEXTERNVALUEDATA->m_source = pExternCellBase;
					}
				}
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::EXTERNROW*>(obj.get());
				if (ptr != nullptr)
				{
					ptr->m_BrtExternRowHdr = XLS::BaseObjectPtr(new XLSB::ExternRowHdr);

					auto ptrExternRowHdr = static_cast<XLSB::ExternRowHdr*>(ptr->m_BrtExternRowHdr.get());
					if (ptrExternRowHdr != nullptr)
					{
						if (m_oR.IsInit())
							ptrExternRowHdr->rw = m_oR->GetValue();
						else
							ptrExternRowHdr->rw = 0;
					}

					ptr->m_arEXTERNVALUE.reserve(m_arrItems.size());
					for (auto &oExternalCell : m_arrItems)
					{
						XLS::BaseObjectPtr item(new XLSB::EXTERNVALUE(ptrExternRowHdr->rw));
						oExternalCell->toBin(item);
						ptr->m_arEXTERNVALUE.push_back(item);
					}

				}
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::EXTERNTABLE*>(obj.get());
				if (ptr != nullptr)
				{
					ptr->m_BrtExternTableStart = XLS::BaseObjectPtr(new XLSB::ExternTableStart());

					auto ptrExternTableStart = static_cast<XLSB::ExternTableStart*>(ptr->m_BrtExternTableStart.get());
					if (ptrExternTableStart != nullptr)
					{
						if (m_oSheetId.IsInit())
							ptrExternTableStart->iTab = m_oSheetId->GetValue();
						else
							ptrExternTableStart->iTab = 0;

						if (m_oRefreshError.IsInit())
							ptrExternTableStart->fRefreshError = m_oRefreshError->GetValue();
						else
							ptrExternTableStart->fRefreshError = false;
					}

					ptr->m_arEXTERNROW.reserve(m_arrItems.size());
					for (auto &oExternalRow : m_arrItems)
					{
						XLS::BaseObjectPtr item(new XLSB::EXTERNROW());
						oExternalRow->toBin(item);
						ptr->m_arEXTERNROW.push_back(item);
					}
				}
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
			void toBin(std::vector<XLS::BaseObjectPtr>& obj)
			{
				obj.reserve(m_arrItems.size());
				for (auto &oExternalSheetData : m_arrItems)
				{
					XLS::BaseObjectPtr item(new XLSB::EXTERNTABLE());
					oExternalSheetData->toBin(item);
					obj.push_back(item);
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

                    auto ptrEXTERNALBOOK = static_cast<XLSB::EXTERNALBOOK*>(ptr->m_EXTERNALBOOK.get());
                    if(ptrEXTERNALBOOK != nullptr)
                    {
                        if(ptrEXTERNALBOOK->m_BrtSupTabs != nullptr)
                            m_oSheetNames = ptrEXTERNALBOOK->m_BrtSupTabs;
                        if(!ptrEXTERNALBOOK->m_arEXTERNNAME.empty())
                            m_oDefinedNames = ptrEXTERNALBOOK->m_arEXTERNNAME;
                        if(!ptrEXTERNALBOOK->m_arEXTERNTABLE.empty())
                            m_oSheetDataSet = ptrEXTERNALBOOK->m_arEXTERNTABLE;
                    }
                }
            }
			void toBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::EXTERNALLINK*>(obj.get());
				if (ptr != nullptr)
				{
					WriteAttributes(ptr->m_BrtBeginSupBook);
					
					ptr->m_EXTERNALBOOK = XLS::BaseObjectPtr(new XLSB::EXTERNALBOOK(static_cast<XLSB::BeginSupBook*>(ptr->m_BrtBeginSupBook.get())->sbt));
					auto ptrEXTERNALBOOK = static_cast<XLSB::EXTERNALBOOK*>(ptr->m_EXTERNALBOOK.get());
					if (ptrEXTERNALBOOK != nullptr)
					{
						if (m_oSheetNames.IsInit())
							m_oSheetNames->toBin(ptrEXTERNALBOOK->m_BrtSupTabs);

						if (m_oDefinedNames.IsInit())
							m_oDefinedNames->toBin(ptrEXTERNALBOOK->m_arEXTERNNAME);

						if (m_oSheetDataSet.IsInit())
							m_oSheetDataSet->toBin(ptrEXTERNALBOOK->m_arEXTERNTABLE);
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

			void WriteAttributes(XLS::BaseObjectPtr& obj)
			{
				if(obj == nullptr)
					obj = XLS::BaseObjectPtr(new XLSB::BeginSupBook());

				auto ptr = static_cast<XLSB::BeginSupBook*>(obj.get());
				if (ptr != nullptr)
				{
					ptr->sbt = XLSB::ExternalReferenceType::WORKBOOK;

					if (m_oRid.IsInit())
						ptr->string1 = m_oRid->GetValue();
					else
						ptr->string1 = L"";

					ptr->string2 = L"";
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::DDEOLEITEMVALUE*>(obj.get());
				if (ptr != nullptr)
				{			
					_INT16 nType = XLSB::rt_SupNameNil;

					if (m_oType.IsInit())
					{
						switch (m_oType->GetValue())
						{
						case SimpleTypes::Spreadsheet::EDdeValueType::ddevaluetypeN: nType = XLSB::rt_SupNameNum; break;
						case SimpleTypes::Spreadsheet::EDdeValueType::ddevaluetypeB: nType = XLSB::rt_SupNameBool; break;
						case SimpleTypes::Spreadsheet::EDdeValueType::ddevaluetypeE: nType = XLSB::rt_SupNameErr; break;
						case SimpleTypes::Spreadsheet::EDdeValueType::ddevaluetypeStr: nType = XLSB::rt_SupNameSt; break;
						}
					}
					else
						nType = XLSB::rt_SupNameNum;
					

					XLS::BaseObjectPtr pSupNameBase = nullptr;

					if (m_arrItems[0])
					{
						switch (nType)
						{
						case XLSB::rt_SupNameNil:
							pSupNameBase = XLS::BaseObjectPtr(new XLSB::SupNameNil());
							break;

						case XLSB::rt_SupNameNum:
							pSupNameBase = XLS::BaseObjectPtr(new XLSB::SupNameNum());
							if (pSupNameBase != nullptr)							
								static_cast<XLSB::ExternCellReal*>(pSupNameBase.get())->value.data.value = std::stod(m_arrItems[0]->m_sText);							
							break;

						case XLSB::rt_SupNameBool:
							pSupNameBase = XLS::BaseObjectPtr(new XLSB::SupNameBool());
							if (pSupNameBase != nullptr)
								static_cast<XLSB::SupNameBool*>(pSupNameBase.get())->value = m_arrItems[0]->m_sText == L"1" ? 1 : 0;
							break;							

						case XLSB::rt_SupNameErr:
							pSupNameBase = XLS::BaseObjectPtr(new XLSB::SupNameErr());
							if (pSupNameBase != nullptr)
							{
								auto& errCode = static_cast<XLSB::SupNameErr*>(pSupNameBase.get())->value;
								switch (fnv1a2(m_arrItems[0]->m_sText.c_str()))
								{
								case fnv1a2(L"#NULL!"): errCode = 0x00; break;
								case fnv1a2(L"#DIV/0!"): errCode = 0x07; break;
								case fnv1a2(L"#VALUE!"): errCode = 0x0F; break;
								case fnv1a2(L"#REF!"): errCode = 0x17; break;
								case fnv1a2(L"#NAME?"): errCode = 0x1D; break;
								case fnv1a2(L"#NUM!"): errCode = 0x24; break;
								case fnv1a2(L"#N/A"): errCode = 0x2A; break;
								case fnv1a2(L"#GETTING_DATA"): errCode = 0x2B; break;
								}
							}
							break;
						case XLSB::rt_SupNameSt:
							pSupNameBase = XLS::BaseObjectPtr(new XLSB::SupNameSt());
							if (pSupNameBase != nullptr)
								static_cast<XLSB::SupNameSt*>(pSupNameBase.get())->value = m_arrItems[0]->m_sText;
							break;
						}
					}
					else
					{
						pSupNameBase = XLS::BaseObjectPtr(new XLSB::SupNameNil());
					}
				
					ptr->m_source = pSupNameBase;					
				}
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
                                    m_oType->SetValue(SimpleTypes::Spreadsheet::EDdeValueType::ddevaluetypeNil);
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				if (obj == nullptr)
					obj = XLS::BaseObjectPtr(new XLSB::DDEOLEITEMVALUES);

				auto ptr = static_cast<XLSB::DDEOLEITEMVALUES*>(obj.get());
				if (ptr != nullptr)
				{
					WriteAttributes(ptr->m_BrtSupNameValueStart);

					ptr->m_arDDEOLEITEMVALUE.reserve(m_arrItems.size());
					for (auto &oDdeValue : m_arrItems)
					{
						XLS::BaseObjectPtr item(new XLSB::DDEOLEITEMVALUE());
						oDdeValue->toBin(item);
						ptr->m_arDDEOLEITEMVALUE.push_back(item);
					}
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
			void WriteAttributes(XLS::BaseObjectPtr& obj)
			{
				if (obj == nullptr)
					obj = XLS::BaseObjectPtr(new XLSB::SupNameValueStart());

				auto ptr = static_cast<XLSB::SupNameValueStart*>(obj.get());
				if (ptr != nullptr)
				{
					if (m_oRows.IsInit())
						ptr->cRw = m_oRows->GetValue();
					else
						ptr->cRw = 0;

					if (m_oCols.IsInit())
						ptr->cCol = m_oCols->GetValue();
					else
						ptr->cCol = 0;
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				XLSB::ExternalReferenceType type;
				type = (_UINT16)XLSB::ExternalReferenceType::DDE;

				auto ptr = static_cast<XLSB::DDEOLEITEM*>(obj.get());
				if (ptr != nullptr)
				{
					if (m_oDdeValues.IsInit())
						m_oDdeValues->toBin(ptr->m_DDEOLEITEMVALUES);

					if (m_oName.IsInit())
					{
						ptr->m_BrtSupNameStart = XLS::BaseObjectPtr(new XLSB::SupNameStart);

						auto ptrSupNameStart = static_cast<XLSB::SupNameStart*>(ptr->m_BrtSupNameStart.get());

						if (ptrSupNameStart != nullptr)
						{
							ptrSupNameStart->name = m_oName.get();
						}
					}

					ptr->m_BrtSupNameBits = XLS::BaseObjectPtr(new XLSB::SupNameBits(type));

					auto ptrSupNameBits = static_cast<XLSB::SupNameBits*>(ptr->m_BrtSupNameBits.get());

					if (ptrSupNameBits != nullptr)
					{
						if(m_oOle.IsInit())
							ptrSupNameBits->contentsDDE.fOLE = m_oOle->GetValue();
						else
							ptrSupNameBits->contentsDDE.fOLE = false;

						if (m_oAdvise.IsInit())
							ptrSupNameBits->contentsDDE.fWantAdvise = m_oAdvise->GetValue();
						else
							ptrSupNameBits->contentsDDE.fWantAdvise = false;

						if (m_oPreferPic.IsInit())
							ptrSupNameBits->contentsDDE.fWantPict = m_oPreferPic->GetValue();
						else
							ptrSupNameBits->contentsDDE.fWantPict = false;
					}

				}
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				XLSB::ExternalReferenceType type;
				type = (_UINT16)XLSB::ExternalReferenceType::DDE;

				auto ptr = static_cast<XLSB::DDEOLELINK*>(obj.get());
				if (ptr != nullptr)
				{
					ptr->m_arDDEOLEITEM.reserve(m_arrItems.size());
					for (auto &oDdeItem : m_arrItems)
					{
						XLS::BaseObjectPtr item(new XLSB::DDEOLEITEM(type));
						oDdeItem->toBin(item);
						ptr->m_arDDEOLEITEM.push_back(item);
					}
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::EXTERNALLINK*>(obj.get());
				if (ptr != nullptr)
				{
					WriteAttributes(ptr->m_BrtBeginSupBook);

					ptr->m_DDEOLELINK = XLS::BaseObjectPtr(new XLSB::DDEOLELINK(static_cast<XLSB::BeginSupBook*>(ptr->m_BrtBeginSupBook.get())->sbt));

					if (ptr->m_DDEOLELINK != nullptr)
					{
						if (m_oDdeItems.IsInit())
							m_oDdeItems->toBin(ptr->m_DDEOLELINK);						
					}
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
			void WriteAttributes(XLS::BaseObjectPtr& obj)
			{
				if (obj == nullptr)
					obj = XLS::BaseObjectPtr(new XLSB::BeginSupBook());

				auto ptr = static_cast<XLSB::BeginSupBook*>(obj.get());
				if (ptr != nullptr)
				{
					ptr->sbt = XLSB::ExternalReferenceType::DDE;

					if (m_oDdeService.IsInit())
						ptr->string1 = m_oDdeService.get();
					else
						ptr->string1 = L"";

					if (m_oDdeTopic.IsInit())
						ptr->string2 = m_oDdeTopic.get();
					else
						ptr->string2 = L"";
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				XLSB::ExternalReferenceType type;
				type = (_UINT16)XLSB::ExternalReferenceType::OLE;

				auto ptr = static_cast<XLSB::DDEOLEITEM*>(obj.get());
				if (ptr != nullptr)
				{
					//if (m_oOleValues.IsInit())
						//m_oOleValues->toBin(ptr->m_DDEOLEITEMVALUES);

					if (m_oName.IsInit())
					{
						ptr->m_BrtSupNameStart = XLS::BaseObjectPtr(new XLSB::SupNameStart);

						auto ptrSupNameStart = static_cast<XLSB::SupNameStart*>(ptr->m_BrtSupNameStart.get());

						if (ptrSupNameStart != nullptr)
						{
							ptrSupNameStart->name = m_oName.get();
						}
					}

					ptr->m_BrtSupNameBits = XLS::BaseObjectPtr(new XLSB::SupNameBits(type));

					auto ptrSupNameBits = static_cast<XLSB::SupNameBits*>(ptr->m_BrtSupNameBits.get());

					if (ptrSupNameBits != nullptr)
					{
						if (m_oIcon.IsInit())
							ptrSupNameBits->contentsOLE.fIcon = m_oIcon->GetValue();
						else
							ptrSupNameBits->contentsOLE.fIcon = false;

						if (m_oAdvise.IsInit())
							ptrSupNameBits->contentsOLE.fWantAdvise = m_oAdvise->GetValue();
						else
							ptrSupNameBits->contentsOLE.fWantAdvise = false;

						if (m_oPreferPic.IsInit())
							ptrSupNameBits->contentsOLE.fWantPict = m_oPreferPic->GetValue();
						else
							ptrSupNameBits->contentsOLE.fWantPict = false;
					}

				}
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				XLSB::ExternalReferenceType type;
				type = (_UINT16)XLSB::ExternalReferenceType::OLE;

				auto ptr = static_cast<XLSB::DDEOLELINK*>(obj.get());
				if (ptr != nullptr)
				{
					ptr->m_arDDEOLEITEM.reserve(m_arrItems.size());
					for (auto &oOleItem : m_arrItems)
					{
						XLS::BaseObjectPtr item(new XLSB::DDEOLEITEM(type));
						oOleItem->toBin(item);
						ptr->m_arDDEOLEITEM.push_back(item);
					}
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
			void toBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::EXTERNALLINK*>(obj.get());
				if (ptr != nullptr)
				{
					WriteAttributes(ptr->m_BrtBeginSupBook);

					ptr->m_DDEOLELINK = XLS::BaseObjectPtr(new XLSB::DDEOLELINK(static_cast<XLSB::BeginSupBook*>(ptr->m_BrtBeginSupBook.get())->sbt));

					if (ptr->m_DDEOLELINK != nullptr)
					{
						if (m_oOleItems.IsInit())
							m_oOleItems->toBin(ptr->m_DDEOLELINK);
					}
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
			void WriteAttributes(XLS::BaseObjectPtr& obj)
			{
				if (obj == nullptr)
					obj = XLS::BaseObjectPtr(new XLSB::BeginSupBook());

				auto ptr = static_cast<XLSB::BeginSupBook*>(obj.get());
				if (ptr != nullptr)
				{
					ptr->sbt = XLSB::ExternalReferenceType::OLE;

					if (m_oRid.IsInit())
						ptr->string1 = m_oRid->GetValue();
					else
						ptr->string1 = L"";

					if (m_oProgId.IsInit())
						ptr->string2 = m_oProgId.get();
					else
						ptr->string2 = L"";
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
                }
            }
			void writeBin(const CPath& oPath) const
			{
				CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
				if (xlsb)
				{
					XLSB::ExternalLinkStreamPtr externalLinkStreamStream(new XLSB::ExternalLinkStream);

					if (externalLinkStreamStream != nullptr)
					{
						externalLinkStreamStream->m_EXTERNALLINK = XLS::BaseObjectPtr(new XLSB::EXTERNALLINK());

						if (m_oExternalBook.IsInit())
						{
							m_oExternalBook->toBin(externalLinkStreamStream->m_EXTERNALLINK);
						}

						if (m_oDdeLink.IsInit())
						{
							m_oDdeLink->toBin(externalLinkStreamStream->m_EXTERNALLINK);
						}

						if (m_oOleLink.IsInit())
						{
							m_oOleLink->toBin(externalLinkStreamStream->m_EXTERNALLINK);
						}
					}
					xlsb->WriteBin(oPath, externalLinkStreamStream.get());
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
				if (dynamic_cast<CXlsb*>(File::m_pMainDocument) && !dynamic_cast<CXlsb*>(File::m_pMainDocument)->IsWriteToXlsx())
				{
					writeBin(oPath);
				}
				else
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
				}
				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write(oPath, oDirectory, oContent);
			}

			bool isValid() const
			{
				return true;
			}
			virtual const OOX::FileType type() const
			{
				if (dynamic_cast<CXlsb*>(File::m_pMainDocument) && !dynamic_cast<CXlsb*>(File::m_pMainDocument)->IsWriteToXlsx())
					return OOX::SpreadsheetBin::FileTypes::ExternalLinksBin;

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
