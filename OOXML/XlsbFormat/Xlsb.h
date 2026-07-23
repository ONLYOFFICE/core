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

#include "../XlsxFormat/Xlsx.h"
#include <unordered_map>

namespace XLS
{
	class GlobalWorkbookInfo;
	typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;

	class BaseObject;
}

namespace OOX
{	
    namespace Spreadsheet
    {

        class CXlsb : public CXlsx
        {
        public:

            CXlsb()
            {
                init();
            }
            CXlsb(const CPath& oFilePath) : CXlsx(oFilePath)
            {
                init();
            }
            ~CXlsb();
            
            bool ReadBin(const CPath& oFilePath, XLS::BaseObject* objStream);
			bool WriteBin(const CPath& oFilePath, XLS::BaseObject* objStream);

            bool WriteBin(const CPath& oDirPath, OOX::CContentTypes& oContentTypes);

            XLS::GlobalWorkbookInfo* GetGlobalinfo();
            void PrepareSi();
			void PrepareHlinks();
            void PrepareTableFormula();
            void LinkTables();
            void ReadSheetData();
            void WriteSheetData();
            void PrepareRichStr();
            void SetPropForWriteSheet(const std::wstring &sPath, OOX::CContentTypes& oContentTypes);
            void WriteSheet(CWorksheet* worksheet);

			bool IsWriteToXlsx();
			void WriteToXlsx(bool isXlsx);
            XLS::StreamCacheReaderPtr GetFileReader(const CPath& oFilePath, BYTE* &streamBuf);
            XLS::StreamCacheWriterPtr GetFileWriter(const CPath& oFilePath);
            bool WriteSreamCache(XLS::StreamCacheWriterPtr writer);
            
            std::unordered_map<std::wstring, _UINT32> m_mapSheetNameSheetData;
            
            bool m_bWriteToXlsb = false;
        private:

            void init();
            XLS::GlobalWorkbookInfoPtr xls_global_info;
            boost::shared_ptr<NSBinPptxRW::CBinaryFileReader> m_binaryReader;
			boost::shared_ptr<NSBinPptxRW::CXlsbBinaryWriter> m_binaryWriter;

            unsigned short workbook_code_page;
            std::wstring m_sPath;
            OOX::CContentTypes m_oContentTypes;

			bool m_bWriteToXlsx = false;
        };

    } //Spreadsheet
} // OOX

