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

#include "FeatHdr11.h"

namespace XLS
{

FeatHdr11::FeatHdr11()
{
}


FeatHdr11::~FeatHdr11()
{
}


BaseObjectPtr FeatHdr11::clone()
{
	return BaseObjectPtr(new FeatHdr11(*this));
}

void FeatHdr11::readFields(CFRecord& record)
{
	record >> frt;
	record >> isf;

//		ISFPROTECTION	= 0x0002, // Specifies the enhanced protection type. 
//		ISFFEC2			= 0x0003, // Specifies the ignored formula errors type.
//		ISFFACTOID		= 0x0004, // Specifies the smart tag type.
//		ISFLIST			= 0x0005, // Specifies the list type.

	record.skipNunBytes(1);		// reserved1
	record.skipNunBytes(4);		// reserved2
	record.skipNunBytes(4);		// reserved3
	record >> idListNext;
	record.skipNunBytes(2);		// reserved4
}

void FeatHdr11::writeFields(CFRecord& record)
{
	frt.rt = 0x0871;
	record << frt  << isf;
	unsigned char reserved1 = 1;
	unsigned int reserved2 = 0xFFFFFFFF;
	record << reserved1 << reserved2 << reserved2 << idListNext;
	record.reserveNunBytes(2);
}

} // namespace XLS

