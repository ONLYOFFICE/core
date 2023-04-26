/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "XMLReader.h"
#include "XLSXTableController.h"
#include "XMLConverter2.h"
#include "XMLMap.h"

#include "../../../../DesktopEditor/common/File.h"
#include "../../../Base/unicode_util.h"
#include "../../../../Common/OfficeFileErrorDescription.h"
#include "../../../XlsxFormat/Workbook/Workbook.h"


#include "../../../../DesktopEditor/xml/include/xmlutils.h"

#include <memory>

_UINT32 XMLReader::Read2(const std::wstring &sFileName, OOX::Spreadsheet::CXlsx &oXlsx)
{
    XmlUtils::CXmlLiteReader reader = {};
    if(!reader.FromFile(sFileName))
    {
        return AVS_FILEUTILS_ERROR_CONVERT_READ_FILE;
    }

    XMLMap map{};

    auto rootNode = std::make_shared<XmlNode>();
    ColumnNameController columnNames{};
    map.ReadXmlStructure(reader, columnNames, rootNode);

    XLSXTableController table = {oXlsx};
    // map хранящий текущий номер колонки для записи
    std::map<_UINT32, _UINT32> rowNumbers = {};
    std::map<_UINT32, std::wstring> stringData = {};

    //table.FormBook();

    return 0;

}
