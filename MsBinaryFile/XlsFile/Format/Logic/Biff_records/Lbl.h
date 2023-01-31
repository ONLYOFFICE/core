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

#include "BiffRecord.h"
#include "../Biff_structures/BiffString.h"
#include "../Biff_structures/NameParsedFormula.h"
#include "../../../../../OOXML/XlsbFormat/Biff12_structures/XLWideString.h"


namespace XLS
{

class Lbl: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Lbl)
	BASE_OBJECT_DEFINE_CLASS_NAME(Lbl)
public:
	Lbl();
	~Lbl();

	BaseObjectPtr clone();

	void readFields(CFRecord& record);

	static const ElementType	type = typeLbl;

	bool fHidden;
	bool fFunc;
	bool fOB;
	bool fProc;
	bool fCalcExp;
	bool fBuiltin;
    //unsigned char	fGrp;
    unsigned short	fGrp;
	bool fPublished;
	bool fWorkbookParam;

    bool fFutureFunction; //biff12
	
    unsigned char   chKey;
    //_UINT16					itab;
    _UINT32 itab;
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
class Lbl_BIFF34 : public Lbl
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Lbl_BIFF34)
	BASE_OBJECT_DEFINE_CLASS_NAME(Lbl_BIFF34)
public:
	Lbl_BIFF34();
	~Lbl_BIFF34();

	BaseObjectPtr clone();
};

} // namespace XLS

