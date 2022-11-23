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

namespace OOX
{
	namespace Spreadsheet
	{
		class CSharedItemsIndex : public WritingElement
		{
		public:
			ComplexTypes_AdditionConstructors(CSharedItemsIndex)
            WritingElement_XlsbConstructors(CSharedItemsIndex)
			CSharedItemsIndex()
			{
			}
			virtual ~CSharedItemsIndex()
			{
			}
			virtual EElementType getType () const
			{
				return et_x_SharedItemsIndex;
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"v", m_oV );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<x");
					WritingStringNullableAttrInt(L"v", m_oV, m_oV->GetValue());
				writer.WriteString(L"/>");
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"v", m_oV )
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
		public:

			nullable<SimpleTypes::CUnsignedDecimalNumber > m_oV;
		};

		class CMemberPropertyIndex : public WritingElement
		{
		public:
			ComplexTypes_AdditionConstructors(CMemberPropertyIndex)
			CMemberPropertyIndex()
			{
			}
			virtual ~CMemberPropertyIndex()
			{
			}
			virtual EElementType getType () const
			{
				return et_x_MemberPropertyIndex;
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"v", m_oV );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<x");
					WritingStringNullableAttrInt2(L"v", m_oV);
				writer.WriteString(L"/>");
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"v", m_oV )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:

