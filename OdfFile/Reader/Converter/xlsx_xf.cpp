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

#include "xlsx_xf.h"
#include <ostream>
#include <boost/functional/hash/hash.hpp>

namespace cpdoccore {
namespace oox {

void xlsx_serialize(std::wostream & _Wostream, const xlsx_xf & xf)
{
    _Wostream << L"<xf ";

    if (xf.applyAlignment)
        _Wostream << L"applyAlignment=\"" << xf.applyAlignment.get() << L"\" ";
    if (xf.applyFill)
        _Wostream << L"applyFill=\"" << xf.applyFill.get() << L"\" ";
    if (xf.applyFont)
        _Wostream << L"applyFont=\"" << xf.applyFont.get() << L"\" ";
    if (xf.applyNumberForm)
        _Wostream << L"applyNumberFormat=\"" << xf.applyNumberForm.get() << L"\" ";
    if (xf.applyProtection)
        _Wostream << L"applyProtection=\"" << xf.applyProtection.get() << L"\" ";

    if (xf.borderId)
        _Wostream << L"borderId=\"" << xf.borderId.get() << L"\" ";
    if (xf.fillId)
        _Wostream << L"fillId=\"" << xf.fillId.get() << L"\" ";
    if (xf.fontId)
        _Wostream << L"fontId=\"" << xf.fontId.get() << L"\" ";
    if (xf.numFmtId)
        _Wostream << L"numFmtId=\"" << xf.numFmtId.get() << L"\" ";

    if (xf.pivotButton)
        _Wostream << L"pivotButton=\"" << xf.pivotButton.get() << L"\" ";
    if (xf.quotePrefix)
        _Wostream << L"quotePrefix=\"" << xf.quotePrefix.get() << L"\" ";
    
    if (xf.xfId)
        _Wostream << L"xfId=\"" << xf.xfId.get() << L"\" ";

    _Wostream << L">";

    if (xf.alignment)
        xlsx_serialize(_Wostream, xf.alignment.get());

    if (xf.protection)
        xlsx_serialize(_Wostream, xf.protection.get());

    _Wostream << L"</xf>";
}

bool xlsx_xf::operator == (const xlsx_xf & rVal) const
{
    const bool res = 
        applyAlignment.get_value_or(false)	== rVal.applyAlignment.get_value_or(false) &&
		applyBorder.get_value_or(false)		== rVal.applyBorder.get_value_or(false) &&
		applyFill.get_value_or(false)		== rVal.applyFill.get_value_or(false) &&
        applyFont.get_value_or(false)		== rVal.applyFont.get_value_or(false) &&
        applyNumberForm.get_value_or(false) == rVal.applyNumberForm.get_value_or(false) &&
        applyProtection.get_value_or(false) == rVal.applyProtection.get_value_or(false) &&
        borderId	== rVal.borderId &&
        fillId		== rVal.fillId &&
        fontId		== rVal.fontId &&
        numFmtId	== rVal.numFmtId &&    
        pivotButton.get_value_or(false) == rVal.pivotButton.get_value_or(false) &&
        quotePrefix.get_value_or(false) == rVal.quotePrefix.get_value_or(false) &&    
        //xfId == rVal.xfId &&
        alignment	== rVal.alignment &&
        protection	== rVal.protection;

    return res;
}

bool xlsx_xf::operator != (const xlsx_xf & rVal) const
{
    return !(this->operator ==(rVal));
}
std::size_t hash_value(xlsx_xf const & val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.applyAlignment.get_value_or(false));
    boost::hash_combine(seed, val.applyBorder.get_value_or(false));
    boost::hash_combine(seed, val.applyFill.get_value_or(false));
    boost::hash_combine(seed, val.applyFont.get_value_or(false));
    boost::hash_combine(seed, val.applyNumberForm.get_value_or(false));
    boost::hash_combine(seed, val.applyProtection.get_value_or(false));
    boost::hash_combine(seed, val.borderId.get_value_or(0));
    boost::hash_combine(seed, val.fillId.get_value_or(0));
    boost::hash_combine(seed, val.fontId.get_value_or(0));
    boost::hash_combine(seed, val.numFmtId.get_value_or(0));
    boost::hash_combine(seed, val.pivotButton.get_value_or(false));
    boost::hash_combine(seed, val.quotePrefix.get_value_or(false));
    boost::hash_combine(seed, val.xfId.get_value_or(0));
    boost::hash_combine(seed, val.alignment.get_value_or(xlsx_alignment()));
    boost::hash_combine(seed, val.protection.get_value_or(xlsx_protection()));

    return seed;    
}

}
}
