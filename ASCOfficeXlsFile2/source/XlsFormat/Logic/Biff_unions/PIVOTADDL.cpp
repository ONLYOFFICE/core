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

#include "PIVOTADDL.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLCALCMEMBER.h>
#include <Logic/Biff_unions/SXADDLHIERARCHY.h>
#include <Logic/Biff_unions/SXADDLFIELD.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>
#include <Logic/Biff_unions/SXADDLCONDFMTS.h>
#include <Logic/Biff_unions/SXADDLSXFILTERS12.h>

namespace XLS
{


PIVOTADDL::PIVOTADDL()
{
}


PIVOTADDL::~PIVOTADDL()
{
}
/*
int b = 5;
boost::shared_ptr<int> by;

void raw_ptr(const int* ptr) // указатель на константный объект.
{
	ptr = &b;  // компилится, потому что указатель не константный
	*ptr = 4;  // не компилится, потому что объект константный
}
// то же самое для shared_ptr
void raw_ptr(boost::shared_ptr<const int> ptr) // указатель на константный объект.
{
	ptr = by;  // компилится, потому что указатель не константный
	*ptr = 4;  // не компилится, потому что объект константный
}
// ===================== теперь наоборот
void raw_ptr2(int* const ptr) // константный указатель на неконстантный объект.
{
	ptr = &b;  // не компилится, потому что указатель - константный
	*ptr = 4;  // компилится, потому что объект не константный
}
// то же самое для shared_ptr
void raw_ptr2(const boost::shared_ptr<int> ptr) // константный указатель на не константный объект.
{
	ptr = by;  // не компилится, потому что указатель - константный
	*ptr = 4;  // компилится, потому что объект не константный
}
*/


BaseObjectPtr PIVOTADDL::clone()
{
	return BaseObjectPtr(new PIVOTADDL(*this));
}


/*
PIVOTADDL = SXAddl_SXCView_SXDId *Continue_SxaddlSxString [SXAddl_SXCView_SXDVer10Info] 
			[SXAddl_SXCView_SXDVer12Info] *SXADDLCALCMEMBER *SXADDLHIERARCHY *SXADDLFIELD *UNKNOWNFRT 
			[SXAddl_SXCView_SXDTableStyleClient] [SXAddl_SXCView_SXDCompactRwHdr *Continue_SxaddlSxString] 
			[SXAddl_SXCView_SXDCompactColHdr *Continue_SxaddlSxString] [SXAddl_SXCView_SXDVerUpdInv] 
			[SXADDLCONDFMTS] [SXADDLSXFILTERS12] *SXAddl_SXCView_SXDVerUpdInv *SXAddl_SXCView_SXDSXPIIvmb 
			[SXAddl_SXCView_SXDVerUpdInv] SXAddl_SXCView_SXDEnd
*/
const bool PIVOTADDL::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCView_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.optional<SXAddl_SXCView_SXDVer10Info>();
	proc.optional<SXAddl_SXCView_SXDVer12Info>();
	proc.repeated<SXADDLCALCMEMBER>(0, 0);
	proc.repeated<SXADDLHIERARCHY>(0, 0);
	proc.repeated<SXADDLFIELD>(0, 0);
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.optional<SXAddl_SXCView_SXDTableStyleClient>();

	if(proc.optional<SXAddl_SXCView_SXDCompactRwHdr>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCView_SXDCompactColHdr>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}

	proc.optional<SXAddl_SXCView_SXDVerUpdInv>();
	proc.optional<SXADDLCONDFMTS>();
	proc.optional<SXADDLSXFILTERS12>();
	proc.repeated<SXAddl_SXCView_SXDVerUpdInv>(0, 0);
	proc.repeated<SXAddl_SXCView_SXDSXPIIvmb>(0, 0);
	proc.optional<SXAddl_SXCView_SXDVerUpdInv>();
	proc.mandatory<SXAddl_SXCView_SXDEnd>();
	return true;
}

} // namespace XLS

