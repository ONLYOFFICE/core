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

#include "SXADDLGROUP.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>

namespace XLS
{


SXADDLGROUP::SXADDLGROUP()
{
}


SXADDLGROUP::~SXADDLGROUP()
{
}


BaseObjectPtr SXADDLGROUP::clone()
{
	return BaseObjectPtr(new SXADDLGROUP(*this));
}


/*
SXADDLGROUP = SXAddl_SXCGroup_SXDId *Continue_SxaddlSxString SXAddl_SXCGroup_SXDGrpInfo 
				*SXAddl_SXCGroup_SXDMember *UNKNOWNFRT SXAddl_SXCGroup_SXDEnd
*/
const bool SXADDLGROUP::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCGroup_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.mandatory<SXAddl_SXCGroup_SXDGrpInfo>();
	proc.repeated<SXAddl_SXCGroup_SXDMember>(0, 0);
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.mandatory<SXAddl_SXCGroup_SXDEnd>();

	return true;
}

} // namespace XLS

