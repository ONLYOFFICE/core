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

#include "AutoFilter12.h"

namespace XLS
{

AutoFilter12::AutoFilter12()
{
}


AutoFilter12::~AutoFilter12()
{
}


BaseObjectPtr AutoFilter12::clone()
{
	return BaseObjectPtr(new AutoFilter12(*this));
}


void AutoFilter12::readFields(CFRecord& record)
{
	_UINT16 flags;
	_UINT32 unused2;
	
	record >> frtRefHeader >> iEntry >> fHideArrow >> ft >> cft >> cCriteria >> cDateGroupings >> flags >> unused2 >> idList;

	_UINT16 _iEntry = iEntry;
	_UINT32 _fHideArrow = fHideArrow;
	_UINT32 _ft = ft;
	_UINT32 _cft = cft;
	_UINT32 _cCriteria = cCriteria;
	_UINT32 _cDateGroupings = cDateGroupings;
	_UINT32 _idList = idList;	

	// TODO доделать
	record.skipNunBytes(record.getDataSize() - record.getRdPtr());

	std::list<CFRecordPtr>& recs = continue_records[rt_ContinueFrt12];
	size_t size = recs.size();

	while( !recs.empty() )
	{
		record.appendRawData(recs.front());
		recs.pop_front();

		//
		FrtRefHeader frtRefHeaderContinue;
		record >> frtRefHeaderContinue;
		if (frtRefHeaderContinue.rt == 0x087F)
		{		
			if ( (ft == BIFF_DWORD(0)) && (cCriteria > 0) )
			{
				AF12CriteriaPtr item(new AF12Criteria);
				item->load(record);
				rgbAF12Criteries.push_back(item);
			}
		}		
	}
}

} // namespace XLS

