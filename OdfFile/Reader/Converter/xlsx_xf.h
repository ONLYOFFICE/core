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
#pragma once

#include <iosfwd>
#include <string>

#include "xlsx_alignment.h"
#include "xlsx_protection.h"

#include "../../Common/CPSharedPtr.h"

namespace cpdoccore {
namespace oox {

    struct xlsx_xf;
    typedef _CP_PTR(xlsx_xf) xlsx_xf_ptr;

    struct xlsx_xf
    {
        _CP_OPT(bool) applyAlignment;
        _CP_OPT(bool) applyBorder;
        _CP_OPT(bool) applyFill;
        _CP_OPT(bool) applyFont;
        _CP_OPT(bool) applyNumberForm;
        _CP_OPT(bool) applyProtection;
        _CP_OPT(unsigned int) borderId;
        _CP_OPT(unsigned int) fillId;
        _CP_OPT(unsigned int) fontId;
        _CP_OPT(unsigned int) numFmtId;
        _CP_OPT(bool) pivotButton;
        _CP_OPT(bool) quotePrefix;
        _CP_OPT(unsigned int) xfId;

        _CP_OPT(xlsx_alignment) alignment;
        _CP_OPT(xlsx_protection) protection;

        mutable std::size_t index;

        bool operator == (const xlsx_xf & rVal) const;
        bool operator != (const xlsx_xf & rVal) const;
        friend std::size_t hash_value(xlsx_xf const & val);
    };

    void xlsx_serialize(std::wostream & _Wostream, const xlsx_xf & xf);

}
}
