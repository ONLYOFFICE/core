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

#include "DConnParamBindingValType.h"

#include "BiffString.h"
#include "Xnum.h"

namespace XLS
{

BiffStructurePtr DConnParamBindingValType::clone()
{
	return BiffStructurePtr(new DConnParamBindingValType(*this));
}

void DConnParamBindingValType::load(CFRecord& record)
{
	record 	>> wBindingValueGrbit;

	switch(wBindingValueGrbit)
	{
		case 0x0001:	rgbBindingValue.reset(new Xnum);						break;
		case 0x0002:	rgbBindingValue.reset(new XLUnicodeStringSegmented);	break;
		case 0x0004:	rgbBindingValue.reset(new DConnParamBindingValByte);	break;
		case 0x0800:	rgbBindingValue.reset(new DConnParamBindingValInt);		break;
	}
	if (rgbBindingValue)
	{
		rgbBindingValue->load(record);
	}
}
//-----------------------------------------------------------------------------------
BiffStructurePtr DConnParamBindingValByte::clone()
{
	return BiffStructurePtr(new DConnParamBindingValByte(*this));
}

void DConnParamBindingValByte::load(CFRecord& record)
{
	unsigned char	reserved1;
	unsigned short	reserved2;
	_UINT32			reserved3;
	record 	>> fVal >> reserved1 >> reserved2 >> reserved3;

}
//-----------------------------------------------------------------------------------
BiffStructurePtr DConnParamBindingValInt::clone()
{
	return BiffStructurePtr(new DConnParamBindingValInt(*this));
}

void DConnParamBindingValInt::load(CFRecord& record)
{
	_UINT32	 reserved;

	record 	>> val >> reserved;

}
} // namespace XLS

