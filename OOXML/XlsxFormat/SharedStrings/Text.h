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
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CStringXLSB
		{
		public:
			CStringXLSB(_UINT32 nSize);
			~CStringXLSB();
			void Clean();
			void fromXML(XmlUtils::CXmlLiteReader& oReader, bool bPreserve);
			void fromStringA(const char* sVal);
			LONG getUTF16Size();

		public:
			WCHAR* m_sBuffer;
			LONG m_nSize;
			LONG m_nLen;

		protected:
			void checkBufferSize(_UINT32 nRequired);
		};

		class CTextXLSB
		{
		public:
			CTextXLSB(_UINT32 nSize);
			void Clean();
			void fromXML(XmlUtils::CXmlLiteReader& oReader, SimpleTypes::Spreadsheet::ECellTypeType eType);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			bool m_bIsInit;
			SimpleTypes::CXmlSpace m_oSpace;

			double m_dValue;
			_UINT32 m_nValue;
			CStringXLSB m_oValue;
		};

		//not implemented:
		class CText : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CText)
			CText(OOX::Document *pMain = NULL);
			virtual ~CText();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, const wchar_t* name) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(std::wstring& str);

			static void trimString(std::wstring& sVal, SimpleTypes::EXmlSpace eSpace);
			std::wstring ToString() const;

			SimpleTypes::EXmlSpace GetSpace() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CXmlSpace>	m_oSpace;
			std::wstring						m_sText;

		};
	} //Spreadsheet
} // namespace OOX
