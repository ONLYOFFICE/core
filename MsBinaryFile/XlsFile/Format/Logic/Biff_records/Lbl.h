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

#include "BiffRecord.h"
#include "../Biff_structures/BiffString.h"
#include "../Biff_structures/NameParsedFormula.h"
#include "../Biff_structures/BIFF12/XLWideString.h"


namespace XLS
{

// Logical representation of Lbl record in BIFF8 and BrtName in BIFF12
class Lbl: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Lbl)
	BASE_OBJECT_DEFINE_CLASS_NAME(Lbl)
public:
	Lbl();
	~Lbl();

	BaseObjectPtr clone();

	void readFields(CFRecord& record) override;
	void writeFields(CFRecord& record) override;

	static const ElementType	type = typeLbl;

    bool fHidden = false;
    bool fFunc = false;
    bool fOB = false;
    bool fProc = false;
    bool fCalcExp = false;
    bool fBuiltin = false;
    //unsigned char	fGrp;
    unsigned short	fGrp = 0;
    bool fPublished = false;
    bool fWorkbookParam = false;

    bool fFutureFunction = false; //biff12
	
    unsigned char   chKey = 0;
    //_UINT16					itab;
    _UINT32 itab = 0;
    XLUnicodeStringNoCch        Name_bin;
    std::wstring                Name;
    XLSB::XLNameWideString      name; //biff12
    NameParsedFormula           rgce; //formula in biff12
    XLSB::XLNullableWideString  comment; //biff12
    XLSB::XLNullableWideString  unusedstring1; //biff12
    XLSB::XLNullableWideString  description; //biff12
    XLSB::XLNullableWideString  helpTopic; //biff12
    XLSB::XLNullableWideString  unusedstring2; //biff12

};

} // namespace XLS

