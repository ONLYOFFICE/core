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
//Generated code
#pragma once

#include "../WritingElement.h"
#include "../../Base/Nullable.h"

#include "../../PPTXFormat/Logic/StyleRef.h"
#include "../../PPTXFormat/Logic/FontRef.h"

namespace SimpleTypes
{
	namespace Spreadsheet
	{
		class CChartSymbol;
	}
}

namespace PPTX
{
	namespace Logic
	{
		class RunProperties;
		class SpPr;
		class BodyPr;
	}
}

namespace OOX
{
namespace Drawing
{
	class COfficeArtExtensionList;
}

namespace Spreadsheet
{
namespace ChartEx
{
	class CStyleEntry : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CStyleEntry)

		CStyleEntry();

		virtual void fromXML(XmlUtils::CXmlNode& node);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

		virtual std::wstring toXML() const;
		virtual EElementType getType();

		BYTE getTypeStyleEntry();
		void setTypeStyleEntry(BYTE type);

		nullable_double			m_lineWidthScale;
		PPTX::Logic::StyleRef	m_lnRef;
		PPTX::Logic::StyleRef	m_fillRef;
		PPTX::Logic::StyleRef	m_effectRef;
		PPTX::Logic::FontRef	m_fontRef;

		nullable<PPTX::Logic::RunProperties>	m_defRPr;
		nullable<PPTX::Logic::BodyPr>			m_bodyPr;
		nullable<PPTX::Logic::SpPr>				m_spPr;

		nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;

	private:
		std::wstring m_name;
	};

//------------------------------------------------------------------------------
	class CMarkerLayout : public WritingElement
	{
	public:
		WritingElement_AdditionMethods(CMarkerLayout)

		CMarkerLayout();

		virtual void fromXML(XmlUtils::CXmlNode& node);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

		virtual std::wstring toXML() const;
		virtual EElementType getType();

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<SimpleTypes::Spreadsheet::CChartSymbol> m_symbol;
		nullable_uint m_size;
	};

//------------------------------------------------------------------------------
	class CChartStyle : public WritingElement
	{
	public:
		CChartStyle();
		virtual ~CChartStyle();

		WritingElement_AdditionMethods(CChartStyle)

		virtual void fromXML(XmlUtils::CXmlNode& node);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable_uint m_id;
		std::vector<CStyleEntry*> m_arStyleEntries;
		nullable<CMarkerLayout>	m_dataPointMarkerLayout;
		nullable<OOX::Drawing::COfficeArtExtensionList> m_extLst;
	};
}
}
}

