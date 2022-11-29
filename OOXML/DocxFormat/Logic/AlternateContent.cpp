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

					CRun altRun(oReader);
					
                    for ( size_t i = 0; i < altRun.m_arrItems.size(); ++i)
                    {
                        if ( altRun.m_arrItems[i] )
						{			
                            m_arrChoiceItems.push_back(altRun.m_arrItems[i]);
						}
					}
					
					//без RemoveAll они очистятся вместе с altRun
					altRun.m_arrItems.clear();
				}
				else if ( L"mc:Fallback" == sName )
				{
					//if (m_oChoiceRequires.is_init() && !m_arrChoiceItems.empty())
					//{
					//	continue; // не зачем баласт читать - берем более современную или оригинальную версию.
					//}
					
					CRun altRun(oReader);
					
                    for ( size_t i = 0; i < altRun.m_arrItems.size(); ++i)
                    {
                        if ( altRun.m_arrItems[i] )
                        {
                            m_arrFallbackItems.push_back(altRun.m_arrItems[i]);
						}
					}
					
					//без RemoveAll они очистятся вместе с altRun
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