			nullable_int m_oV;
		};
		class CField : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CField)

			CField(){}
			virtual ~CField() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<field");
					WritingStringNullableAttrInt2(L"x", m_oX);
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
			}
			virtual EElementType getType () const
			{
				return et_x_Field;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle ( oReader, L"x",	m_oX )
				WritingElement_ReadAttributes_End( oReader )
			}
			
			nullable_int m_oX;
		};
		class CColumnRowFields : public WritingElementWithChilds<CField>
		{
		public:
			WritingElement_AdditionConstructors(CColumnRowFields)
            WritingElement_XlsbConstructors(CColumnRowFields)
			CColumnRowFields(){}
			virtual ~CColumnRowFields() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const {}
			void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_ColumnRowFields;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber > m_oCount;
		};
		class CDataField : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataField)
            WritingElement_XlsbConstructors(CDataField)
			CDataField(){}
			virtual ~CDataField() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_DataField;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable_int													m_oBaseField;
			nullable<SimpleTypes::CUnsignedDecimalNumber>					m_oBaseItem;
			nullable<SimpleTypes::CUnsignedDecimalNumber>					m_oFld;
			nullable_string													m_oName;
			nullable<SimpleTypes::CUnsignedDecimalNumber>					m_oNumFmtId;
			nullable<SimpleTypes::Spreadsheet::CShowDataAs>					m_oShowDataAs;
			nullable<SimpleTypes::Spreadsheet::CDataConsolidateFunction>	m_oSubtotal;

			nullable<OOX::Drawing::COfficeArtExtensionList>					m_oExtLst;
		};
		class CDataFields : public WritingElementWithChilds<CDataField>
		{
		public:
			WritingElement_AdditionConstructors(CDataFields)
            WritingElement_XlsbConstructors(CDataFields)
			CDataFields(){}
			virtual ~CDataFields() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_DataFields;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CPageField : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageField)
            WritingElement_XlsbConstructors(CPageField)
			CPageField(){}
			virtual ~CPageField() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PageField;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable_int									m_oFld;
			nullable_int									m_oHier;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oItem;
			nullable_string									m_oCap;
			nullable_string									m_oName;

			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
		class CPageFields : public WritingElementWithChilds<CPageField>
		{
		public:
			WritingElement_AdditionConstructors(CPageFields)
            WritingElement_XlsbConstructors(CPageFields)
			CPageFields(){}
			virtual ~CPageFields() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PageFields;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CFieldItem : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFieldItem)
            WritingElement_XlsbConstructors(CFieldItem)
			CFieldItem(){}
			virtual ~CFieldItem() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_FieldItem;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable_bool	m_oChild;
			nullable_bool	m_oExpanded;
			nullable_bool	m_oDrillAcross;
			nullable_bool	m_oCalculated;
			nullable_bool	m_oHidden;
			nullable_bool	m_oMissing;
			nullable_string	m_oUserCaption;
			nullable_bool	m_oCharacter;
			nullable_bool	m_oHideDetails;
			nullable<SimpleTypes::Spreadsheet::CPivotItemType>	m_oItemType;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oItemIndex;

		};
		class CFieldItems : public WritingElementWithChilds<CFieldItem>
		{
		public:
			WritingElement_AdditionConstructors(CFieldItems)
            WritingElement_XlsbConstructors(CFieldItems)
			CFieldItems(){}
			virtual ~CFieldItems() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_FieldItems;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CReference : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CReference)
            WritingElement_XlsbConstructors(CReference)
			CReference(){}
			virtual ~CReference() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_Reference;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable_bool		m_oAvgSubtotal;
			nullable_bool		m_oByPosition;
			nullable_bool		m_oCountASubtotal;
			nullable_bool		m_oCountSubtotal;
			nullable_bool		m_oDefaultSubtotal;
			nullable_bool		m_oMaxSubtotal;
			nullable_bool		m_oMinSubtotal;
			nullable_bool		m_oProductSubtotal;
			nullable_bool		m_oRelative;
			nullable_bool		m_oSelected;
			nullable_bool		m_oStdDevPSubtotal;
			nullable_bool		m_oStdDevSubtotal;
			nullable_bool		m_oSumSubtotal;
			nullable_bool		m_oVarPSubtotal;
			nullable_bool		m_oVarSubtotal;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oField;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;

			nullable<CSharedItemsIndex>	m_oX;
			nullable<OOX::Drawing::COfficeArtExtensionList>			m_oExtLst;
		};
		class CReferences : public WritingElementWithChilds<CReference>
		{
		public:
			WritingElement_AdditionConstructors(CReferences)
            WritingElement_XlsbConstructors(CReferences)
			CReferences(){}
			virtual ~CReferences() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_References;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CPivotArea : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPivotArea)
            WritingElement_XlsbConstructors(CPivotArea)
			CPivotArea(){}
			virtual ~CPivotArea() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotArea;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::Spreadsheet::CPivotAxisType> m_oAxis;
			nullable_bool	m_oCacheIndex;
			nullable_bool	m_oCollapsedLevelsAreSubtotals;
			nullable_bool	m_oDataOnly;
			nullable_int	m_oField;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oFieldPosition;
			nullable_bool	m_oGrandCol;
			nullable_bool	m_oGrandRow;
			nullable_bool	m_oLabelOnly;
			nullable_string	m_oOffsetRef;
			nullable_bool	m_oOutline;
			nullable<SimpleTypes::Spreadsheet::CPivotAreaType> m_oType;

			nullable<CReferences>							m_oReferences;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
		class CAutoSortScope : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAutoSortScope)
            WritingElement_XlsbConstructors(CAutoSortScope)
			CAutoSortScope(){}
			virtual ~CAutoSortScope() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_AutoSortScope;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader){}
			
			nullable<CPivotArea> m_oPivotArea;
		};
		class CPivotField : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPivotField)
            WritingElement_XlsbConstructors(CPivotField)
			CPivotField(){}
			virtual ~CPivotField() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotField;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable_bool	m_oAllDrilled;
			nullable_bool	m_oAutoShow;
			nullable_bool	m_oAvgSubtotal;
			nullable<SimpleTypes::Spreadsheet::CPivotAxisType> m_oAxis;
			nullable_bool	m_oCompact;
			nullable_bool	m_oCountASubtotal;
			nullable_bool	m_oCountSubtotal;
			nullable_bool	m_oDataField;
			nullable_bool	m_oDataSourceSort;
			nullable_bool	m_oDefaultAttributeDrillState;
			nullable_bool	m_oDefaultSubtotal;
			nullable_bool	m_oDragOff;
			nullable_bool	m_oDragToCol;
			nullable_bool	m_oDragToData;
			nullable_bool	m_oDragToPage;
			nullable_bool	m_oDragToRow;
			nullable_bool	m_oHiddenLevel;
			nullable_bool	m_oHideNewItems;
			nullable_bool	m_oIncludeNewItemsInFilter;
			nullable_bool	m_oInsertBlankRow;
			nullable_bool	m_oInsertPageBreak;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oItemPageCount;
			nullable_bool	m_oMaxSubtotal;
			nullable_bool	m_oMeasureFilter;
			nullable_bool	m_oMinSubtotal;
			nullable_bool	m_oMultipleItemSelectionAllowed;
			nullable_string	m_oName;
			nullable_bool	m_oNonAutoSortDefault;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oNumFmtId;
			nullable_bool	m_oOutline;
			nullable_bool	m_oProductSubtotal;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oRankBy;
			nullable_bool	m_oServerField;
			nullable_bool	m_oShowAll;
			nullable_bool	m_oShowDropDowns;
			nullable_bool	m_oShowPropAsCaption;
			nullable_bool	m_oShowPropCell;
			nullable_bool	m_oShowPropTip;
			nullable<SimpleTypes::Spreadsheet::CFieldSortType> m_oSortType;
			nullable_bool	m_oStdDevPSubtotal;
			nullable_bool	m_oStdDevSubtotal;
			nullable_string	m_oSubtotalCaption;
			nullable_bool	m_oSubtotalTop;
			nullable_bool	m_oSumSubtotal;
			nullable_bool	m_oTopAutoShow;
			nullable_string	m_oUniqueMemberProperty;
			nullable_bool	m_oVarPSubtotal;
			nullable_bool	m_oVarSubtotal;
			
			nullable<CAutoSortScope>						m_oAutoSortScope;
			nullable<CFieldItems>							m_oItems;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
		class CPivotFields : public WritingElementWithChilds<CPivotField>
		{
		public:
			WritingElement_AdditionConstructors(CPivotFields)
            WritingElement_XlsbConstructors(CPivotFields)
			CPivotFields(){}
			virtual ~CPivotFields() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotFields;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CColumnRowItem : public WritingElementWithChilds<CSharedItemsIndex>
		{
		public:
			WritingElement_AdditionConstructors(CColumnRowItem)
            WritingElement_XlsbConstructors(CColumnRowItem)
			CColumnRowItem(){}
			virtual ~CColumnRowItem() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_ColumnRowItem;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oI;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oR;
			nullable<SimpleTypes::Spreadsheet::CPivotItemType>		m_oT;
		};		
		class CColumnRowItems : public WritingElementWithChilds<CColumnRowItem>
        {
		public:
			WritingElement_AdditionConstructors(CColumnRowItems)
            WritingElement_XlsbConstructors(CColumnRowItems)
			CColumnRowItems(){}
			virtual ~CColumnRowItems() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const {}
			void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_ColumnRowItems;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CPivotTableFormat : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPivotTableFormat)
            WritingElement_XlsbConstructors(CPivotTableFormat)
			CPivotTableFormat(){}
			virtual ~CPivotTableFormat() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotTableFormat;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::Spreadsheet::CFormatAction>	m_oAction;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oDxfId;
			
			nullable<CPivotArea>								m_oPivotArea;
			nullable<OOX::Drawing::COfficeArtExtensionList>		m_oExtLst;
		};
		class CPivotTableFormats : public WritingElementWithChilds<CPivotTableFormat>
		{
		public:
			WritingElement_AdditionConstructors(CPivotTableFormats)
            WritingElement_XlsbConstructors(CPivotTableFormats)
			CPivotTableFormats(){}
			virtual ~CPivotTableFormats() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotTableFormats;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CPivotTableStyleInfo : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPivotTableStyleInfo)
            WritingElement_XlsbConstructors(CPivotTableStyleInfo)
			CPivotTableStyleInfo(){}
			virtual ~CPivotTableStyleInfo() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType () const
			{
				return et_x_PivotTableStyleInfo;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string	m_oName;
			nullable_bool	m_oShowColHeaders;
			nullable_bool	m_oShowColStripes;
			nullable_bool	m_oShowLastColumn;
			nullable_bool	m_oShowRowHeaders;
			nullable_bool	m_oShowRowStripes;

		};
		class CPivotTableLocation : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPivotTableLocation)
            WritingElement_XlsbConstructors(CPivotTableLocation)
			CPivotTableLocation(){}
			virtual ~CPivotTableLocation() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;
			}
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotTableLocation;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oColPageCount;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oFirstDataCol;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oFirstDataRow;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oFirstHeaderRow;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oRowPageCount;
			nullable_string									m_oRef;
		};
		class CPivotTableDefinition : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPivotTableDefinition)
            WritingElement_XlsbConstructors(CPivotTableDefinition)
			CPivotTableDefinition()
			{
			}
			virtual ~CPivotTableDefinition()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotTableDefinition;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);		
            void ReadAttributes(XLS::BaseObjectPtr& obj);
	//----------
			nullable<CColumnRowFields>		m_oColFields;
			nullable<CColumnRowItems>		m_oColItems;
			nullable<CDataFields>			m_oDataFields;
			nullable<CPivotTableFormats>	m_oFormats;
			nullable<CPivotTableLocation>	m_oLocation;
			nullable<CPageFields>			m_oPageFields;
			nullable<CPivotFields>			m_oPivotFields;
			nullable<CColumnRowFields>		m_oRowFields;
			nullable<CColumnRowItems>		m_oRowItems;
			nullable<CPivotTableStyleInfo>	m_oPivotTableStyleInfo;
	//nullable<CPivotTableFilters>					m_oFilters;
	//nullable<CPivotChartFormats>					m_oChartFormats;
	//nullable<Pivot Conditional Formats>			m_oConditionalFormats;
	//nullable<Column OLAP Hierarchy References>	m_oColHierarchiesUsage;
	//nullable<PivotTable OLAP Hierarchies>			m_oPivotHierarchies;
	//nullable<Row OLAP Hierarchy References>		m_oRowHierarchiesUsage;

			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
	//----------
			nullable_bool	m_oApplyAlignmentFormats;
			nullable_bool	m_oApplyBorderFormats;
			nullable_bool	m_oApplyFontFormats;
			nullable_bool	m_oApplyNumberFormats;
			nullable_bool	m_oApplyPatternFormats;
			nullable_bool	m_oApplyWidthHeightFormats;
			nullable_bool	m_oAsteriskTotals;
			nullable_bool	m_oColGrandTotals;
			nullable_string	m_oColHeaderCaption;
			nullable_bool	m_oCompact;
			nullable_bool	m_oCompactData;
			nullable_bool	m_oCustomListSort;
			nullable_string	m_oDataCaption;
			nullable_bool	m_oDataOnRows;
			nullable_bool	m_oDisableFieldList;
			nullable_bool	m_oEditData;
			nullable_bool	m_oEnableDrill;
			nullable_bool	m_oEnableFieldProperties;
			nullable_bool	m_oEnableWizard;
			nullable_string	m_oErrorCaption;
			nullable_bool	m_oFieldListSortAscending;
			nullable_bool	m_oFieldPrintTitles;
			nullable_string	m_oGrandTotalCaption;
			nullable_bool	m_oGridDropZones;
			nullable_bool	m_oImmersive;
			nullable_bool	m_oItemPrintTitles;
			nullable_bool	m_oMdxSubqueries;
			nullable_bool	m_oMergeItem;
			nullable_string	m_oMissingCaption;
			nullable_bool	m_oMultipleFieldFilters;
			nullable_string	m_oName;
			nullable_bool	m_oOutline;
            nullable_bool	m_oOutlineData;
			nullable_bool	m_oPageOverThenDown;
			nullable_string	m_oPageStyle;
			nullable_string	m_oPivotTableStyle;
			nullable_bool	m_oPreserveFormatting;
			nullable_bool	m_oPrintDrill;
			nullable_bool	m_oPublished;
			nullable_bool	m_oRowGrandTotals;
			nullable_string	m_oRowHeaderCaption;
			nullable_bool	m_oShowCalcMbrs;
			nullable_bool	m_oShowDataDropDown;
			nullable_bool	m_oShowDataTips;
			nullable_bool	m_oShowDrill;
			nullable_bool	m_oShowDropZones;
			nullable_bool	m_oShowEmptyCol;
			nullable_bool	m_oShowEmptyRow;
			nullable_bool	m_oShowError;
			nullable_bool	m_oShowHeaders;
			nullable_bool	m_oShowItems;
			nullable_bool	m_oShowMemberPropertyTips;
			nullable_bool	m_oShowMissing;
			nullable_bool	m_oShowMultipleLabel;
			nullable_bool	m_oSubtotalHiddenItems;
			nullable_string	m_oTag;
			nullable_bool	m_oUseAutoFormatting;
			nullable_string	m_oVacatedStyle;
			nullable_bool	m_oVisualTotals;
			
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCreatedVersion;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oMinRefreshableVersion;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oUpdatedVersion;
			
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oDataPosition;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oAutoFormatId;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCacheId;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oChartFormat;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oIndent;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oPageWrap;
		};

		class CPivotTableFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CPivotTableFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				
				m_pData = NULL;
				m_nDataLength = 0;
			}
			CPivotTableFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;

				m_pData = NULL;
				m_nDataLength = 0;

				read( oRootPath, oPath );
			}
			virtual ~CPivotTableFile()
			{
				m_nDataLength = 0;
				RELEASEARRAYOBJECTS(m_pData)
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			void setData(BYTE* pData, long length)
			{
				m_nDataLength = length;
				m_pData = new BYTE[length];
				memcpy(m_pData, pData, length);
			}
            void readBin(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::PivotTable;
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

			nullable<CPivotTableDefinition> m_oPivotTableDefinition;
		private:
			BYTE *m_pData;
			long m_nDataLength;

			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};

	} //Spreadsheet
} // namespace OOX
