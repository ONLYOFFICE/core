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

#include "../ComplexTypes_Spreadsheet.h"
#include "../WritingElement.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"
#include "../../Common/SimpleTypes_Shared.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CDataValidationFormula : public WritingElement //тоже что и CFormulaCF
		{
		public:
			WritingElement_AdditionConstructors(CDataValidationFormula)
			CDataValidationFormula(OOX::Document *pMain = NULL);
			virtual ~CDataValidationFormula();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;			
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);				
			virtual EElementType getType () const;

			std::wstring m_sNodeName;
			std::wstring m_sText;
		};

		class CDataValidation : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataValidation)
            WritingElement_XlsbConstructors(CDataValidation)
			CDataValidation(OOX::Document *pMain = NULL);
			virtual ~CDataValidation();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;

			bool IsExtended();
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

			nullable<SimpleTypes::CUnsignedDecimalNumber>					m_oSpinCount;

			nullable<SimpleTypes::Spreadsheet::CDataValidationType>			m_oType;
			nullable<SimpleTypes::COnOff>									m_oAllowBlank;
			nullable_string													m_oError;
			nullable_string													m_oErrorTitle;
			nullable<SimpleTypes::Spreadsheet::CDataValidationErrorStyle>	m_oErrorStyle;
			nullable<SimpleTypes::Spreadsheet::CDataValidationImeMode>		m_oImeMode;
			nullable<SimpleTypes::Spreadsheet::CDataValidationOperator>		m_oOperator;
			nullable_string													m_oPrompt;
			nullable_string													m_oPromptTitle;
			nullable<SimpleTypes::COnOff>									m_oShowDropDown;
			nullable<SimpleTypes::COnOff>									m_oShowErrorMessage;
			nullable<SimpleTypes::COnOff>									m_oShowInputMessage;

			nullable_string									m_oSqRef; // ToDo переделать на тип "sqref" (18.18.76) - последовательность "ref", разделенные пробелом

			mutable nullable_string							m_oUuid;
			nullable<CDataValidationFormula>				m_oFormula1;
			nullable<CDataValidationFormula>				m_oFormula2;
		};

		class CDataValidations : public WritingElementWithChilds<CDataValidation>
		{
		public:
			WritingElement_AdditionConstructors(CDataValidations)
            WritingElement_XlsbConstructors(CDataValidations)
			CDataValidations(OOX::Document *pMain = NULL);
			virtual ~CDataValidations();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
					void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;

            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			mutable nullable_int							m_oCount;
			nullable<SimpleTypes::COnOff>					m_oDisablePrompts;
			nullable<SimpleTypes::CDecimalNumber>			m_oXWindow;
			nullable<SimpleTypes::CDecimalNumber>			m_oYWindow;
		};

	} //Spreadsheet
} // namespace OOX
