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

#include "PtgElfRadicalLel.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgElfRadicalLel::clone()
{
	return BiffStructurePtr(new PtgElfRadicalLel(*this));
}


void PtgElfRadicalLel::storeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fQuoted);
	record << ilel << flags;
}


void PtgElfRadicalLel::loadFields(CFRecord& record)
{
	unsigned short flags;
	record >> ilel >> flags;
	fQuoted = GETBIT(flags, 0);
}


void PtgElfRadicalLel::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	// The significant tag follows (PtgArea or PtgAreaErr)
}


} // namespace XLS

