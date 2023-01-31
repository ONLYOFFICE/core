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

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/FileSharing.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/ExternSheet.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Window1.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Lbl.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/UserBView.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/WOpt.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/BookExt.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/ColInfo.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Row.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Table.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/ShrFmla.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Array.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Window2.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Pane.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Selection.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/SortData.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Font.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/XF.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/DXF.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/TableStyles.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/TableStyle.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/TableStyleElement.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Setup.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Dv.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Qsi.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Qsir.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Qsif.h"

#include "../Biff12_structures/FRTHeader.h"

#include "../../XlsxFormat/WritingElement.h"


namespace XLSB
{
    // Logical representation of common record in BIFF12
    class FileSharing: public XLS::FileSharing
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(FileSharing)
        BASE_OBJECT_DEFINE_CLASS_NAME(FileSharing)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new FileSharing(*this));
        }

    };

    class ExternSheet: public XLS::ExternSheet
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ExternSheet)
        BASE_OBJECT_DEFINE_CLASS_NAME(ExternSheet)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new ExternSheet(*this));
        }

    };

    class BookView: public XLS::Window1
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BookView)
        BASE_OBJECT_DEFINE_CLASS_NAME(BookView)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BookView(*this));
        }
    };

    class Name: public XLS::Lbl
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Name)
        BASE_OBJECT_DEFINE_CLASS_NAME(Name)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new Name(*this));
        }
    };

    class UserBookView: public XLS::UserBView
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(UserBookView)
        BASE_OBJECT_DEFINE_CLASS_NAME(UserBookView)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new UserBookView(*this));
        }
    };

    class WebOpt: public XLS::WOpt
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(WebOpt)
        BASE_OBJECT_DEFINE_CLASS_NAME(WebOpt)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new WebOpt(*this));
        }
    };

    class FileRecover: public XLS::BookExt
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(FileRecover)
        BASE_OBJECT_DEFINE_CLASS_NAME(FileRecover)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new FileRecover(*this));
        }
    };

    class ColInfo: public XLS::ColInfo
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ColInfo)
        BASE_OBJECT_DEFINE_CLASS_NAME(ColInfo)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new ColInfo(*this));
        }
    };

    class RowHdr: public XLS::Row
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(RowHdr)
        BASE_OBJECT_DEFINE_CLASS_NAME(RowHdr)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new RowHdr(*this));
        }
    };

    class Table: public XLS::Table
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Table)
        BASE_OBJECT_DEFINE_CLASS_NAME(Table)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new Table(*this));
        }
    };

    class ShrFmla: public XLS::ShrFmla
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ShrFmla)
        BASE_OBJECT_DEFINE_CLASS_NAME(ShrFmla)
        ShrFmla(const XLS::CellRef& cell_base_ref_init) : XLS::ShrFmla(cell_base_ref_init) {}

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new ShrFmla(*this));
        }
    };

    class ArrFmla: public XLS::Array
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(ArrFmla)
        BASE_OBJECT_DEFINE_CLASS_NAME(ArrFmla)
        ArrFmla(const XLS::CellRef& cell_base_ref_init) : XLS::Array(cell_base_ref_init) {}

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new ArrFmla(*this));
        }
    };

    class BeginWsView: public XLS::Window2
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginWsView)
        BASE_OBJECT_DEFINE_CLASS_NAME(BeginWsView)

        static const XLS::ElementType	type = XLS::typeBeginWsView;

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginWsView(*this));
        }
    };

    class Pane: public XLS::Pane
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Pane)
        BASE_OBJECT_DEFINE_CLASS_NAME(Pane)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new Pane(*this));
        }
    };

    class Sel: public XLS::Selection
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Sel)
        BASE_OBJECT_DEFINE_CLASS_NAME(Sel)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new Sel(*this));
        }
    };

    class BeginSortState: public XLS::SortData
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginSortState)
        BASE_OBJECT_DEFINE_CLASS_NAME(BeginSortState)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginSortState(*this));
        }
    };

    class Font: public XLS::Font
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(Font)
        BASE_OBJECT_DEFINE_CLASS_NAME(Font)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new Font(*this));
        }
    };

    class XF: public XLS::XF
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(XF)
        BASE_OBJECT_DEFINE_CLASS_NAME(XF)

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
        BASE_OBJECT_DEFINE_CLASS_NAME(DXF)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new DXF(*this));
        }
    };

    class BeginTableStyles: public XLS::TableStyles
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginTableStyles)
        BASE_OBJECT_DEFINE_CLASS_NAME(BeginTableStyles)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginTableStyles(*this));
        }
    };

    class BeginTableStyle: public XLS::TableStyle
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginTableStyle)
        BASE_OBJECT_DEFINE_CLASS_NAME(BeginTableStyle)

		XLS::BaseObjectPtr clone() override
		{
			return XLS::BaseObjectPtr(new BeginTableStyle(*this));
		}
    };

    class TableStyleElement: public XLS::TableStyleElement
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(TableStyleElement)
        BASE_OBJECT_DEFINE_CLASS_NAME(TableStyleElement)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new TableStyleElement(*this));
        }
    };

    class DXF14: public XLS::DXF
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(DXF14)
        BASE_OBJECT_DEFINE_CLASS_NAME(DXF14)

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
        BASE_OBJECT_DEFINE_CLASS_NAME(DXF15)

		XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new DXF15(*this));
        }
    };

    class PageSetup : public XLS::Setup
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(PageSetup)
        BASE_OBJECT_DEFINE_CLASS_NAME(PageSetup)

        static const XLS::ElementType	type = XLS::typePageSetup;

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new PageSetup(*this));
        }
    };

    class DVal : public XLS::Dv
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(DVal)
        BASE_OBJECT_DEFINE_CLASS_NAME(DVal)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new DVal(*this));
        }

        static const XLS::ElementType	type = XLS::typeDVal;
    };

    class DVal14 : public XLS::Dv
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(DVal14)
        BASE_OBJECT_DEFINE_CLASS_NAME(DVal14)

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
        BASE_OBJECT_DEFINE_CLASS_NAME(BeginQSI)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginQSI(*this));
        }
    };

    class BeginQSIR: public XLS::Qsir
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginQSIR)
        BASE_OBJECT_DEFINE_CLASS_NAME(BeginQSIR)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginQSIR(*this));
        }
    };

    class BeginQSIF: public XLS::Qsif
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginQSIF)
        BASE_OBJECT_DEFINE_CLASS_NAME(BeginQSIF)

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginQSIF(*this));
        }
    };

    class CsPageSetup : public XLS::Setup
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(CsPageSetup)
        BASE_OBJECT_DEFINE_CLASS_NAME(CsPageSetup)

        static const XLS::ElementType	type = XLS::typeCsPageSetup;

        CsPageSetup(bool isChart = true) : XLS::Setup(isChart) {}

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new CsPageSetup(*this));
        }
    };

    class BeginCsView: public XLS::Window2
    {
        BIFF_RECORD_DEFINE_TYPE_INFO(BeginCsView)
        BASE_OBJECT_DEFINE_CLASS_NAME(BeginCsView)

        static const XLS::ElementType	type = XLS::typeBeginCsView;

        BeginCsView(bool isChart = true) : XLS::Window2(isChart) {}

        XLS::BaseObjectPtr clone() override
        {
            return XLS::BaseObjectPtr(new BeginCsView(*this));
        }
    };


} // namespace XLSB
