/*
 * (c) Copyright Ascensio System SIA 2010-2021
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

#include  "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/BiffRecord.h"
#include "../../XlsxFormat/WritingElement.h"



namespace XLSB
{
    // Logical representation of BrtSheetProtection record in BIFF12
    class SheetProtection: public XLS::BiffRecord
    {
            BIFF_RECORD_DEFINE_TYPE_INFO(SheetProtection)
            BASE_OBJECT_DEFINE_CLASS_NAME(SheetProtection)
        public:
            SheetProtection();
            ~SheetProtection();

            XLS::BaseObjectPtr clone();

            void readFields(XLS::CFRecord& record);

            static const XLS::ElementType	type = XLS::typeSheetProtection;

            _UINT16                     protpwd;
            XLS::Boolean<unsigned int>  fLocked;
            XLS::Boolean<unsigned int>  fObjects;
            XLS::Boolean<unsigned int>  fScenarios;
            XLS::Boolean<unsigned int>  fFormatCells;
            XLS::Boolean<unsigned int>  fFormatColumns;
            XLS::Boolean<unsigned int>  fFormatRows;
            XLS::Boolean<unsigned int>  fInsertColumns;
            XLS::Boolean<unsigned int>  fInsertRows;
            XLS::Boolean<unsigned int>  fInsertHyperlinks;
            XLS::Boolean<unsigned int>  fDeleteColumns;
            XLS::Boolean<unsigned int>  fDeleteRows;
            XLS::Boolean<unsigned int>  fSelLockedCells;
            XLS::Boolean<unsigned int>  fSort;
            XLS::Boolean<unsigned int>  fAutoFilter;
            XLS::Boolean<unsigned int>  fPivotTables;
            XLS::Boolean<unsigned int>  fSelUnlockedCells;

    };

} // namespace XLSB

