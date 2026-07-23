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

#include "xlsx_merge_cells.h"
#include "xlsx_utils.h"

#include <vector>
#include <xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

class xlsx_merge_cells::Impl
{
    struct merge
    {
        merge(size_t _c, size_t _r, size_t _cols, size_t _rows): 
            c(_c), r(_r), cols(_cols), rows(_rows)
            {}
        size_t c, r, cols, rows;
    };

public:
    void add_merge(size_t column, size_t row, size_t colsMerged, size_t rowsMerged)
    {
        merges_.push_back(merge(column, row, colsMerged, rowsMerged));
    }

    void xlsx_serialize(std::wostream & _Wostream)
    {
        if (!merges_.empty())
        {
            CP_XML_WRITER(_Wostream)
            {
                CP_XML_NODE(L"mergeCells")
                {
                    CP_XML_ATTR(L"count", merges_.size());

                    for (size_t i = 0 ; i < merges_.size(); i++)
                    {
						merge & m = merges_[i];

                        CP_XML_NODE(L"mergeCell")
                        {
                            CP_XML_ATTR(L"ref", getCellAddress(m.c, m.r) + L":" + getCellAddress(m.c + m.cols, m.r + m.rows));
                        } 
                    }
                }          
            }
        }
    }

private:
    std::vector<merge> merges_;
};

xlsx_merge_cells::xlsx_merge_cells(): impl_(new xlsx_merge_cells::Impl())
{}

xlsx_merge_cells::xlsx_merge_cells(const xlsx_merge_cells & m): impl_(new xlsx_merge_cells::Impl(*m.impl_))
{
}

const xlsx_merge_cells & xlsx_merge_cells::operator=(const xlsx_merge_cells & m)
{
    _CP_SCOPED_PTR(Impl) newImpl(new xlsx_merge_cells::Impl(*m.impl_));
    newImpl.swap(impl_);        
    return *this;
}

xlsx_merge_cells::~xlsx_merge_cells()
{}

void xlsx_merge_cells::add_merge(size_t column, size_t row, size_t colsMerged, size_t rowsMerged)
{
    return impl_->add_merge(column, row, colsMerged, rowsMerged);
}

void xlsx_merge_cells::xlsx_serialize(std::wostream & _Wostream)
{
    return impl_->xlsx_serialize(_Wostream);
}

}
}
