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
	class CDouble;
	class CUnsignedDecimalNumber;

	namespace Spreadsheet
	{
		class CCalcMode;
		class CRefMode;
	}
}

namespace OOX
{
	namespace Spreadsheet
	{
    class CCalcPr : public WritingElement
    {
    public:
        WritingElement_AdditionMethods(CCalcPr)
        WritingElement_XlsbConstructors(CCalcPr)
			CCalcPr();
			virtual ~CCalcPr();

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
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oCalcId;
			nullable<SimpleTypes::Spreadsheet::CCalcMode>		m_oCalcMode;
			nullable<SimpleTypes::COnOff>						m_oFullCalcOnLoad;
			nullable<SimpleTypes::Spreadsheet::CRefMode>		m_oRefMode;
			nullable<SimpleTypes::COnOff>						m_oIterate;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oIterateCount;
			nullable<SimpleTypes::CDouble>						m_oIterateDelta;
			nullable<SimpleTypes::COnOff>						m_oFullPrecision;
			nullable<SimpleTypes::COnOff>						m_oCalcCompleted;
			nullable<SimpleTypes::COnOff>						m_oCalcOnSave;
			nullable<SimpleTypes::COnOff>						m_oConcurrentCalc;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oConcurrentManualCount;
			nullable<SimpleTypes::COnOff>						m_oForceFullCalc;
		};

	} //Spreadsheet
} // namespace OOX

