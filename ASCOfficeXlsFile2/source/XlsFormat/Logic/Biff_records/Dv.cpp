/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "Dv.h"

namespace XLS
{

BaseObjectPtr Dv::clone()
{
	return BaseObjectPtr(new Dv(*this));
}


void Dv::writeFields(CFRecord& record)
{

}


void Dv::readFields(CFRecord& record)
{
	_UINT32 flags;
	record >> flags;

	valType = static_cast<unsigned char>(GETBITS(flags, 0, 3));
	errStyle = static_cast<unsigned char>(GETBITS(flags, 4, 6));
	
	fStrLookup	= GETBIT(flags, 7);
	fAllowBlank = GETBIT(flags, 8);
	fSuppressCombo = GETBIT(flags, 9);
	mdImeMode = static_cast<unsigned char>(GETBITS(flags, 10, 17));
	fShowInputMsg = GETBIT(flags, 18);
	fShowErrorMsg = GETBIT(flags, 19);
	typOperator = static_cast<unsigned char>(GETBITS(flags, 20, 23));

	record >> PromptTitle >> ErrorTitle >> Prompt >> Error;
	formula1.load(record);
	formula2.load(record);
	record >> sqref;
}

} // namespace XLS

