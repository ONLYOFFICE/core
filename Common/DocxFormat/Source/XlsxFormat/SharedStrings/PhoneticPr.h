/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef OOX_PHONETIC_FILE_INCLUDE_H_
#define OOX_PHONETIC_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "Text.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		class CPhonetic : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CPhonetic)
			CPhonetic()
			{
			}
			virtual ~CPhonetic()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual EElementType getType () const
			{
				return et_PhoneticPr;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("alignment"),      m_oAlignment )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("fontId"),      m_oFontId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("type"),      m_oType )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
				nullable<SimpleTypes::Spreadsheet::CPhoneticAlignment<>>	m_oAlignment;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>				m_oFontId;
				nullable<SimpleTypes::Spreadsheet::CPhoneticType<>>			m_oType;
		};

		class CRPh : public WritingElementWithChilds<CText>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CRPh)
			CRPh()
			{
			}
			virtual ~CRPh()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("t") == sName )
						m_arrItems.push_back( new CText( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_rPh;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("eb"),      m_oEb )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("sb"),      m_oSb )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oEb;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oSb;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_PHONETIC_FILE_INCLUDE_H_