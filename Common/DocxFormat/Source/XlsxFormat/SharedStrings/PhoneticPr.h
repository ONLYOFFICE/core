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
#ifndef OOX_PHONETIC_FILE_INCLUDE_H_
#define OOX_PHONETIC_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "Text.h"
#include "../../XlsbFormat/Biff12_structures/PhRun.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		class CPhonetic : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPhonetic)
			CPhonetic()
			{
			}
			virtual ~CPhonetic()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BiffStructure& obj)
            {
                ReadAttributes(obj);
            }
			virtual EElementType getType () const
			{
				return et_x_PhoneticPr;
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

            void ReadAttributes(XLS::BiffStructure& obj)
            {
                auto ptr = static_cast<XLSB::PhRun*>(&obj);

                if(ptr != nullptr)
                {
                    switch(ptr->alcH)
                    {
                        case 0:
                            m_oAlignment = SimpleTypes::Spreadsheet::phoneticalignmentNoControl;
                            break;
                        case 1:
                            m_oAlignment = SimpleTypes::Spreadsheet::phoneticalignmentLeft;
                            break;
                        case 2:
                            m_oAlignment = SimpleTypes::Spreadsheet::phoneticalignmentCenter;
                            break;
                        case 3:
                            m_oAlignment = SimpleTypes::Spreadsheet::phoneticalignmentDistributed;
                            break;
                    }

                    switch(ptr->phType)
                    {
                        case 0:
                            m_oType = SimpleTypes::Spreadsheet::phonetictypeHalfwidthKatakana;
                            break;
                        case 1:
                            m_oType = SimpleTypes::Spreadsheet::phonetictypeFullwidthKatakana;
                            break;
                        case 2:
                            m_oType = SimpleTypes::Spreadsheet::phonetictypeHiragana;
                            break;
                        case 3:
                            m_oType = SimpleTypes::Spreadsheet::phonetictypeNoConversion;
                            break;
                    }

                    m_oFontId = ptr->ifnt;

                }
            }
		public:
				nullable<SimpleTypes::Spreadsheet::CPhoneticAlignment<>>	m_oAlignment;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>				m_oFontId;
				nullable<SimpleTypes::Spreadsheet::CPhoneticType<>>			m_oType;
		};

		class CRPh : public WritingElementWithChilds<CText>
		{
		public:
			WritingElement_AdditionConstructors(CRPh)
			CRPh()
			{
			}
			virtual ~CRPh()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("t") == sName )
						m_arrItems.push_back( new CText( oReader ));
				}
			}

            void fromBin(XLS::BiffStructure& obj, std::wstring& str)
            {
                auto ptr = new CText();
                ptr->fromBin(str);
                m_arrItems.push_back(ptr);
                ReadAttributes(obj);
            }

			virtual EElementType getType () const
			{
				return et_x_rPh;
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

            void ReadAttributes(XLS::BiffStructure& obj)
            {
                auto ptr = static_cast<XLSB::PhRun*>(&obj);

                if(ptr != nullptr)
                {
                    m_oEb = ptr->ichMom;
                    m_oSb = ptr->ichFirst;
                }
            }
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oEb;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oSb;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_PHONETIC_FILE_INCLUDE_H_
