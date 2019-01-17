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

namespace OOX
{
	namespace Spreadsheet
	{
		class CExternalSheetNames : public WritingElementWithChilds<ComplexTypes::Spreadsheet::String>
		{
		public:
			WritingElement_AdditionConstructors(CExternalSheetNames)
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
			virtual EElementType getType() const
			{
				return et_x_ExternalSheetNames;
			}
		};

		class CExternalDefinedName : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExternalDefinedName)
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
			virtual EElementType getType() const
			{
				return et_x_ExternalDefinedName;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("name"), m_oName )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("refersTo"), m_oRefersTo )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("sheetId"), m_oSheetId )
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
			virtual EElementType getType() const
			{
				return et_x_ExternalDefinedNames;
			}
		};

		class CExternalCell : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExternalCell)
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
					m_oValue->toXML2(writer, _T("v"));
				writer.WriteString(L"</cell>");
			}
			virtual std::wstring      toXML() const
			{
				NSStringUtils::CStringBuilder writer;
				toXML(writer);
				return writer.GetData().c_str();
			}
			virtual EElementType getType() const
			{
				return et_x_ExternalCell;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("r"), m_oRef )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("t"), m_oType )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("vm"), m_oValueMetadata )
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
			virtual EElementType getType() const
			{
				return et_x_ExternalRow;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("r"), m_oR )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oR;
		};

		class CExternalSheetData : public WritingElementWithChilds<CExternalRow>
		{
		public:
			WritingElement_AdditionConstructors(CExternalSheetData)
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
			virtual EElementType getType() const
			{
				return et_x_ExternalSheetData;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("sheetId"), m_oSheetId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("refreshError"), m_oRefreshError )
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
			virtual EElementType getType() const
			{
				return et_x_ExternalSheetDataSet;
			}
		};

		class CExternalBook : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExternalBook)
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
			virtual EElementType getType() const
			{
				return et_x_ExternalBook;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("r:id"), m_oRid )
				WritingElement_ReadAttributes_End( oReader )
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
			virtual EElementType getType() const
			{
				return et_x_DdeValue;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("t"), m_oType )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CDdeValueType<> > m_oType;
		};

		class CDdeValues : public WritingElementWithChilds<CDdeValue>
		{
		public:
			WritingElement_AdditionConstructors(CDdeValues)
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
			virtual EElementType getType() const
			{
				return et_x_DdeValues;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("rows"), m_oRows )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("cols"), m_oCols )
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
			virtual EElementType getType() const
			{
				return et_x_OleItem;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("name"), m_oName )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("ole"), m_oOle )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("advise"), m_oAdvise )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("preferPic"), m_oPreferPic )
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
			virtual EElementType getType() const
			{
				return et_x_DdeItems;
			}
		};

		class CDdeLink : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDdeLink)
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
			virtual EElementType getType() const
			{
				return et_x_DdeLink;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("ddeService"), m_oDdeService )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("ddeTopic"), m_oDdeTopic )
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
			virtual EElementType getType() const
			{
				return et_x_OleItem;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("name"), m_oName )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("icon"), m_oIcon )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("advise"), m_oAdvise )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("preferPic"), m_oPreferPic )
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
			virtual EElementType getType() const
			{
				return et_x_OleItems;
			}
		};

		class COleLink : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COleLink)
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
			virtual EElementType getType() const
			{
				return et_x_OleLink;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("r:id"), m_oRid )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("progId"), m_oProgId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:

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
			CExternalLink(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				read( oRootPath, oPath );
			}
			virtual ~CExternalLink()
			{
			}
		public:

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

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;
				
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if ( _T("externalLink") == sName )
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
					{
						int nStylesDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
						{
							sName = XmlUtils::GetNameNoNS(oReader.GetName());

							if ( _T("externalBook") == sName )
							{
								m_oExternalBook = oReader;
							}
							else if ( _T("oleLink") == sName )
							{
								m_oOleLink = oReader;
							}
							else if ( _T("ddeLink") == sName )
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
				sXml.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
				sXml.WriteString(_T("<externalLink xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">"));
			
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
				
				sXml.WriteString(_T("</externalLink>"));

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
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //ExternalLink
} // namespace OOX

#endif // OOX_EXTERNALLINKS_FILE_INCLUDE_H_
