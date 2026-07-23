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
#include "AlternateContent.h"
#include "Run.h"

namespace OOX
{
	namespace Logic
	{
		CAlternateContent::CAlternateContent(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CAlternateContent::~CAlternateContent()
		{
			Clear();
		}

		void CAlternateContent::Clear()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrChoiceItems.size(); nIndex++ )
			{
				if ( m_arrChoiceItems[nIndex] )
					delete m_arrChoiceItems[nIndex];
				m_arrChoiceItems[nIndex] = NULL;
			}
			m_arrChoiceItems.clear();
			for (unsigned int nIndex = 0; nIndex < m_arrFallbackItems.size(); nIndex++ )
			{
				if ( m_arrFallbackItems[nIndex] )
					delete m_arrFallbackItems[nIndex];
				m_arrFallbackItems[nIndex] = NULL;
			}
			m_arrFallbackItems.clear();
		}

		void CAlternateContent::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CAlternateContent::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"mc:Choice" == sName )
				{
					ReadAttributes(oReader, m_oChoiceRequires);

					CRun altRun;
					altRun = oReader;
					
                    for ( size_t i = 0; i < altRun.m_arrItems.size(); ++i)
                    {
                        if ( altRun.m_arrItems[i] )
						{			
                            m_arrChoiceItems.push_back(altRun.m_arrItems[i]);
						}
					}
					
					//without RemoveAll they will be cleared together with altRun
					altRun.m_arrItems.clear();
				}
				else if ( L"mc:Fallback" == sName )
				{
					//if (m_oChoiceRequires.is_init() && !m_arrChoiceItems.empty())
					//{
					//	continue; // no need to read ballast - take the more modern or original version.
					//}
					
					CRun altRun;
					altRun = oReader;
					
                    for ( size_t i = 0; i < altRun.m_arrItems.size(); ++i)
                    {
                        if ( altRun.m_arrItems[i] )
                        {
                            m_arrFallbackItems.push_back(altRun.m_arrItems[i]);
						}
					}
					
					//without RemoveAll they will be cleared together with altRun
					altRun.m_arrItems.clear();
				}
			}
		}

		std::wstring CAlternateContent::toXML() const
		{
			return _T("");
		}
		EElementType CAlternateContent::getType() const
		{
			return OOX::et_mc_alternateContent;
		}

		void CAlternateContent::ReadAttributes(XmlUtils::CXmlLiteReader& oReader, nullable_string & oRequires)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, (L"Requires"), oRequires )
			WritingElement_ReadAttributes_End( oReader )

		}
	} // namespace Words
} // namespace OOX
