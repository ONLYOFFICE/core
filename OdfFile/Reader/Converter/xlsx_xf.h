/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
