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

#include <Logic/Biff_records/BiffRecord.h>
#include "../../XlsxFormat/WritingElement.h"
#include "../XlsbElementsType.h"
#include "../Biff12_structures/Cell.h"
#include "../Biff12_structures/XLWideString.h"
#include "../Biff12_structures/GrbitFmla.h"
#include <Logic/Biff_structures/CellParsedFormula.h>
#include <Logic/Biff_structures/Xnum.h>
using namespace XLS;

namespace XLSB
{

    class FmlaBase
    {
       public:
            FmlaBase(bool flag) : formula(flag) {}
            Cell               cell;
            GrbitFmla          grbitFlags;
            CellParsedFormula  formula;
    };

    template <class T, CF_RECORD_TYPE id>
    class Fmla_T: public FmlaBase, public BiffRecord
    {
       public:
            const CFRecordType::TypeId getTypeId() const
            {
                return id;
            }
            const std::string & getClassName() const
            {
                static std::string str;
                switch(id)
                {
                    case rt_FmlaString:
                        str = "FmlaString";
                        return str;
                    case rt_FmlaNum:
                        str = "FmlaNum";
                        return str;
                    case rt_FmlaBool:
                        str = "FmlaBool";
                        return str;
                    case rt_FmlaError:
                        str = "FmlaError";
                        return str;
                }

            }
            virtual XLS::ElementType get_type() { return type; }

            Fmla_T() : FmlaBase(false)
            {                
            }

            virtual ~Fmla_T()
            {
            }

            BaseObjectPtr clone()
            {
                return BaseObjectPtr(new Fmla_T<T, id>(*this));
            }

            void readFields(CFRecord& record)
            {
                record >> cell >> value >> grbitFlags >> formula;
            }

            T   value;
    };

    typedef Fmla_T<XLWideString,                rt_FmlaString>		FmlaString;
    typedef Fmla_T<XLS::Boolean<unsigned char>, rt_FmlaBool>		FmlaBool;
    typedef Fmla_T<Xnum,                        rt_FmlaNum>             FmlaNum;
    typedef Fmla_T<unsigned char,               rt_FmlaError>		FmlaError;

} // namespace XLSB

