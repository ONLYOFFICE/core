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
#include "../Styles/dxf.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CFormulaCF : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFormulaCF)

			CFormulaCF();
			virtual ~CFormulaCF();
			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType () const;

			static const CFormulaCF Merge(const CFormulaCF& oPrev, const CFormulaCF& oCurrent);
			bool isExtended ();

			std::wstring m_sNodeName;
			std::wstring m_sText;
		};

		class CConditionalFormatValueObject : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CConditionalFormatValueObject)
            WritingElement_XlsbConstructors(CConditionalFormatValueObject)

			CConditionalFormatValueObject();
			virtual ~CConditionalFormatValueObject();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;
			bool isExtended ();

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff>						m_oGte;
			nullable<SimpleTypes::Spreadsheet::ST_CfvoType>		m_oType;
			nullable<std::wstring>								m_oVal;

			nullable<CFormulaCF>								m_oFormula;
		};

		class CConditionalFormatIconSet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CConditionalFormatIconSet)
            WritingElement_XlsbConstructors(CConditionalFormatIconSet)

			CConditionalFormatIconSet();
			virtual ~CConditionalFormatIconSet();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);

            void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

			nullable<SimpleTypes::Spreadsheet::ST_IconSetType>		m_oIconSet;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oIconId;
		};

		class CColorScale : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColorScale)
            WritingElement_XlsbConstructors(CColorScale)

			CColorScale();
			virtual ~CColorScale();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

			template<typename Type>
			static nullable<Type> Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent);
			static const CColorScale Merge(const CColorScale& oPrev, const CColorScale& oCurrent);
			bool isExtended ()
			{
				return false;
			}
			std::vector<nullable<CConditionalFormatValueObject>>	m_arrValues;
			std::vector<nullable<CColor>>							m_arrColors;
		};

		class CDataBar : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataBar)
            WritingElement_XlsbConstructors(CDataBar)

			CDataBar();
			virtual ~CDataBar();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			bool isExtended ();

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

			template<typename Type>
			static nullable<Type> Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent);
			static const CDataBar Merge(const CDataBar& oPrev, const CDataBar& oCurrent);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oMaxLength;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oMinLength;
			nullable<SimpleTypes::COnOff>					m_oShowValue;
			
			nullable<CColor>								m_oColor;

			std::vector<nullable<CConditionalFormatValueObject>> m_arrValues;

//---ext-----------
			nullable<SimpleTypes::Spreadsheet::ST_DataBarAxisPosition>		m_oAxisPosition;
			nullable<SimpleTypes::Spreadsheet::ST_DataBarDirection>			m_oDirection;
			nullable<SimpleTypes::COnOff>									m_oBorder;
			nullable<SimpleTypes::COnOff>									m_oGradient;
			nullable<SimpleTypes::COnOff>									m_oNegativeBarColorSameAsPositive;
			nullable<SimpleTypes::COnOff>									m_oNegativeBarBorderColorSameAsPositive;

			nullable<CColor>								m_oAxisColor;
			nullable<CColor>								m_oBorderColor;
			
			nullable<CColor>								m_oNegativeFillColor;
			nullable<CColor>								m_oNegativeBorderColor;
		};

		class CIconSet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CIconSet)
            WritingElement_XlsbConstructors(CIconSet)

			CIconSet();
			virtual ~CIconSet();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

			template<typename Type>
			static nullable<Type> Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent);
			static const CIconSet Merge(const CIconSet& oPrev, const CIconSet& oCurrent);

			bool isExtended ();

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff>							m_oPercent;
			nullable<SimpleTypes::COnOff>							m_oReverse;
			nullable<SimpleTypes::COnOff>							m_oShowValue;
			std::vector<nullable<CConditionalFormatValueObject>>	m_arrValues;

			nullable<SimpleTypes::Spreadsheet::ST_IconSetType>		m_oIconSet;
//---ext-----------
			nullable<SimpleTypes::COnOff>							m_oCustom;
			std::vector<nullable<CConditionalFormatIconSet>>		m_arrIconSets;
		};

		class CConditionalFormattingRule : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CConditionalFormattingRule)
            WritingElement_XlsbConstructors(CConditionalFormattingRule)

			CConditionalFormattingRule();
			virtual ~CConditionalFormattingRule();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			bool isExtended();

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;
			bool isValid () const;

			template<typename Type>
			static nullable<Type> Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent);
			static const CConditionalFormattingRule Merge(const CConditionalFormattingRule& oPrev, const CConditionalFormattingRule& oCurrent);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<CDxf>										m_oDxf;
			nullable<SimpleTypes::COnOff>						m_oAboveAverage;
			nullable<SimpleTypes::COnOff>						m_oBottom;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oDxfId;
			nullable<SimpleTypes::COnOff>						m_oEqualAverage;
			nullable<SimpleTypes::Spreadsheet::ST_CfOperator>	m_oOperator;
			nullable<SimpleTypes::COnOff>						m_oPercent;
			nullable<SimpleTypes::CDecimalNumber>				m_oPriority;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oRank;
			nullable<SimpleTypes::CDecimalNumber>				m_oStdDev;
			nullable<SimpleTypes::COnOff>						m_oStopIfTrue;
			nullable<std::wstring>								m_oId;
			nullable<std::wstring>								m_oText;
			nullable<SimpleTypes::Spreadsheet::ST_TimePeriod>	m_oTimePeriod;
			nullable<SimpleTypes::Spreadsheet::ST_CfType>		m_oType;
			
			nullable<OOX::Drawing::COfficeArtExtensionList>		m_oExtLst;
			nullable<std::wstring>								m_oExtId;

			nullable<CIconSet>					m_oIconSet;
			nullable<CColorScale>				m_oColorScale;
			nullable<CDataBar>					m_oDataBar;
			std::vector<nullable<CFormulaCF>>	m_arrFormula;

			bool bUsage;
		};

		class CConditionalFormatting  : public WritingElementWithChilds<CConditionalFormattingRule>
		{
		public:
			WritingElement_AdditionConstructors(CConditionalFormatting)
            WritingElement_XlsbConstructors(CConditionalFormatting)

			CConditionalFormatting(OOX::Document *pMain = NULL);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			bool IsExtended();

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;
			bool IsUsage();
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

			mutable bool m_bIsExtended;
			mutable bool m_bIsValid;
		public:

			nullable<SimpleTypes::COnOff>	m_oPivot;
			nullable<std::wstring >			m_oSqRef; // ToDo переделать на тип "sqref" (18.18.76) - последовательность "ref", разделенные пробелом
		};

	} //Spreadsheet
} // namespace OOX
