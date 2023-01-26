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
#ifndef OOX_LOGIC_SDT_INCLUDE_H_
#define OOX_LOGIC_SDT_INCLUDE_H_

#include "../WritingElement.h"
#include "RunProperty.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// SdtListItem 17.5.2.21 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtListItem : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSdtListItem)
			CSdtListItem();
			virtual ~CSdtListItem();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string	m_sDisplayText;
			nullable_string	m_sValue;
		};

		//--------------------------------------------------------------------------------
		// DataBinding 17.5.2.6 (Part 1)
		//--------------------------------------------------------------------------------
		class CDataBinding : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDataBinding)
			CDataBinding();
			virtual ~CDataBinding();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<std::wstring > m_sPrefixMappings;
			nullable<std::wstring > m_sStoreItemID;
			nullable<std::wstring > m_sXPath;
		};

		//--------------------------------------------------------------------------------
		// CalendarType 17.5.2.3 (Part 1)
		//--------------------------------------------------------------------------------
		class CCalendarType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CCalendarType)
			CCalendarType();
			virtual ~CCalendarType();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::CCalendarType m_oVal;
		};

		//--------------------------------------------------------------------------------
		// SdtDateMappingType 17.5.2.40 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtDateMappingType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSdtDateMappingType)
			CSdtDateMappingType();
			virtual ~CSdtDateMappingType();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::CSdtDateMappingType m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Lock 17.5.2.23 (Part 1)
		//--------------------------------------------------------------------------------
		class CLock : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLock)
			CLock();
			virtual ~CLock();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::CLock m_oVal;
		};

		//--------------------------------------------------------------------------------
		// SdtText 17.5.2.44 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtText : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSdtText)
			CSdtText();
			virtual ~CSdtText();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff> m_oMultiLine;
		};
		//--------------------------------------------------------------------------------
		// SdtAppearance 2.5.4.2 ([MS-DOCX])
		//--------------------------------------------------------------------------------
		class CSdtAppearance : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSdtAppearance)
			CSdtAppearance();
			virtual ~CSdtAppearance();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<SimpleTypes::CSdtAppearance> m_oVal;
		};

		//Not from specification
		class CComb : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CComb)
			CComb();
			virtual ~CComb();
			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CHeightRule>		m_oWRule;
			nullable_int							m_oWidth;
			nullable_string							m_oSym;
			nullable_string							m_oFont;
		};

		//Not from specification
		class CTextFormFormat : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTextFormFormat)
			CTextFormFormat();
			virtual ~CTextFormFormat();
			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTextFormFormatType> m_oType;
			nullable_string m_oVal;
			nullable_string m_oSymbols;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//Not from specification
		class CFormPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFormPr)
			CFormPr(OOX::Document *pMain = NULL);
			virtual ~CFormPr();
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_oKey;
			nullable_string m_oLabel;
			nullable_string m_oHelpText;
			nullable_bool m_oRequired;

			nullable<ComplexTypes::Word::CBorder> m_oBorder;
			nullable<ComplexTypes::Word::CShading> m_oShd;
			nullable<SimpleTypes::CRelationshipId> m_oFieldRid;
		};

		//Not from specification
		class CTextFormPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTextFormPr)
			CTextFormPr(OOX::Document *pMain = NULL);
			virtual ~CTextFormPr();
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<ComplexTypes::Word::CComb> m_oComb;
			nullable<ComplexTypes::Word::CDecimalNumber> m_oMaxCharacters;
			nullable<ComplexTypes::Word::CBorder> m_oCombBorder;
			nullable_bool m_oAutoFit;
			nullable_bool m_oMultiLine;
			nullable<ComplexTypes::Word::CTextFormFormat> m_oFormat;
		};

		//Not from specification
		class CComplexFormPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CComplexFormPr)
			CComplexFormPr(OOX::Document *pMain = NULL);
			virtual ~CComplexFormPr();
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CComplexFormType> m_oType;
		};

		//--------------------------------------------------------------------------------
		// CSdtComboBox 17.5.2.5 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtComboBox : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtComboBox)
			CSdtComboBox(OOX::Document *pMain = NULL);
			virtual ~CSdtComboBox();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<ComplexTypes::Word::CTextFormFormat> m_oFormat;
			nullable<std::wstring > m_sLastValue;

			std::vector<ComplexTypes::Word::CSdtListItem*> m_arrListItem;
		};

		//--------------------------------------------------------------------------------
		// CDate 17.5.2.7 (Part 1)
		//--------------------------------------------------------------------------------
		class CDate : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDate)
			CDate(OOX::Document *pMain = NULL);
			virtual ~CDate();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDateTime> m_oFullDate;

			nullable<ComplexTypes::Word::CCalendarType> m_oCalendar;
			nullable<ComplexTypes::Word::String> m_oDateFormat;
			nullable<ComplexTypes::Word::CLang> m_oLid;
			nullable<ComplexTypes::Word::CSdtDateMappingType> m_oStoreMappedDateAs;
		};

		//--------------------------------------------------------------------------------
		// CSdtDocPart 17.5.2.12 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtDocPart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtDocPart)
			CSdtDocPart(OOX::Document *pMain = NULL);
			virtual ~CSdtDocPart();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			std::wstring toXML2(const std::wstring& sName) const;

			virtual EElementType getType() const;

		public:
 			nullable<ComplexTypes::Word::String> m_oDocPartCategory;
			nullable<ComplexTypes::Word::String> m_oDocPartGallery;
            nullable<ComplexTypes::Word::COnOff2> m_oDocPartUnique;
		};

		//--------------------------------------------------------------------------------
		// CSdtDropDownList 17.5.2.15 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtDropDownList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtDropDownList)
			CSdtDropDownList(OOX::Document *pMain = NULL);
			virtual ~CSdtDropDownList();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string									m_sLastValue;
			std::vector<ComplexTypes::Word::CSdtListItem*>	m_arrListItem;
		};

		//--------------------------------------------------------------------------------
		// CPlaceHolder 17.5.2.25 (Part 1)
		//--------------------------------------------------------------------------------
		class CPlaceHolder : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPlaceHolder)
			CPlaceHolder(OOX::Document *pMain = NULL);
			virtual ~CPlaceHolder();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			// Nodes
			nullable<ComplexTypes::Word::String> m_oDocPart;
		};

		//--------------------------------------------------------------------------------
		// SdtEndPr 17.5.2.37 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtEndPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtEndPr)
			CSdtEndPr(OOX::Document *pMain = NULL);
			virtual ~CSdtEndPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			nullable<OOX::Logic::CRunProperty> m_oRPr;
		};

		class CSdtCheckBoxSymbol : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtCheckBoxSymbol)
			CSdtCheckBoxSymbol(OOX::Document *pMain = NULL);
			virtual ~CSdtCheckBoxSymbol();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			std::wstring ToString() const;

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CShortHexNumber>		m_oVal;
			nullable<std::wstring>						m_oFont;
		};

		class CSdtCheckBox : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtCheckBox)
			CSdtCheckBox(OOX::Document *pMain = NULL);
			virtual ~CSdtCheckBox();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
            nullable<ComplexTypes::Word::COnOff2> m_oChecked;
			nullable<CSdtCheckBoxSymbol> m_oCheckedState;
			nullable<CSdtCheckBoxSymbol> m_oUncheckedState;

			nullable<ComplexTypes::Word::String> m_oGroupKey;//Not from specification
		};

		class CSdtPicture : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtPicture)
			CSdtPicture(OOX::Document *pMain = NULL);
			virtual ~CSdtPicture();
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_int m_oScaleFlag;
			nullable_bool m_oLockProportions;
			nullable_bool m_oRespectBorders;
			nullable_double m_oShiftX;
			nullable_double m_oShiftY;
		};

		//--------------------------------------------------------------------------------
		// SdtEndPr 17.5.2.38 (Part 1)
		//--------------------------------------------------------------------------------
		enum ESdtType
		{
			sdttypeUnknown      = -1,
			sdttypeBibliography =  0,
			sdttypeCitation     =  1,
			sdttypeComboBox     =  2,
			sdttypeDate         =  3,
			sdttypeDocPartList  =  4,
			sdttypeDocPartObj   =  5,
			sdttypeDropDownList =  6,
			sdttypeEquation     =  7,
			sdttypeGroup        =  8,
			sdttypePicture      =  9,
			sdttypeRichText     = 10,
			sdttypeText         = 11,
			sdttypeCheckBox     = 12
		};

		class CSdtPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtPr)
			CSdtPr(OOX::Document *pMain = NULL);
			virtual ~CSdtPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			std::wstring toXMLStart() const;
			std::wstring toXMLEnd() const;

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:

			ESdtType m_eType;

			nullable<ComplexTypes::Word::String> m_oAlias;
			nullable<ComplexTypes::Word::CSdtAppearance> m_oAppearance;
			nullable<OOX::Logic::CSdtComboBox> m_oComboBox;
			nullable<ComplexTypes::Word::CColor> m_oColor;
			nullable<ComplexTypes::Word::CDataBinding> m_oDataBinding;
			nullable<OOX::Logic::CDate> m_oDate;
			nullable<OOX::Logic::CSdtDocPart> m_oDocPartList;
			nullable<OOX::Logic::CSdtDocPart> m_oDocPartObj;
			nullable<OOX::Logic::CSdtDropDownList> m_oDropDownList;
			nullable<ComplexTypes::Word::CDecimalNumber> m_oId;
			nullable<ComplexTypes::Word::CDecimalNumber> m_oLabel;
			nullable<ComplexTypes::Word::CLock> m_oLock;
			nullable<OOX::Logic::CPlaceHolder> m_oPlaceHolder;
			nullable<OOX::Logic::CRunProperty> m_oRPr;
            nullable<ComplexTypes::Word::COnOff2> m_oShowingPlcHdr;
			nullable<ComplexTypes::Word::CUnsignedDecimalNumber> m_oTabIndex;
			nullable<ComplexTypes::Word::String> m_oTag;
            nullable<ComplexTypes::Word::COnOff2> m_oTemporary;
			nullable<ComplexTypes::Word::CSdtText> m_oText;
			nullable<CSdtCheckBox> m_oCheckbox;

			nullable<CFormPr> m_oFormPr; //Not from specification
			nullable<CTextFormPr> m_oTextFormPr; //Not from specification
			nullable<CSdtPicture> m_oPicture; //Not from specification
			nullable<CComplexFormPr> m_oComplexFormPr; // Not from specification
		};

		//--------------------------------------------------------------------------------
		// SdtContent 17.5.2.38 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtContent : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CSdtContent)
			CSdtContent(OOX::Document *pMain = NULL);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			// Childs
		};

		//--------------------------------------------------------------------------------
		// Sdt 17.5.2.29 - 17.5.2.32 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdt : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdt)
			CSdt(OOX::Document *pMain = NULL);
			virtual ~CSdt();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			nullable<OOX::Logic::CSdtContent> m_oSdtContent;
			nullable<OOX::Logic::CSdtEndPr  > m_oSdtEndPr;
			nullable<OOX::Logic::CSdtPr     > m_oSdtPr;
		};

	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SDT_INCLUDE_H_
