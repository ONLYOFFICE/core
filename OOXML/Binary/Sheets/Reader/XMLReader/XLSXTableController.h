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
#pragma once

#include "../CellFormatController/CellFormatController.h"

#include "../../../../Base/Base.h"
#include "../../../../XlsxFormat/Worksheets/Worksheet.h"
#include "../../../../XlsxFormat/Xlsx.h"

#include <string>
#include <vector>

/// @brief класс -обертка позволяющий добавлять ячейки в таблицу и создавать документ из созданных ячеек
class XLSXTableController
{

public:
    /// @brief инициализация полей объекта
    /// @param book объект который будет заполнен данными с помощью метода FormBook
    /// @param lcid идентификатор локлаи
    XLSXTableController(OOX::Spreadsheet::CXlsx &book, _INT32 lcid);

    /// @brief добавление ячейки
    /// @param sText вставляемый текст
    /// @param nRow номер строки
    /// @param nCol номер столбца
    /// @param bIsWrap признак свернутости
    void AddCell(const std::wstring &sText, INT nRow, INT nCol);

    /// @brief получение документа xlsx
    void FormBook();

private:
    /// @brief добавление ряда
    /// @param pRow указатель на строку
    /// @param pWorkSheet указатель на лист
    /// @param nRow номер строки
    _UINT32 addRow(OOX::Spreadsheet::CRow *pRow, OOX::Spreadsheet::CWorksheet *pWorkSheet,  INT nRow);

    /// @brief добавление страницы
    /// @param page указатель на лист
    /// @param pageNumber номер страницы
    void addPage(OOX::Spreadsheet::CWorksheet *page, INT pageNumber);

    /// @brief документ xlsx
    OOX::Spreadsheet::CXlsx *book_;

    /// @brief вектор со строками таблицы
    std::vector<OOX::Spreadsheet::CRow*> tableRows_;

    /// @brief контроллер форматов
    std::shared_ptr<CellFormatController> formates_;
};
