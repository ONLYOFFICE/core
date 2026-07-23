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
#include "oMathPara.h"
#include "../Logic/Run.h"

namespace OOX
{	
	namespace Logic
	{
		COMathPara::COMathPara(OOX::Document *pMain) : WritingElementWithChilds<WritingElement>(pMain)
		{
		}
		COMathPara::~COMathPara()
		{
		}
        std::wstring COMathPara::toXML() const
		{
            std::wstring sResult = _T("<m:oMathPara>");

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
                }
            }

			sResult += _T("</m:oMathPara>");

			return sResult;
		}
		void COMathPara::fromXML(XmlUtils::CXmlNode& oNode)
		{
			std::vector<XmlUtils::CXmlNode> oChilds;			
			if ( oNode.GetNodes( _T("*"), oChilds ) )
			{
				for ( size_t nIndex = 0; nIndex < oChilds.size(); nIndex++ )
				{
					XmlUtils::CXmlNode& oItem = oChilds[nIndex];
					if ( oItem.IsValid( ) )
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if ( _T("w:r") == sName )
							AssignPtrXmlContent(pItem, CRun, oItem)
						else if ( _T("m:oMath") == sName )
							AssignPtrXmlContent(pItem, COMath, oItem)
						else if ( _T("m:oMathParaPr") == sName )
							AssignPtrXmlContent(pItem, COMathParaPr, oItem)

						if ( pItem )
							m_arrItems.push_back( pItem );
					}
				}
			}
		}
		void COMathPara::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("m:oMath") == sName )
					AssignPtrXmlContent(pItem, COMath, oReader)
				else if ( _T("m:oMathParaPr") == sName )
					AssignPtrXmlContent(pItem, COMathParaPr, oReader)
				else if ( _T("w:r") == sName )
					AssignPtrXmlContent(pItem, CRun, oReader)

				if ( pItem )
					m_arrItems.push_back( pItem );

			}
		}
		EElementType COMathPara::getType() const
		{
			return et_m_oMathPara;
		}
	}//namespace Logic
}//namespace OOX
