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

#include "Run.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CRun::CRun()
		{
		}
		CRun::~CRun()
		{
		}
		void CRun::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CRun::toXML() const
		{
			return _T("");
		}
		void CRun::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<r>"));
			if(m_oRPr.IsInit())
				m_oRPr->toXML(writer);

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString(_T("</r>"));
		}
		void CRun::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("rPr") == sName )
					m_oRPr = oReader;
				else if ( _T("t") == sName )
					m_arrItems.push_back( new CText( oReader ));
			}
		}
		void CRun::fromBin(std::wstring& str, unsigned short fontindex)
		{
			auto ptr = new CText();
			ptr->fromBin(str);
			m_arrItems.push_back(ptr);

			m_oRPr.Init();
			m_oRPr->m_nFontIndex.Init();
			m_oRPr->m_nFontIndex = fontindex;
		}
		EElementType CRun::getType () const
		{
			return et_x_r;
		}
		void CRun::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

	} //Spreadsheet
} // namespace OOX
