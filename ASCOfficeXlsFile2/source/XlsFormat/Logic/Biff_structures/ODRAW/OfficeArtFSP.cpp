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

#include "OfficeArtFSP.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


OfficeArtFSP::OfficeArtFSP()
:	OfficeArtRecord(0x02, FSP)
{
}


XLS::BiffStructurePtr OfficeArtFSP::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFSP(*this));
}


void OfficeArtFSP::storeFields(XLS::CFRecord& record)
{
	_UINT32 flags = 0;

	SETBIT(flags, 0, fGroup);
	SETBIT(flags, 1, fChild);
	SETBIT(flags, 2, fPatriarch);
	SETBIT(flags, 3, fDeleted);
	SETBIT(flags, 4, fOleShape);
	SETBIT(flags, 5, fHaveMaster);
	SETBIT(flags, 6, fFlipH);
	SETBIT(flags, 7, fFlipV);
	SETBIT(flags, 8, fConnector);
	SETBIT(flags, 9, fHaveAnchor);
	SETBIT(flags, 10, fBackground);
	SETBIT(flags, 11, fHaveSpt);

	record << spid << flags;
}


void OfficeArtFSP::loadFields(XLS::CFRecord& record)
{
	_UINT32 flags;
	record >> spid >> flags;

	fGroup = GETBIT(flags, 0);
	fChild = GETBIT(flags, 1);
	fPatriarch = GETBIT(flags, 2);
	fDeleted = GETBIT(flags, 3);
	fOleShape = GETBIT(flags, 4);
	fHaveMaster = GETBIT(flags, 5);
	fFlipH = GETBIT(flags, 6);
	fFlipV = GETBIT(flags, 7);
	fConnector = GETBIT(flags, 8);
	fHaveAnchor = GETBIT(flags, 9);
	fBackground = GETBIT(flags, 10);
	fHaveSpt = GETBIT(flags, 11);

	shape_id = rh_own.recInstance;
}


const unsigned short OfficeArtFSP::GetInstanceToStore()
{
	return shape_id;
}


} // namespace XLS
