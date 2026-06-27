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
	class CUnsignedDecimalNumber;
}

namespace OOX
{
	namespace Spreadsheet
	{
		//not implemented:
		//<extLst>
		class CDefinedName : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDefinedName)
			CDefinedName(OOX::Document *pMain = NULL);
			virtual ~CDefinedName();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toXLS();
			virtual EElementType getType () const;

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
				nullable_string									m_oComment;
				nullable_string									m_oCustomMenu;
				nullable_string									m_oDescription;
				nullable<SimpleTypes::COnOff>					m_oFunction;
				nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oFunctionGroupId;
				nullable_string									m_oHelp;
				nullable<SimpleTypes::COnOff>					m_oHidden;
				nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oLocalSheetId;
				nullable_string									m_oName;
				nullable<SimpleTypes::COnOff>					m_oPublishToServer;
				nullable_string									m_oShortcutKey;
				nullable_string									m_oStatusBar;
				nullable<SimpleTypes::COnOff>					m_oVbProcedure;
				nullable<SimpleTypes::COnOff>					m_oWorkbookParameter;
				nullable<SimpleTypes::COnOff>					m_oXlm;

				nullable_string									m_oRef;
		};

		class CDefinedNames : public WritingElementWithChilds<CDefinedName>
		{
		public:
			WritingElement_AdditionMethods(CDefinedNames)
            WritingElement_XlsbVectorConstructors(CDefinedNames)
			CDefinedNames(OOX::Document *pMain = NULL);
			virtual ~CDefinedNames();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
			std::vector<XLS::BaseObjectPtr> toBin();
			void toXLS(XLS::BaseObjectPtr substream);
			std::vector<XLS::BaseObjectPtr> AddFutureFunctions(const _UINT32 namesStart, bool xls = false);
			virtual EElementType getType () const;

		private:
			XLS::BaseObjectPtr createFutureFunction(const std::wstring& funcName);
			XLS::BaseObjectPtr createCustomFunctionXLS(const std::wstring& funcName);
			XLS::BaseObjectPtr createCustomFunction(const std::wstring& custom);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

	} //Spreadsheet
} // namespace OOX
