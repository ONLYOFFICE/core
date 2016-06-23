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

#include "SXADDLHIERARCHY.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLGRPLEVEL.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>

namespace XLS
{


SXADDLHIERARCHY::SXADDLHIERARCHY()
{
}


SXADDLHIERARCHY::~SXADDLHIERARCHY()
{
}


BaseObjectPtr SXADDLHIERARCHY::clone()
{
	return BaseObjectPtr(new SXADDLHIERARCHY(*this));
}


/*
SXADDLHIERARCHY = SXAddl_SXCHierarchy_SXDId *Continue_SxaddlSxString *SXAddl_SXCHierarchy_SXDProperty 
				*SXADDLGRPLEVEL [SXAddl_SXCHierarchy_SXDVerUpdInv] *SXAddl_SXCHierarchy_SXDFilterMember 
				[SXAddl_SXCHierarchy_SXDVerUpdInv] 
				[SXAddl_SXCHierarchy_SXDSXSetParentUnique *Continue_SxaddlSxString] [SXAddl_SXCHierarchy_SXDIconSet] 
				[SXAddl_SXCHierarchy_SXDUserCaption *Continue_SxaddlSxString] 
				*UNKNOWNFRT [SXAddl_SXCHierarchy_SXDVerUpdInv] *SXAddl_SXCHierarchy_SXDUserCaption 
				[SXAddl_SXCHierarchy_SXDVerUpdInv] [SXAddl_SXCHierarchy_SXDInfo12] 
				[SXAddl_SXCHierarchy_SXDDisplayFolder *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDMeasureGrp *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDParentKPI *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDKPIValue *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDKPIGoal *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDKPIStatus *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDKPITrend *Continue_SxaddlSxString] 
				[SXAddl_SXCHierarchy_SXDKPIWeight *Continue_SxaddlSxString]
				[SXAddl_SXCHierarchy_SXDKPITime *Continue_SxaddlSxString] SXAddl_SXCHierarchy_SXDEnd
*/
const bool SXADDLHIERARCHY::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCHierarchy_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.repeated<SXAddl_SXCHierarchy_SXDProperty>(0, 0);
	proc.repeated<SXADDLGRPLEVEL>(0, 0);
	proc.optional<SXAddl_SXCHierarchy_SXDVerUpdInv>();

	if(proc.optional<SXAddl_SXCHierarchy_SXDSXSetParentUnique>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	proc.optional<SXAddl_SXCHierarchy_SXDIconSet>();
	if(proc.optional<SXAddl_SXCHierarchy_SXDUserCaption>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.optional<SXAddl_SXCHierarchy_SXDVerUpdInv>();
	proc.repeated<SXAddl_SXCHierarchy_SXDUserCaption>(0, 0);
	proc.optional<SXAddl_SXCHierarchy_SXDVerUpdInv>();
	proc.optional<SXAddl_SXCHierarchy_SXDInfo12>();
	if(proc.optional<SXAddl_SXCHierarchy_SXDDisplayFolder>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDMeasureGrp>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDParentKPI>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPIValue>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPIGoal>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPIStatus>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPITrend>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPIWeight>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCHierarchy_SXDKPITime>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}

	proc.mandatory<SXAddl_SXCHierarchy_SXDEnd>();

	return true;
}

} // namespace XLS

