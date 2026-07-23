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
#include "XMLConverter2.h"
#include "XMLMap.h"

#include "../../../../../DesktopEditor/common/File.h"
#include "../../../../Base/unicode_util.h"
#include "../../../../../Common/OfficeFileErrorDescription.h"
#include "../../../../XlsxFormat/Workbook/Workbook.h"

#include "../../../../../DesktopEditor/xml/include/xmlutils.h"

#include <memory>

_UINT32 XMLReader::Read2(const std::wstring &sFileName, OOX::Spreadsheet::CXlsx &oXlsx, _INT32 lcid)
{
    XmlUtils::CXmlLiteReader reader = {};
    if(!reader.FromFile(sFileName))
    {
        return AVS_FILEUTILS_ERROR_CONVERT_READ_FILE;
    }

    /// read xml file structure
    XMLMap map{};
    auto rootNode = std::make_shared<XmlNode>();
    ColumnNameController nameController{};
    std::set<std::wstring> repeatableColumns{};
    map.ReadXmlStructure(reader, nameController, rootNode, repeatableColumns);
    reader.MoveToStart();

    /// create table
    XLSXTableController table = {oXlsx, lcid};
    /// fill first row of table with column names
    auto colNames = nameController.GetColumnNames();
    std::map<_UINT32, std::wstring> namesMap;
    for(auto i = colNames.begin(); i != colNames.end(); i++)
    {
        namesMap.emplace(i->second, i->first);
    }

    for(auto i = namesMap.begin(); i != namesMap.end(); i++)
    {
        table.AddCell(i->second, 1, i->first);
    }

    XMLConverter converter = {reader, rootNode, nameController, repeatableColumns};
    converter.ConvertXml(table);

    table.FormBook();

    return 0;

}
