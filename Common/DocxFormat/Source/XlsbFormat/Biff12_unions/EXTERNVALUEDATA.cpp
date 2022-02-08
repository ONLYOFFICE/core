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

#include "EXTERNVALUEDATA.h"
#include "../Biff12_records/ExternCell.h"
#include "../Biff12_structures/CellRef.h"

using namespace XLS;

namespace XLSB
{

    EXTERNVALUEDATA::EXTERNVALUEDATA(_INT32 row) : m_Row(row)
    {
    }

    EXTERNVALUEDATA::~EXTERNVALUEDATA()
    {
    }

    BaseObjectPtr EXTERNVALUEDATA::clone()
    {
        return BaseObjectPtr(new EXTERNVALUEDATA(*this));
    }

    // EXTERNVALUEDATA = BrtExternCellBlank / BrtExternCellReal / BrtExternCellBool / BrtExternCellError / BrtExternCellString
    const bool EXTERNVALUEDATA::loadContent(BinProcessor& proc)
    {
        ExternCellBlank   externCellBlank;
        ExternCellReal    externCellReal;
        ExternCellBool    externCellBool;
        ExternCellError	  externCellError;
        ExternCellString  externCellString;

        if(proc.optional(externCellBlank))
        {
            m_Col = externCellBlank.col;
        }
        else if(proc.optional(externCellReal))
        {
            m_Col = externCellReal.col;
        }
        else if(proc.optional(externCellBool))
        {
            m_Col = externCellBool.col;
        }
        else if(proc.optional(externCellError))
        {
            m_Col = externCellError.col;
        }
        else if(proc.optional(externCellString))
        {
            m_Col = externCellString.col;
        }
        else
        {
            return false;
        }

        m_Ref = XLSB::RgceLoc(m_Row - 1, m_Col, true, true).toString();

        m_source = elements_.back();
        elements_.pop_back();
        return true;
    }

} // namespace XLSB
