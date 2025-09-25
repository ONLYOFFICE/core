﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
		//необработано:
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
			std::vector<XLS::BaseObjectPtr> AddFutureFunctions(const _UINT32 namesStart);
			virtual EElementType getType () const;

		private:
			XLS::BaseObjectPtr createFutureFunction(const std::wstring& funcName);
			XLS::BaseObjectPtr createCustomFunction(const std::wstring& custom);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

	} //Spreadsheet
} // namespace OOX
