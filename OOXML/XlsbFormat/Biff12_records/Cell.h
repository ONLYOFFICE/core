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

#include "../../XlsxFormat/WritingElement.h"

#include "../Biff12_structures/Cell.h"
#include  "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/Xnum.h"
#include "../Biff12_structures/XLWideString.h"

namespace XLSB
{

class CellBase
{
   public:
        Cell cell;
        virtual ~CellBase(){}
};

template <class T, CF_RECORD_TYPE id>
class Cell_T: public CellBase, public XLS::BiffRecord
{
   public:
        const XLS::CFRecordType::TypeId getTypeId() const
        {
            return id;
        }
        const std::string & getClassName() const
        {
            static std::string str;
            switch(id)
            {
				case rt_CellBlank:	str = "CellBlank"; break;
				case rt_CellRk:		str = "CellRk"; break;
				case rt_CellError:	str = "CellError"; break; 
				case rt_CellBool:	str = "CellBool"; break;
				case rt_CellReal:	str = "CellReal"; break;
				case rt_CellIsst:	str = "CellIsst"; break; 
				case rt_CellSt:		str = "CellSt"; break;
            }
			return str;
        }
        ~Cell_T()
        {
        }

		XLS::BaseObjectPtr clone()
        {
            return XLS::BaseObjectPtr(new Cell_T<T, id>(*this));
        }

        void readFields(XLS::CFRecord& record)
        {
            record >> cell;
            if(id != rt_CellBlank)
                record >> value;
        }

        T value;
};

typedef Cell_T<int,                                 rt_CellBlank>               CellBlank;
typedef Cell_T<XLS::RkNumber,                       rt_CellRk>                  CellRk;
typedef Cell_T<unsigned char,                       rt_CellError>               CellError;
typedef Cell_T<XLS::Boolean<unsigned char>,         rt_CellBool>                CellBool;
typedef Cell_T<XLS::Xnum,                           rt_CellReal>                CellReal;
typedef Cell_T<_UINT32,                             rt_CellIsst>                CellIsst;
typedef Cell_T<XLWideString,                        rt_CellSt>                  CellSt;

} // namespace XLSB

