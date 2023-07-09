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

#include "../../../../Base/Base.h"
#include "../../../../XlsxFormat/Worksheets/Worksheet.h"
#include "../../../../XlsxFormat/Styles/Styles.h"


/// @brief класс определяющий и корректирующий тип данных, помещаемых в ячейки таблицы
class CellFormatController
{

public:
    /// @brief конструктор
    /// @param styles стили из таблицы
    CellFormatController(OOX::Spreadsheet::CStyles *styles);

    /// @brief обрабатывает вставляемые в ячейку таблицы данные, переводя их в нужный тип, и заполняет ими ячейку
    /// @param pCell указатель на ячейку
    /// @param value вставляемые в ячейку данные в строковом типе
    void ProcessCellType(OOX::Spreadsheet::CCell *pCell, const std::wstring &value, bool bIsWrap = false);

private:

    /// @brief создание стиля для указанного формата
    /// @param format формат значения
    void createFormatStyle(const std::wstring &format);

    /// @brief указатель на ячейку с которой ведется работа
    OOX::Spreadsheet::CCell *pCell_;

    /// @brief map с форматами данных
    std::map<std::wstring, unsigned int> mapDataNumber_;

    /// @brief указатель на стили документа
	OOX::Spreadsheet::CStyles *m_pStyles;

    /// @brief указатель на полученное строковое значение
    const std::wstring *value_;

};
