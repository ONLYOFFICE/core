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

#include "../Xlsx.h"
#include "../CommonInclude.h"

#include "BookViews.h"
#include "CalcPr.h"
#include "DefinedNames.h"
#include "Sheets.h"
#include "WorkbookPr.h"
#include "ExternalReferences.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CPersonList;
		//необработанные child:
		//<customWorkbookViews>
		//<extLst>
		//<fileRecoveryPr>
		//<fileSharing>
		//<fileVersion>
		//<functionGroups>
		//<oleSize>
		//<pivotCaches>
		//<smartTagPr>
		//<smartTagTypes>
		//<webPublishing>
		//<webPublishObjects>
		//<workbookPr>
		//<workbookProtection>
		class CWorkbook : public OOX::File, public OOX::IFileContainer
		{
		public:
			CWorkbook(OOX::Document* pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
			{
				m_bMacroEnabled	= false;
				m_bSpreadsheets = true;
				m_pPersonList = NULL;

				CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
				if ((xlsx) && (!xlsx->m_pWorkbook))
				{
					xlsx->m_pWorkbook = this;
				}
			}
			CWorkbook(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
			{
				m_bMacroEnabled	= false;
				m_bSpreadsheets = true;
				m_pPersonList = NULL;
				
  				CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
				if ((xlsx) && (!xlsx->m_pWorkbook))
				{
					xlsx->m_pWorkbook = this;
				}
				read(oRootPath, oPath);
			}
			virtual ~CWorkbook()
			{
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

  				CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
				if ( (xlsx ) && (xlsx->m_pVbaProject) )
				{
					m_bMacroEnabled = true;
				}

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;

				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if ( L"workbook" == sName )
				{
					if ( !oReader.IsEmptyNode() )
					{
						int nDocumentDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nDocumentDepth ) )
						{
							sName = XmlUtils::GetNameNoNS(oReader.GetName());

							if ( L"bookViews" == sName )
								m_oBookViews = oReader;
							else if ( L"calcPr" == sName )
								m_oCalcPr = oReader;
							else if ( L"definedNames" == sName )
								m_oDefinedNames = oReader;
							else if ( L"sheets" == sName )
								m_oSheets = oReader;
							else if ( L"workbookPr" == sName )
								m_oWorkbookPr = oReader;
							else if ( L"externalReferences" == sName )
								m_oExternalReferences = oReader;
							else if ( L"fileVersion" == sName )
							{
								WritingElement_ReadAttributes_Start( oReader )
									WritingElement_ReadAttributes_Read_if( oReader, L"appName", m_oAppName )
								WritingElement_ReadAttributes_End( oReader )
							}
						}
					}
				}		
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><workbook xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">");

				if(m_oWorkbookPr.IsInit())
					m_oWorkbookPr->toXML(sXml);
				if(m_oBookViews.IsInit())
					m_oBookViews->toXML(sXml);
				if(m_oSheets.IsInit())
					m_oSheets->toXML(sXml);
				if (m_oExternalReferences.IsInit())
					m_oExternalReferences->toXML(sXml);
				if(m_oDefinedNames.IsInit())
					m_oDefinedNames->toXML(sXml);
				if(m_oCalcPr.IsInit())
					m_oCalcPr->toXML(sXml);

				if(m_oPivotCachesXml.IsInit())
					sXml.WriteString(m_oPivotCachesXml.get());
				sXml.WriteString(L"</workbook>");

                std::wstring sPath = oPath.GetPath();
                NSFile::CFileBinary::SaveToFile(sPath.c_str(), sXml.GetData());

				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write( oPath, oDirectory, oContent );
			}
			virtual const OOX::FileType type() const
			{
				if (m_bMacroEnabled)	return OOX::Spreadsheet::FileTypes::WorkbookMacro;
				else					return OOX::Spreadsheet::FileTypes::Workbook;
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
			void PrepareToWrite()
			{
				//WorkbookPr
				if(false == m_oWorkbookPr.IsInit())
					m_oWorkbookPr.Init();
				//todo если этот параметр выставлен, то берется стандартная тема, а не из файла.
				//if(false == m_oWorkbookPr->m_oDefaultThemeVersion.IsInit())
				//{
				//	m_oWorkbookPr->m_oDefaultThemeVersion.Init();
				//	m_oWorkbookPr->m_oDefaultThemeVersion->SetValue(124226);
				//}
				//BookViews
				if(false == m_oBookViews.IsInit())
					m_oBookViews.Init();
				
				if(m_oBookViews->m_arrItems.empty())
					m_oBookViews->m_arrItems.push_back(new OOX::Spreadsheet::CWorkbookView());
				
				OOX::Spreadsheet::CWorkbookView* pWorkbookView = m_oBookViews->m_arrItems.front();

				if(false == pWorkbookView->m_oXWindow.IsInit())
				{
					pWorkbookView->m_oXWindow.Init();
					pWorkbookView->m_oXWindow->SetValue(360);
				}
				if(false == pWorkbookView->m_oYWindow.IsInit())
				{
					pWorkbookView->m_oYWindow.Init();
					pWorkbookView->m_oYWindow->SetValue(15);
				}
				if(false == pWorkbookView->m_oWindowWidth.IsInit())
				{
					pWorkbookView->m_oWindowWidth.Init();
					pWorkbookView->m_oWindowWidth->SetValue(20955);
				}
				if(false == pWorkbookView->m_oWindowHeight.IsInit())
				{
					pWorkbookView->m_oWindowHeight.Init();
					pWorkbookView->m_oWindowHeight->SetValue(9720);
				}
			}
			LONG GetActiveSheetIndex()
			{
				LONG lActiveSheet = 0;
				std::wstring sSheetRId = L"Sheet1"; // Читаем не по rId, а по имени листа
				// Get active sheet
				if ( m_oBookViews.IsInit() && !m_oBookViews->m_arrItems.empty())
				{
					if (m_oBookViews->m_arrItems.front()->m_oActiveTab.IsInit())
					{
						lActiveSheet =  m_oBookViews->m_arrItems.front()->m_oActiveTab->GetValue();
						if (0 > lActiveSheet)
							lActiveSheet = 0;
					}
				}
				return lActiveSheet;
			}
			CPath											m_oReadPath;

			nullable<OOX::Spreadsheet::CBookViews>			m_oBookViews;
			nullable<OOX::Spreadsheet::CDefinedNames>		m_oDefinedNames;
			nullable<OOX::Spreadsheet::CSheets>				m_oSheets;
			nullable<OOX::Spreadsheet::CWorkbookPr>			m_oWorkbookPr;
			nullable<OOX::Spreadsheet::CExternalReferences>	m_oExternalReferences;
			nullable<std::wstring>							m_oPivotCachesXml;
            nullable<OOX::Spreadsheet::CCalcPr>				m_oCalcPr;
			nullable_string									m_oAppName;
			
			CPersonList*									m_pPersonList;
			bool											m_bMacroEnabled;
		};
	} //Spreadsheet
} // namespace OOX

