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
#ifndef OOX_HYPERLINKS_FILE_INCLUDE_H_
#define OOX_HYPERLINKS_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CHyperlink : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CHyperlink)
			CHyperlink()
			{
			}
			virtual ~CHyperlink()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<hyperlink"));
				if(m_oDisplay.IsInit())
				{
					CString sVal;
					sVal.Append(_T(" display=\""));
					sVal.Append(XmlUtils::EncodeXmlString(m_oDisplay.get()));
					sVal.Append(_T("\""));
					writer.WriteString(sVal);
				}
				if(m_oRid.IsInit())
				{
					CString sVal; sVal.Format(_T(" r:id=\"%ls\""), m_oRid->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oLocation.IsInit())
				{
					CString sVal;
					sVal.Append(_T(" location=\""));
					sVal.Append(XmlUtils::EncodeXmlString(m_oLocation.get()));
					sVal.Append(_T("\""));
					writer.WriteString(sVal);
				}
				if(m_oRef.IsInit())
				{
					CString sVal; sVal.Format(_T(" ref=\"%ls\""), XmlUtils::EncodeXmlString(m_oRef.get()));
					writer.WriteString(sVal);
				}
				if(m_oTooltip.IsInit())
				{
					CString sVal;
					sVal.Append(_T(" tooltip=\""));
					sVal.Append(XmlUtils::EncodeXmlString(m_oTooltip.get()));
					sVal.Append(_T("\""));
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
				return et_Hyperlink;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("display"),      m_oDisplay)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),      m_oRid )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("location"),      m_oLocation )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("tooltip"),      m_oTooltip )

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<CString>						m_oDisplay;
				nullable<SimpleTypes::CRelationshipId>	m_oRid;
				nullable<CString>						m_oLocation;
				nullable<CString>						m_oRef;
				nullable<CString>						m_oTooltip;
		};

		class CHyperlinks  : public WritingElementWithChilds<CHyperlink>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CHyperlinks)
			CHyperlinks()
			{
			}
			virtual ~CHyperlinks()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_arrItems.size() > 0)
				{
					writer.WriteString(_T("<hyperlinks>"));
					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(_T("</hyperlinks>"));
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

					if ( _T("hyperlink") == sName )
						m_arrItems.push_back( new CHyperlink( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_Hyperlinks;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_HYPERLINKS_FILE_INCLUDE_H_