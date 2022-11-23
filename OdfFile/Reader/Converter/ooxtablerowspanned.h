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

#include "../../Common/CPSharedPtr.h"
#include <string>

namespace cpdoccore { 
namespace oox {

// контекст для обработки объединения столбцов

struct table_row_spanned
{
public:
    table_row_spanned() : num_(0), column_spanned_(0), style_(L""){}
    
    void num(unsigned int Val)  {num_ = Val; }
    unsigned int num() const { return num_; }
    void decrease() { --num_; }

    void set_style(const std::wstring & Val) {style_ = Val; }
    const std::wstring & style() const { return style_; }

    void column_spanned(unsigned int Val) {column_spanned_ = Val;}
    unsigned int column_spanned() const { return column_spanned_; }

private:
    unsigned int num_;
    unsigned int column_spanned_;
    std::wstring style_;
};

typedef shared_ptr<table_row_spanned>::Type table_row_spanned_ptr;

}
}
