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

#include "Feature11.h"
#include "ContinueFrt11.h"

namespace XLS
{

Feature11::Feature11(bool bFeat12) : bFeature12(bFeat12)
{
}

Feature11::~Feature11()
{
}

BaseObjectPtr Feature11::clone()
{
	return BaseObjectPtr(new Feature11(*this));
}

void Feature11::readFields(CFRecord& record)
{
	size_t sz = continue_records.size();
	std::list<CFRecordPtr>& recs = continue_records[rt_ContinueFrt11];

	while (!recs.empty())
	{
		ContinueFrt11 continueFrt;
		continueFrt.readFields(*recs.front());

		record.appendRawData(continueFrt.pData.get(), continueFrt.nData);
		recs.pop_front();
	}	

	record >> frtRefHeaderU;
	record >> isf;
	
	record.skipNunBytes(1);		// reserved1
	record.skipNunBytes(4);		// reserved2
	
	record >> cref2;
	record >> cbFeatData;
	record.skipNunBytes(2);		// reserved3

	for (int i = 0; i < cref2 ; ++i)
	{
		Ref8U reff;
		record >> reff;
		refs2.push_back(BiffStructurePtr(new Ref8U(reff)));
		sqref += reff.toString() + ((i == cref2 - 1) ? L"" : L" ");
	}
	if (cbFeatData == 0)
	{
		cbFeatData = record.getDataSize() - (cref2 * 8) - 27;
	}

	rgbFeat.bFeature12 = bFeature12;
	rgbFeat.cbFeatData = cbFeatData;
	record >> rgbFeat;	
}

} // namespace XLS

