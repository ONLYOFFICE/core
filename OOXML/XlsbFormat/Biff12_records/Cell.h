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

#include "../../XlsxFormat/WritingElement.h"

#include "../Biff12_structures/Cell.h"
#include  "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/Xnum.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/XLWideString.h"

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

        void readFields(XLS::CFRecord& record) override
        {
            record >> cell;
            if(id != rt_CellBlank)
                record >> value;
        }

		void writeFields(XLS::CFRecord& record) override
		{
			record << cell;
			if (id != rt_CellBlank)
				record << value;
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

