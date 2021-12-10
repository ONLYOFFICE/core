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

#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/FileSharing.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/ExternSheet.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Window1.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Lbl.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/UserBView.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/WOpt.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/BookExt.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/ColInfo.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Row.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Table.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/ShrFmla.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Array.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Window2.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Pane.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Selection.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/SortData.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Font.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/XF.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/DXF.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/TableStyles.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/TableStyle.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/TableStyleElement.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Setup.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Dv.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Qsi.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Qsir.h"
#include "../../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_records/Qsif.h"
#include "../Biff12_structures/FRTHeader.h"

#include "../../XlsxFormat/WritingElement.h"


namespace XLSB
{
    // Logical representation of common record in BIFF12
    class FileSharing: public XLS::FileSharing
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(FileSharing)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new FileSharing(*this));
        }

    };

    class ExternSheet: public XLS::ExternSheet
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ExternSheet)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new ExternSheet(*this));
        }

    };

    class BookView: public XLS::Window1
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BookView)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BookView(*this));
        }
    };

    class Name: public XLS::Lbl
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Name)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new Name(*this));
        }
    };

    class UserBookView: public XLS::UserBView
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(UserBookView)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new UserBookView(*this));
        }
    };

    class WebOpt: public XLS::WOpt
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(WebOpt)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new WebOpt(*this));
        }
    };

    class FileRecover: public XLS::BookExt
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(FileRecover)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new FileRecover(*this));
        }
    };

    class ColInfo: public XLS::ColInfo
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ColInfo)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new ColInfo(*this));
        }
    };

    class RowHdr: public XLS::Row
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(RowHdr)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new RowHdr(*this));
        }
    };

    class Table: public XLS::Table
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Table)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new Table(*this));
        }
    };

    class ShrFmla: public XLS::ShrFmla
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ShrFmla)
        ShrFmla(const XLS::CellRef& cell_base_ref_init) : XLS::ShrFmla(cell_base_ref_init) {}

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new ShrFmla(*this));
        }
    };

    class ArrFmla: public XLS::Array
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ArrFmla)
        ArrFmla(const XLS::CellRef& cell_base_ref_init) : XLS::Array(cell_base_ref_init) {}

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new ArrFmla(*this));
        }
    };

    class BeginWsView: public XLS::Window2
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginWsView)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginWsView(*this));
        }
    };

    class Pane: public XLS::Pane
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Pane)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new Pane(*this));
        }
    };

    class Sel: public XLS::Selection
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Sel)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new Sel(*this));
        }
    };

    class BeginSortState: public XLS::SortData
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginSortState)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginSortState(*this));
        }
    };

    class Font: public XLS::Font
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Font)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new Font(*this));
        }
    };

    class XF: public XLS::XF
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(XF)

        XF(size_t& cell_xf_current_id, size_t& style_xf_current_id) :
            XLS::XF(cell_xf_current_id, style_xf_current_id) {}

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new XF(*this));
        }
    };

    class DXF: public XLS::DXF
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(DXF)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new DXF(*this));
        }
    };

    class BeginTableStyles: public XLS::TableStyles
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginTableStyles)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginTableStyles(*this));
        }
    };

    class BeginTableStyle: public XLS::TableStyle
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginTableStyle)

		XLS::BaseObjectPtr clone() override
		{
			return XLS::BaseObjectPtr(new BeginTableStyle(*this));
		}
    };

    class TableStyleElement: public XLS::TableStyleElement
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(TableStyleElement)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new TableStyleElement(*this));
        }
    };

    class DXF14: public XLS::DXF
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(DXF14)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new DXF14(*this));
        }

        static const XLS::ElementType	type = XLS::typeDXF14;

        void readFields(XLS::CFRecord& record) override
        {
            record.skipNunBytes(4);
            XLS::DXF::readFields(record);
        }
    };

    class DXF15: public DXF14
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(DXF15)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new DXF15(*this));
        }
    };

    class PageSetup : public XLS::Setup
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(PageSetup)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new PageSetup(*this));
        }
    };

    class DVal : public XLS::Dv
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(DVal)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new DVal(*this));
        }

        //static const XLS::ElementType	type = XLS::typeDVal;
    };

    class DVal14 : public XLS::Dv
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(DVal14)

        DVal14(bool ext14 = true) : XLS::Dv(ext14) {}

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new DVal14(*this));
        }

        static const XLS::ElementType	type = XLS::typeDVal14;
    };

    class BeginQSI: public XLS::Qsi
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginQSI)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginQSI(*this));
        }
    };

    class BeginQSIR: public XLS::Qsir
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginQSIR)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginQSIR(*this));
        }
    };

    class BeginQSIF: public XLS::Qsif
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginQSIF)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginQSIF(*this));
        }
    };

} // namespace XLSB
