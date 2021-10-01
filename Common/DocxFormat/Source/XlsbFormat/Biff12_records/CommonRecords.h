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

#include <Logic/Biff_records/FileSharing.h>
#include <Logic/Biff_records/ExternSheet.h>
#include <Logic/Biff_records/Window1.h>
#include <Logic/Biff_records/Lbl.h>
#include <Logic/Biff_records/UserBView.h>
#include <Logic/Biff_records/WOpt.h>
#include <Logic/Biff_records/BookExt.h>
#include <Logic/Biff_records/ColInfo.h>
#include <Logic/Biff_records/Row.h>
#include <Logic/Biff_records/Table.h>
#include <Logic/Biff_records/ShrFmla.h>
#include <Logic/Biff_records/Array.h>
#include <Logic/Biff_records/Window2.h>
#include <Logic/Biff_records/Pane.h>
#include <Logic/Biff_records/Selection.h>
#include <Logic/Biff_records/SortData.h>
#include "../../XlsxFormat/WritingElement.h"
#include "../XlsbElementsType.h"


namespace XLSB
{
    // Logical representation of common record in BIFF12
    class FileSharing: public XLS::FileSharing
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(FileSharing)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new FileSharing(*this));
        }

    };

    class ExternSheet: public XLS::ExternSheet
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ExternSheet)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new ExternSheet(*this));
        }

    };

    class BookView: public XLS::Window1
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BookView)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new BookView(*this));
        }
    };

    class Name: public XLS::Lbl
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Name)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new Name(*this));
        }
    };

    class UserBookView: public XLS::UserBView
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(UserBookView)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new UserBookView(*this));
        }
    };

    class WebOpt: public XLS::WOpt
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(WebOpt)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new WebOpt(*this));
        }
    };

    class FileRecover: public XLS::BookExt
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(FileRecover)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new FileRecover(*this));
        }
    };

    class ColInfo: public XLS::ColInfo
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ColInfo)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new ColInfo(*this));
        }
    };

    class RowHdr: public XLS::Row
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(RowHdr)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new RowHdr(*this));
        }
    };

    class Table: public XLS::Table
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Table)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new Table(*this));
        }
    };

    class ShrFmla: public XLS::ShrFmla
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ShrFmla)
        ShrFmla(const CellRef& cell_base_ref_init) : XLS::ShrFmla(cell_base_ref_init) {}

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new ShrFmla(*this));
        }
    };

    class ArrFmla: public XLS::Array
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ArrFmla)
        ArrFmla(const CellRef& cell_base_ref_init) : XLS::Array(cell_base_ref_init) {}

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new ArrFmla(*this));
        }
    };

    class BeginWsView: public XLS::Window2
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginWsView)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new BeginWsView(*this));
        }
    };

    class Pane: public XLS::Pane
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Pane)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new Pane(*this));
        }
    };

    class Sel: public XLS::Selection
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Sel)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new Sel(*this));
        }
    };

    class BeginSortState: public XLS::SortData
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginSortState)

        BaseObjectPtr clone() override
        {
            return BaseObjectPtr(new BeginSortState(*this));
        }
    };

} // namespace XLSB

