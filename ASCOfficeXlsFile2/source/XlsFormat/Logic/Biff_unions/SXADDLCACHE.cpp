/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "SXADDLCACHE.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLCACHE12.h>
#include <Logic/Biff_unions/SXADDLDBQUERY.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>

namespace XLS
{


SXADDLCACHE::SXADDLCACHE()
{
}


SXADDLCACHE::~SXADDLCACHE()
{
}


// SXADDLCACHE = SXAddl_SXCCache_SXDId SXAddl_SXCCache_SXDVer10Info [SXAddl_SXCCache_SXDVerSXMacro] 
BaseObjectPtr SXADDLCACHE::clone()
{
	return BaseObjectPtr(new SXADDLCACHE(*this));
}


//               [SXADDLCACHE12] [SXADDLDBQUERY] *UNKNOWNFRT SXAddl_SXCCache_SXDEnd
const bool SXADDLCACHE::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCCache_SXDId>())
	{
		return false;
	}
	proc.mandatory<SXAddl_SXCCache_SXDVer10Info>();
	proc.optional<SXAddl_SXCCache_SXDVerSXMacro>();
	proc.optional<SXADDLCACHE12>();
	proc.optional<SXADDLDBQUERY>();
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.mandatory<SXAddl_SXCCache_SXDEnd>();
	return true;
}

} // namespace XLS

