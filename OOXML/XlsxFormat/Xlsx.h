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
#pragma once

#include "../DocxFormat/IFileContainer.h"

#include "../../DesktopEditor/common/Directory.h"

#include "../PPTXFormat/Theme.h"

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

			CXlsx();
			CXlsx(const CPath& oFilePath);
			virtual ~CXlsx();

			bool Read(const CPath& oFilePath);
			bool Write(const CPath& oDirPath, OOX::CContentTypes &oContentTypes);
			bool WriteWorkbook(const CPath& oDirPath);
			bool WriteXLS(const CPath& oFilePath);

			bool ReadNative(const CPath& oFilePath);
			bool WriteNative(const CPath& oDirPath, OOX::CContentTypes &oContentTypes);
			
			void PrepareToWrite();
			
			void PrepareWorkbook();

			void CreateWorkbook ();
			void CreateSharedStrings ();
			void CreateStyles ();

			bool hasPivot();
			
			PPTX::Theme  *GetTheme () const;

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
			
			bool bDeleteWorkbook;
			bool bDeleteSharedStrings;
			bool bDeleteStyles;
			bool bDeleteCalcChain;
			bool bDeleteWorksheets;
			bool bDeleteVbaProject;
			bool bDeleteJsaProject;

		private:

			void PrepareWorksheet(CWorksheet* pWorksheet);
            void init();

            boost::unordered_map<std::wstring, size_t>	m_mapXlsxEnumeratedGlobal;
		};

	} //Spreadsheet
} // OOX

