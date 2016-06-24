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
#ifndef OOX_NUMFMTS_FILE_INCLUDE_H_
#define OOX_NUMFMTS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CNumFmt : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CNumFmt)
			CNumFmt()
			{
			}
			virtual ~CNumFmt()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
                CString sHeader = _T("numFmt");
                toXML2(writer, sHeader);
			}
			void toXML2(XmlUtils::CStringWriter& writer, CString& sHeader) const
			{
				writer.WriteString(_T("<") + sHeader);
				if(m_oNumFmtId.IsInit())
				{
					CString sVal;sVal.Format(_T(" numFmtId=\"%d\""), m_oNumFmtId->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oFormatCode.IsInit())
				{
                    //CString sVal;sVal.Format(_T(" formatCode=\"%ls\""), XmlUtils::EncodeXmlString(m_oFormatCode.get()));
                    
                    CString sVal(_T(" formatCode=\""));
                    sVal += XmlUtils::EncodeXmlString(m_oFormatCode.get());
                    sVal += CString(_T("\""));
                    
                    writer.WriteString(sVal);
				}
				if(m_oSourceLinked.IsInit())
				{
					CString sVal;
                    sVal.Format(_T(" sourceLinked=\"%ls\""), m_oSourceLinked->ToString2(SimpleTypes::onofftostring1));
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
				return et_NumFmt;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("formatCode"),      m_oFormatCode )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("numFmtId"),      m_oNumFmtId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("sourceLinked"),      m_oSourceLinked )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<CString >								m_oFormatCode;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oNumFmtId;
			nullable<SimpleTypes::COnOff<>>				m_oSourceLinked;
		};
		class CNumFmts : public WritingElementWithChilds<CNumFmt>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CNumFmts)
			CNumFmts()
			{
			}
			virtual ~CNumFmts()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_arrItems.size() > 0 )
				{
					writer.WriteString(_T("<numFmts"));
					if(m_oCount.IsInit())
					{
						CString sVal;sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
						writer.WriteString(sVal);
					}
					writer.WriteString(_T(">"));
					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(_T("</numFmts>"));
				}
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

					if ( _T("numFmt") == sName )
						m_arrItems.push_back( new CNumFmt( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_NumFmts;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_NUMFMTS_FILE_INCLUDE_H_
