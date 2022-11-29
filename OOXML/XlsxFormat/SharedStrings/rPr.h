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

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/ComplexTypes.h"

#include "../../Common/SimpleTypes_Spreadsheet.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CColor
		{
		public:
			WritingElement_AdditionConstructors(CColor)
			CColor();
			virtual ~CColor();
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff<>>						m_oAuto;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oIndexed;
			nullable_string										m_oRgb;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oTheme;
			nullable<SimpleTypes::CDouble>						m_oTint;
		};

		//необработано:
		class CRPr : public WritingElementWithChilds
		{
		public:
			WritingElement_AdditionConstructors(CRPr)
			CRPr();
			virtual ~CRPr();

		public:
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<ComplexTypes::Word::COnOff2 >					m_oBold;
			nullable<ComplexTypes::Word::COnOff2 >					m_oCharset;
			nullable<CColor>										m_oColor;
			nullable<ComplexTypes::Word::COnOff2 >					m_oCondense;
			nullable<ComplexTypes::Word::COnOff2 >					m_oExtend;
			//nullable<SimpleTypes::Spreadsheet::CFontFamily<> >	m_oFamily;
			nullable<ComplexTypes::Word::COnOff2 >					m_oItalic;
			nullable<ComplexTypes::Word::COnOff2 >					m_oOutline;
			nullable<ComplexTypes::Word::String >					m_oRFont;
			//nullable<SimpleTypes::Spreadsheet::CFontScheme<> >	m_oScheme;
			nullable<ComplexTypes::Word::COnOff2 >					m_oShadow;
			nullable<ComplexTypes::Word::COnOff2 >					m_oStrike;
			nullable<ComplexTypes::Word::CDouble2>					m_oSz;
			nullable<SimpleTypes::Spreadsheet::CUnderline >			m_oUnderline;
			nullable<SimpleTypes::CVerticalAlignRun >				m_oVertAlign;
		};
	} //Spreadsheet
} // namespace OOX
