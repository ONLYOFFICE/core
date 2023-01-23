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
            XLS::GlobalWorkbookInfo* GetGlobalinfo();
            void PrepareSi();
            void PrepareTableFormula();
            void ReadSheetData(bool isWriteSheetToXlsx = false);
            void SetPropForWriteSheet(const std::wstring &sPath, OOX::CContentTypes& oContentTypes);
            void WriteSheet(CWorksheet* worksheet);

            std::unordered_map<std::wstring, _UINT32> m_mapSheetNameSheetData;
        private:

            void init();
            XLS::GlobalWorkbookInfoPtr xls_global_info;
            boost::shared_ptr<NSBinPptxRW::CBinaryFileReader> m_binaryReader;

            unsigned short workbook_code_page;
            std::wstring m_sPath;
            OOX::CContentTypes m_oContentTypes;

        };

    } //Spreadsheet
} // OOX

