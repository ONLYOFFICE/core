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
#ifndef OOX_XLSX_INCLUDE_H_
#define OOX_XLSX_INCLUDE_H_

#include "../Base/SmartPtr.h"
#include "../DocxFormat/IFileContainer.h"

#include "../../../../DesktopEditor/common/Directory.h"

#include "../../../../ASCOfficePPTXFile/PPTXFormat/Theme.h"

namespace OOX
{
	class CApp;
	class CCore;

	namespace Spreadsheet
	{
		class CWorksheet;
		class CWorkbook;
		class CSharedStrings;
		class CStyles;
		class CCalcChain;
		class WorkbookComments;

		class CXlsx : public OOX::Document, public OOX::IFileContainer 
		{
		public:

			CXlsx() : OOX::IFileContainer(dynamic_cast<OOX::Document*>(this))
			{
				init();
			}
			CXlsx(const CPath& oFilePath) : OOX::IFileContainer(dynamic_cast<OOX::Document*>(this))
			{
				init();

				Read( oFilePath );
			}
			virtual ~CXlsx();

			bool Read(const CPath& oFilePath);
			bool Write(const CPath& oDirPath, OOX::CContentTypes &oContentTypes);
			bool WriteWorkbook(const CPath& oDirPath);
			
			void PrepareToWrite();
			
			void PrepareWorkbook();

			void CreateWorkbook ();
			void CreateSharedStrings ();
			void CreateStyles ();
			
			PPTX::Theme  *GetTheme () const
			{
				return (PPTX::Theme  *)(m_pTheme.GetPointer());
			}
			smart_ptr<PPTX::Theme>							m_pTheme;

			CApp*											m_pApp;
			CCore*											m_pCore;

			CWorkbook*                                      m_pWorkbook;
            CSharedStrings*                                 m_pSharedStrings;
            CStyles*                                        m_pStyles;
            CCalcChain*                                     m_pCalcChain;
			OOX::VbaProject*								m_pVbaProject;
			OOX::JsaProject*								m_pJsaProject;
            
			WorkbookComments*								m_pWorkbookComments;
			NSBinPptxRW::CXlsbBinaryWriter*					m_pXlsbWriter;
			int												m_nLastReadRow;
			int												m_nLastReadCol;
			bool											m_bNeedCalcChain;// disable because it is useless but reading takes considerable time

			std::vector<CWorksheet*>								m_arWorksheets;	//order as is
			std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>	m_mapWorksheets; //copy, for fast find - order by rId(name) 
		private:
			void PrepareWorksheet(CWorksheet* pWorksheet);
			void init()
			{
				m_bSpreadsheets		= true;

				m_pApp				= NULL;
				m_pCore				= NULL;

				m_pWorkbook			= NULL;
				m_pSharedStrings	= NULL;
				m_pStyles			= NULL;
				m_pCalcChain		= NULL;
				m_pVbaProject		= NULL;
				m_pJsaProject		= NULL;
				m_pWorkbookComments = NULL;
				m_pXlsbWriter = NULL;
				m_nLastReadRow = 0;
				m_nLastReadCol = -1;
				m_bNeedCalcChain = true;

				bDeleteWorkbook			= false;
				bDeleteSharedStrings	= false;
				bDeleteStyles			= false;
				bDeleteCalcChain		= false;
				bDeleteWorksheets		= false;
				bDeleteVbaProject		= false;
				bDeleteJsaProject		= false;
			}

            boost::unordered_map<std::wstring, size_t>	m_mapXlsxEnumeratedGlobal;
			
			bool bDeleteWorkbook;
			bool bDeleteSharedStrings;
			bool bDeleteStyles;
			bool bDeleteCalcChain;
			bool bDeleteWorksheets;
			bool bDeleteVbaProject;
			bool bDeleteJsaProject;
		};

	} //Spreadsheet
} // OOX

#endif // OOX_XLSX_INCLUDE_H_
