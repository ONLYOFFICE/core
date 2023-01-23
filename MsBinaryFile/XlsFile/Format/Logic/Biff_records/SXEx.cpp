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

#include "SXEx.h"

namespace XLS
{

SXEx::SXEx()
{
	cWrapPage = 0;
}


SXEx::~SXEx()
{
}


BaseObjectPtr SXEx::clone()
{
	return BaseObjectPtr(new SXEx(*this));
}

void SXEx::readFields(CFRecord& record)
{
	record >> csxformat >> cchErrorString >> cchNullString >> cchTag >> csxselect;

	_UINT32 flags;
	record >> crwPage >> ccolPage >> flags;

	fAcrossPageLay				= GETBIT(flags, 0);
	cWrapPage					= GETBITS(flags, 1, 9);
	fEnableWizard				= GETBIT(flags, 16);
	fEnableDrilldown			= GETBIT(flags, 17);
	fEnableFieldDialog			= GETBIT(flags, 18);
	fPreserveFormatting			= GETBIT(flags, 19);
	fMergeLabels				= GETBIT(flags, 20);
	fDisplayErrorString			= GETBIT(flags, 21);
	fDisplayNullString			= GETBIT(flags, 22);
	fSubtotalHiddenPageItems	= GETBIT(flags, 23);

	record >> cchPageFieldStyle >> cchTableStyle >> cchVacateStyle;

	if (cchErrorString > 0 && cchErrorString != 0xffff)
	{
		stError.setSize(cchErrorString);
		record >> stError;
	}
	if (cchNullString > 0 && cchNullString != 0xffff)
	{
		stDisplayNull.setSize(cchNullString);
		record >> stDisplayNull;
	}
	if (cchTag > 0 && cchTag != 0xffff)
	{
		stTag.setSize(cchTag);
		record >> stTag;
	}
	if (cchPageFieldStyle > 0 && cchPageFieldStyle != 0xffff)
	{
		stPageFieldStyle.setSize(cchPageFieldStyle);
		record >> stPageFieldStyle;
	}
	if (cchTableStyle > 0 && cchTableStyle != 0xffff)
	{
		stTableStyle.setSize(cchTableStyle);
		record >> cchTableStyle;
	}
	if (cchVacateStyle > 0 && cchVacateStyle != 0xffff)
	{
		stVacateStyle.setSize(cchVacateStyle);	
		record >> cchVacateStyle;
	}
	
	int skip = record.getDataSize() - record.getRdPtr();
	record.skipNunBytes(skip);
}

} // namespace XLS

