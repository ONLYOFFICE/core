﻿/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#ifndef OOX_SHAREDSTRINGS_FILE_INCLUDE_H_
#define OOX_SHAREDSTRINGS_FILE_INCLUDE_H_

#include "../Xlsx.h"
#include "../CommonInclude.h"

#include "Si.h"
#include <map>

namespace OOX
{
	namespace Spreadsheet
	{
		class CSharedStrings : public OOX::File, public OOX::IFileContainer
		{
		public:
			CSharedStrings(OOX::Document* pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
			{
                m_nCount        = 0;
				m_bSpreadsheets = true;

				CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
				if ((xlsx) && (!xlsx->m_pSharedStrings))
					xlsx->m_pSharedStrings = this;
			}
			CSharedStrings(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
			{
                m_nCount        = 0;
                m_bSpreadsheets = true;

  				CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
				if ((xlsx) && (!xlsx->m_pSharedStrings))
					xlsx->m_pSharedStrings = this;

				read( oRootPath, oPath );
			}
			virtual ~CSharedStrings()
			{
				ClearItems();
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
			{
				m_oReadPath = oPath;
				IFileContainer::Read( oRootPath, oPath );

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( m_oReadPath.GetPath() ) )
				{
					//test un-upper(lower)case open - CALACATA GREECE.xlsx
					if (!m_oReadPath.FileInDirectoryCorrect())
						return;
					if ( !oReader.FromFile( m_oReadPath.GetPath() ) )
						return;
				}

				if ( !oReader.ReadNextNode() )
					return;

				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if ( _T("sst") == sName )
				{
					ReadAttributes( oReader );

					m_nCount = 0;

					if ( !oReader.IsEmptyNode() )
					{
						int nSharedStringsDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nSharedStringsDepth ) )
						{
							sName = XmlUtils::GetNameNoNS(oReader.GetName());

							if ( _T("si") == sName )
							{
								CSi* pItem = new CSi( oReader );
                                m_arrItems.push_back(pItem );
								m_nCount++;
							}
						}
					}
				}		
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				NSStringUtils::CStringBuilder writer;
				writer.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><sst xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\""));
				WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
				WritingStringNullableAttrInt(L"uniqueCount", m_oUniqueCount, m_oUniqueCount->GetValue());
				writer.WriteString(_T(">"));

                for(size_t i = 0; i < m_arrItems.size(); i++)
				{
                    m_arrItems[i]->toXML(writer);
				}

				writer.WriteString(_T("</sst>"));
                std::wstring sPath = oPath.GetPath();
                NSFile::CFileBinary::SaveToFile(sPath.c_str(), writer.GetData());

                oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::SharedStrings;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}
			const int AddSi(CSi* pSi)
			{
				int nIndex = m_nCount++;
                m_arrItems.push_back(pSi);
				return nIndex;
			}
		private:
			CPath m_oReadPath;

			void ClearItems()
			{
				m_nCount = 0;
                for(size_t i = 0; i < m_arrItems.size(); i++)
				{
                    if ( m_arrItems[i] )delete m_arrItems[i];

                    m_arrItems[i] = NULL;
				}
                m_arrItems.clear();
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("count"),		m_oCount )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("uniqueCount"),	m_oUniqueCount )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oCount;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oUniqueCount;
            std::vector<CSi*>                               m_arrItems;
			int												m_nCount;

		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SHAREDSTRINGS_FILE_INCLUDE_H_
