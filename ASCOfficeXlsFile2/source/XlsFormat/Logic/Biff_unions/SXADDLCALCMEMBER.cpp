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

#include "SXADDLCALCMEMBER.h"
#include <Logic/Biff_records/SXAddl.h>

namespace XLS
{


SXADDLCALCMEMBER::SXADDLCALCMEMBER()
{
}


SXADDLCALCMEMBER::~SXADDLCALCMEMBER()
{
}


BaseObjectPtr SXADDLCALCMEMBER::clone()
{
	return BaseObjectPtr(new SXADDLCALCMEMBER(*this));
}


// SXADDLCALCMEMBER = (SXAddl_SXCView_SXDCalcMember [SXAddl_SXCView_SXDCalcMemString *Continue_SxaddlSxString])
const bool SXADDLCALCMEMBER::loadContent(BinProcessor& proc)
{
#pragma message("####################### SXADDLCALCMEMBER union is not implemented")
	Log::info("SXADDLCALCMEMBER union is not implemented!!!!!!!!!!!!!!!");

	if(!proc.mandatory<SXAddl_SXCView_SXDCalcMember>())
	{
		return false;
	}
	if(proc.optional<SXAddl_SXCView_SXDCalcMemString>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}

	return true;
}

} // namespace XLS

