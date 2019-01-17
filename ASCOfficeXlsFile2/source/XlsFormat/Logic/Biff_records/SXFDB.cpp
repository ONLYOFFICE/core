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

#include "SXFDB.h"

namespace XLS
{

SXFDB::SXFDB()
{
}


SXFDB::~SXFDB()
{
}

BaseObjectPtr SXFDB::clone()
{
	return BaseObjectPtr(new SXFDB(*this));
}

void SXFDB::readFields(CFRecord& record)
{
	unsigned short	flags;
	record >> flags >> ifdbParent >> ifdbBase >> citmUnq >> csxoper >> cisxoper >> catm;
	record >> stFieldName;

	fAllAtoms			= GETBIT(flags, 0);
	fSomeUnhashed		= GETBIT(flags, 1);
	fUsed				= GETBIT(flags, 2);
	fHasParent			= GETBIT(flags, 3);
	fRangeGroup			= GETBIT(flags, 4);
	fNumField			= GETBIT(flags, 5);
	fTextEtcField		= GETBIT(flags, 7);
	fnumMinMaxValid		= GETBIT(flags, 8);
	fShortIitms			= GETBIT(flags, 9);
	fNonDates			= GETBIT(flags, 10);
	fDateInField		= GETBIT(flags, 11);
	fServerBased		= GETBIT(flags, 13);
	fCantGetUniqueItems	= GETBIT(flags, 14);
	fCalculatedField	= GETBIT(flags, 15);

	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	
	global_info->arPivotCacheFieldShortSize.push_back(fShortIitms);
	global_info->arPivotCacheFields.push_back(fAllAtoms);
}

} // namespace XLS

