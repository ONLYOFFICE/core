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
#include "XMLReader.h"
#include "XLSXTableController.h"
#include "XML2TableConverter.h"

#include "../../../../../DesktopEditor/common/File.h"
#include "../../../../Base/unicode_util.h"
#include "../../../../../Common/OfficeFileErrorDescription.h"
#include "../../../../XlsxFormat/Workbook/Workbook.h"

#include <memory>

_UINT32 XMLReader::Read(const std::wstring &sFileName, OOX::Spreadsheet::CXlsx &oXlsx)
{
    XmlUtils::CXmlLiteReader reader = {};
    if(!reader.FromFile(sFileName))
    {
        return AVS_FILEUTILS_ERROR_CONVERT_READ_FILE;
    }

    XML2TableConverter converter = {reader};
    XLSXTableController table = {oXlsx, -1};
    // map storing current column number for writing
    std::map<_UINT32, _UINT32> rowNumbers = {};
    std::map<_UINT32, std::wstring> stringData = {};

    while(converter.ReadNextElement(stringData))
    {
        for(auto i = stringData.begin(); i != stringData.end(); i++)
        {
            _UINT32 rowNumber;
            auto column = rowNumbers.find(i->first);
            if(column != rowNumbers.end())
            {
                rowNumber = column->second;
                column->second ++;

            }
            else
            {
                rowNumber = 2;
                rowNumbers.emplace(i->first, 3);
            }
            table.AddCell(i->second, rowNumber, i->first);
        }
        stringData.clear();
    }

    for(auto i = stringData.begin(); i != stringData.end(); i++)
    {
        table.AddCell(i->second, 1, i->first);
    }
    stringData.clear();

    table.FormBook();

    return 0;

}
