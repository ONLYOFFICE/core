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

#include "BookBool.h"

namespace XLS
{

BookBool::BookBool()
{
}


BookBool::~BookBool()
{
}


BaseObjectPtr BookBool::clone()
{
	return BaseObjectPtr(new BookBool(*this));
}

void BookBool::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fNoSaveSup			= GETBIT(flags, 0);
	fHasEnvelope		= GETBIT(flags, 2);
	fEnvelopeVisible	= GETBIT(flags, 3);
	fEnvelopeInitDone	= GETBIT(flags, 4);
	
	unsigned char grUpdateLinks_num = GETBITS(flags, 5, 6);
	switch(grUpdateLinks_num)
	{
		case 0:
			grUpdateLinks = std::wstring (L"userSet");
			break;
		case 1:
			grUpdateLinks = std::wstring (L"never");
			break;
		case 2:
			grUpdateLinks = std::wstring (L"always");
			break;
		default:
			// EXCEPT::RT::WrongBiffRecord("Unsupported value of grUpdateLinks.", record.getTypeString());
			break;
	}
	
	fHideBorderUnselLists = GETBIT(flags, 8);
}

void BookBool::writeFields(CFRecord& record)
{
    unsigned short flags = 0;
    SETBIT(flags, 0, fNoSaveSup)
    SETBIT(flags, 2, fHasEnvelope)
    SETBIT(flags, 3, fEnvelopeVisible)
    SETBIT(flags, 4, fEnvelopeInitDone)
    unsigned char grUpdateLinks_num = 0;
    if(grUpdateLinks == L"never")
        grUpdateLinks_num = 1;
    else if(grUpdateLinks == L"always")
        grUpdateLinks_num = 2;
    SETBITS(flags, 5, 6, grUpdateLinks_num)
    SETBIT(flags, 8, fHideBorderUnselLists)
    record << flags;
}

} // namespace XLS

