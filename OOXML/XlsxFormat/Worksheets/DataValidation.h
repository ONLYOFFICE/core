/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../WritingElement.h"
#include "../../Base/Nullable.h"

namespace SimpleTypes
{
	class COnOff;
	class CDecimalNumber;
	class CUnsignedDecimalNumber;
	class CGuid;

	namespace Spreadsheet
	{
		class CDataValidationType;
		class CDataValidationErrorStyle;
		class CDataValidationImeMode;
		class CDataValidationOperator;
	}
}

namespace OOX
{
	namespace Spreadsheet
	{
		class CDataValidationFormula : public WritingElement //same as CFormulaCF
		{
		public:
			WritingElement_AdditionMethods(CDataValidationFormula)
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
			WritingElement_AdditionMethods(CDataValidation)
            WritingElement_XlsbConstructors(CDataValidation)
			CDataValidation(OOX::Document *pMain = NULL);
			virtual ~CDataValidation();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			void CreateElements(XmlUtils::CXmlLiteReader& oReader, int Depth);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;

			bool IsExtended();
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toXLS();
            void toBin(XLS::StreamCacheWriterPtr& writer);
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

			nullable_string									m_oSqRef; // ToDo convert to "sqref" type (18.18.76) - sequence of "ref" separated by space
			nullable_string									m_oList;
			mutable nullable<SimpleTypes::CGuid>			m_oUid;
			nullable<CDataValidationFormula>				m_oFormula1;
			nullable<CDataValidationFormula>				m_oFormula2;
		};

		class CDataValidations : public WritingElementWithChilds<CDataValidation>
		{
		public:
			WritingElement_AdditionMethods(CDataValidations)
            WritingElement_XlsbConstructors(CDataValidations)
			CDataValidations(OOX::Document *pMain = NULL);
			virtual ~CDataValidations();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
					void toXML2(NSStringUtils::CStringBuilder& writer, bool bExtendedWrite) const;

            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toXLS();
            void toBin(XLS::StreamCacheWriterPtr& writer);
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
