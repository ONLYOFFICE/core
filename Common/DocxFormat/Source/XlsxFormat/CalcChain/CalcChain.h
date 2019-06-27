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
#ifndef OOX_CALCCHAIN_FILE_INCLUDE_H_
#define OOX_CALCCHAIN_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CCalcCell : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CCalcCell)
			CCalcCell()
			{
			}
			virtual ~CCalcCell()
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_CalcCell;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("a"),      m_oArray )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("i"),      m_oSheetId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("l"),      m_oDependencyLevel )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("s"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("t"),      m_oRef )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::COnOff<>>					m_oArray;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oSheetId;
			nullable<SimpleTypes::COnOff<>>					m_oDependencyLevel;
			nullable<std::wstring>							m_oRef;
			nullable<SimpleTypes::COnOff<>>					m_oChildChain;
			nullable<SimpleTypes::COnOff<>>					m_oNewThread;
		};
		//необработанные child:
		//<extLst>
		class CCalcChain : public OOX::File, public OOX::IFileContainer
		{
		public:
			CCalcChain(OOX::Document* pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
  				
				CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
				if (xlsx && xlsx->m_bNeedCalcChain) xlsx->m_pCalcChain = this;
			}
			CCalcChain(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
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
			virtual ~CCalcChain()
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
								m_arrItems.push_back(new CCalcCell(oReader));
						}
					}
				}		
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::CalcChain;
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
			void ClearItems()
			{
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if ( m_arrItems[i] )delete m_arrItems[i];
                }

				m_arrItems.clear();
			}
		private:
			CPath m_oReadPath;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		public:
            std::vector<CCalcCell *>  m_arrItems;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CALCCHAIN_FILE_INCLUDE_H_
