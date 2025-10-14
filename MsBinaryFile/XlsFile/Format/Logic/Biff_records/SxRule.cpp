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

#include "SxRule.h"

namespace XLS
{

SxRule::SxRule()
{
}

SxRule::~SxRule()
{
}

BaseObjectPtr SxRule::clone()
{
	return BaseObjectPtr(new SxRule(*this));
}


void SxRule::readFields(CFRecord& record)
{
	unsigned short flags, reserved;
	record >> iDim >> isxvd >> flags >> reserved >> csxFilt >> irwFirst >> irwLast >> icolFirst >> icolLast;

	sxaxisRw		= GETBIT(flags, 0);
	sxaxisCol		= GETBIT(flags, 1);
	sxaxisPage		= GETBIT(flags, 2);
	sxaxisData		= GETBIT(flags, 3);
	sxrType			= GETBITS(flags, 4, 7);
	fPart			= GETBIT(flags, 8);
	fDataOnly		= GETBIT(flags, 9);
	fLabelOnly		= GETBIT(flags, 10);
	fGrandRw		= GETBIT(flags, 11);
	fGrandCol		= GETBIT(flags, 12);
	fGrandRwSav		= GETBIT(flags, 13);
	fCacheBased		= GETBIT(flags, 14);
	fGrandColSav	= GETBIT(flags, 15);
}

void SxRule::writeFields(CFRecord& record)
{
    unsigned short flags = 0;
    SETBIT(flags, 0, sxaxisRw)
    SETBIT(flags, 1, sxaxisCol)
    SETBIT(flags, 2, sxaxisPage)
    SETBIT(flags, 3, sxaxisData)
    SETBITS(flags, 4, 7, sxrType)
    SETBIT(flags, 8, fPart)
    SETBIT(flags, 9, fDataOnly)
    SETBIT(flags, 10, fLabelOnly)
    SETBIT(flags, 11, fGrandRw)
    SETBIT(flags, 12, fGrandCol)
    SETBIT(flags, 13, fGrandRwSav)
    SETBIT(flags, 14, fCacheBased)
    SETBIT(flags, 15, fGrandColSav)

    record << iDim << isxvd << flags;
    record.reserveNunBytes(2);
    record << csxFilt;
    if(fPart)
    {
        record << irwFirst << irwLast << icolFirst << icolLast;
    }

}

} // namespace XLS

