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
//Generated code
#pragma once

#include "../../Base/Nullable.h"
#include "../../DocxFormat/Drawing/DrawingExt.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"
#include "../WritingElement.h"
#include "../../PPTXFormat/Logic/StyleRef.h"
#include "../../PPTXFormat/Logic/FontRef.h"
#include "../../PPTXFormat/Logic/RunProperties.h"
#include "../../PPTXFormat/Logic/SpPr.h"
#include "../../PPTXFormat/Logic/BodyPr.h"

namespace OOX
{
namespace Spreadsheet
{
namespace ChartEx
{
	class CStyleEntry : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CStyleEntry)

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
		WritingElement_AdditionConstructors(CMarkerLayout)

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

		WritingElement_AdditionConstructors(CChartStyle)

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

