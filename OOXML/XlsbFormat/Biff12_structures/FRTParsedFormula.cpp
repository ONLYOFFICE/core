﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "FRTParsedFormula.h"

using namespace XLS;

namespace XLSB
{

FRTParsedFormula::FRTParsedFormula() :	ParsedFormula(CellRef())
{
}

FRTParsedFormula& FRTParsedFormula::operator=(const std::wstring& value)
{
	ParsedFormula::operator = (value);
	return *this;
}

BiffStructurePtr FRTParsedFormula::clone()
{
    return BiffStructurePtr(new FRTParsedFormula(*this));
}

void FRTParsedFormula::load(XLS::CFRecord& record)
{   
    _UINT32 cce;
	_UINT32 cb;

    record >> cce;
    record >> cb;

    rgce.load(record, cce);
    //rgcb.load(record, rgce.getPtgs(), true);
}

void FRTParsedFormula::save(XLS::CFRecord& record)
{
	_UINT32 cce = 0;
	_UINT32 cb = 0;

	record << cce;
	record << cb;

	auto rdPtr = record.getRdPtr();

	rgce.save(record);

	cce = record.getRdPtr() - rdPtr;

	record.RollRdPtrBack(cce + 8);
	record << cce;
	record.skipNunBytes(cce + 4);

}

} // namespace XLS

