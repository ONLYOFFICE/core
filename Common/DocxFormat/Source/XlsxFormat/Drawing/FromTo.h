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
#ifndef OOX_FROMTO_FILE_INCLUDE_H_
#define OOX_FROMTO_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CFromTo : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFromTo)
			CFromTo()
			{
			}
			virtual ~CFromTo()
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
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
			{
				writer.WriteString(L"<");
				writer.WriteString(sName);
				writer.WriteString(L">");
				if(m_oCol.IsInit())
				{
					WritingStringValInt(L"xdr:col", m_oCol->GetValue());
				}
				if(m_oColOff.IsInit())
				{
					WritingStringValInt64(L"xdr:colOff", m_oColOff->ToEmu());
				}
				if(m_oRow.IsInit())
				{
					WritingStringValInt(L"xdr:row", m_oRow->GetValue());
				}
				if(m_oRowOff.IsInit())
				{
					WritingStringValInt64(L"xdr:rowOff", m_oRowOff->ToEmu());
				}
				writer.WriteString(L"</");
				writer.WriteString(sName);
				writer.WriteString(L">");
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("col") == sName )
						m_oCol = oReader.GetText2();
					else if ( _T("colOff") == sName )
						m_oColOff = oReader.GetText2();
					else if ( _T("row") == sName )
						m_oRow = oReader.GetText2();
					else if ( _T("rowOff") == sName )
						m_oRowOff = oReader.GetText2();
				}
			}

			virtual EElementType getType () const
			{
				return et_x_FromTo;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCol;
			nullable<SimpleTypes::CEmu>			m_oColOff;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRow;
			nullable<SimpleTypes::CEmu>			m_oRowOff;
		};
		class CExt : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExt)
			CExt()
			{
			}
			virtual ~CExt()
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
				writer.WriteString(_T("<xdr:ext"));
				WritingStringNullableAttrInt64(L"cx", m_oCx, m_oCx->ToEmu());
				WritingStringNullableAttrInt64(L"cy", m_oCy, m_oCy->ToEmu());
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_Ext;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("cx"),      m_oCx )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("cy"),      m_oCy )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CEmu>			m_oCx;
			nullable<SimpleTypes::CEmu>			m_oCy;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_FROMTO_FILE_INCLUDE_H_
