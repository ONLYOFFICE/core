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
			WritingElementSpreadsheet_AdditionConstructors(CFromTo)
			CFromTo()
			{
			}
			virtual ~CFromTo()
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
			virtual void toXML2(XmlUtils::CStringWriter& writer, CString sName) const
			{
				CString sStart;sStart.Format(_T("<%ls>"), sName);
				writer.WriteString(sStart);
				if(m_oCol.IsInit())
				{
					CString sVal;sVal.Format(_T("<xdr:col>%d</xdr:col>"), m_oCol->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oColOff.IsInit())
				{
					CString sVal;sVal.Format(_T("<xdr:colOff>%lld</xdr:colOff>"), m_oColOff->ToEmu());
					writer.WriteString(sVal);
				}
				if(m_oRow.IsInit())
				{
					CString sVal;sVal.Format(_T("<xdr:row>%d</xdr:row>"), m_oRow->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oRowOff.IsInit())
				{
					CString sVal;sVal.Format(_T("<xdr:rowOff>%lld</xdr:rowOff>"), m_oRowOff->ToEmu());
					writer.WriteString(sVal);
				}
				CString sEnd;sEnd.Format(_T("</%ls>"), sName);
				writer.WriteString(sEnd);
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

					if ( _T("col") == sName )
						m_oCol = oReader.GetText2().GetString();
					else if ( _T("colOff") == sName )
						m_oColOff = oReader.GetText2().GetString();
					else if ( _T("row") == sName )
						m_oRow = oReader.GetText2().GetString();
					else if ( _T("rowOff") == sName )
						m_oRowOff = oReader.GetText2().GetString();
				}
			}

			virtual EElementType getType () const
			{
				return et_FromTo;
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
			WritingElementSpreadsheet_AdditionConstructors(CExt)
			CExt()
			{
			}
			virtual ~CExt()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<xdr:ext"));
				if(m_oCx.IsInit())
				{
					CString sVal;sVal.Format(_T(" cx=\"%lld\""), m_oCx->ToEmu());
					writer.WriteString(sVal);
				}
				if(m_oCy.IsInit())
				{
					CString sVal;sVal.Format(_T(" cy=\"%lld\""), m_oCy->ToEmu());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_Ext;
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