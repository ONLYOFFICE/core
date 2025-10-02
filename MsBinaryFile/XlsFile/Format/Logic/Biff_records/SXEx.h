/*
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
#pragma once

#include "BiffRecord.h"
#include "../Biff_structures/BiffString.h"
#include "../Biff_structures/CellRangeRef.h"

namespace XLS
{

class SXEx: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXEx)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXEx)
public:
	SXEx();
	~SXEx();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);
    void writeFields(CFRecord& record);

	static const ElementType type = typeSXEx;

    unsigned short	csxformat = 0;
    unsigned short	cchErrorString = 0xFFFF;
    unsigned short	cchNullString = 0xFFFF;
    unsigned short	cchTag = 0xFFFF;
    unsigned short	csxselect = 0;

    DRw				crwPage = 0;
    ColU			ccolPage = 0;

    bool			fAcrossPageLay = false;
    unsigned char	cWrapPage = 0;
    bool			fEnableWizard = false;
	bool			fEnableDrilldown = true;
    bool			fEnableFieldDialog = false;
    bool			fPreserveFormatting = false;
	bool			fMergeLabels = true;
	bool			fDisplayErrorString = true;
	bool			fDisplayNullString = true;
	bool			fSubtotalHiddenPageItems = true;

	unsigned short	cchPageFieldStyle = 0xFFFF;
	unsigned short	cchTableStyle = 0xFFFF;
	unsigned short	cchVacateStyle = 0xFFFF;

	XLUnicodeStringNoCch	stError;
	XLUnicodeStringNoCch	stDisplayNull;
	XLUnicodeStringNoCch	stTag;
	XLUnicodeStringNoCch	stPageFieldStyle;
	XLUnicodeStringNoCch	stTableStyle;
	XLUnicodeStringNoCch	stVacateStyle;
};

} // namespace XLS

