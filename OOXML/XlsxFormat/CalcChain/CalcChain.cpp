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

#include "CalcChain.h"
#include "../../Common/SimpleTypes_Shared.h"

namespace OOX
{
	namespace Spreadsheet
	{
			CCalcCell::CCalcCell()
			{
			}
			CCalcCell::~CCalcCell()
			{
			}
			void CCalcCell::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CCalcCell::toXML() const
			{
				return _T("");
			}
			void CCalcCell::toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			void CCalcCell::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			EElementType CCalcCell::getType () const
			{
				return et_x_CalcCell;
			}
			void CCalcCell::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("a"),      m_oArray )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("i"),      m_oSheetId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("l"),      m_oDependencyLevel )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("s"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("t"),      m_oRef )

					WritingElement_ReadAttributes_End( oReader )
			}

			CCalcChain::CCalcChain(OOX::Document* pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;

				CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
				if (xlsx && xlsx->m_bNeedCalcChain) xlsx->m_pCalcChain = this;
			}
			CCalcChain::CCalcChain(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;

				CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
				if (xlsx)
				{
					if(xlsx->m_bNeedCalcChain)
					{
						xlsx->m_pCalcChain = this;
						read( oRootPath, oPath );
					}
				}
				else
				{
					read( oRootPath, oPath );
				}
			}
			CCalcChain::~CCalcChain()
			{
				ClearItems();
			}
			void CCalcChain::read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			void CCalcChain::read(const CPath& oRootPath, const CPath& oPath)
			{
				m_oReadPath = oPath;
				IFileContainer::Read( oRootPath, oPath );

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;

				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if ( _T("calcChain") == sName )
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
					{
						int nStylesDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
						{
							sName = XmlUtils::GetNameNoNS(oReader.GetName());

							if ( _T("c") == sName )
							{
								CCalcCell* pCalcCell = new CCalcCell();
								*pCalcCell = oReader;
								m_arrItems.push_back(pCalcCell);
							}
						}
					}
				}
			}
			void CCalcChain::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
			}
			const OOX::FileType CCalcChain::type() const
			{
				return OOX::Spreadsheet::FileTypes::CalcChain;
			}
			const CPath CCalcChain::DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			const CPath CCalcChain::DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& CCalcChain::GetReadPath()
			{
				return m_oReadPath;
			}
			void CCalcChain::ClearItems()
			{
				for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if ( m_arrItems[i] )delete m_arrItems[i];
				}

				m_arrItems.clear();
			}
			void CCalcChain::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

	} //Spreadsheet
} // namespace OOX
