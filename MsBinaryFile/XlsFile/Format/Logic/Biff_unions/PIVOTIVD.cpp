﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "PIVOTIVD.h"
#include "../Biff_records/SxIvd.h"
#include "../Biff_records/Continue.h"

namespace XLS
{

PIVOTIVD::PIVOTIVD()
{
}


PIVOTIVD::~PIVOTIVD()
{
}


BaseObjectPtr PIVOTIVD::clone()
{
	return BaseObjectPtr(new PIVOTIVD(*this));
}

// PIVOTIVD = SxIvd *Continue
const bool PIVOTIVD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SxIvd>())
	{
		return false;
	}
	m_SxIvd = elements_.back();
	elements_.pop_back();

	int count = proc.repeated<Continue>(0, 0);

	return true;
}

const bool PIVOTIVD::saveContent(BinProcessor& proc)
{
    if(m_SxIvd == nullptr)
        return false;
    proc.mandatory(*m_SxIvd);
    auto castedPtr = static_cast<SxIvd*>(m_SxIvd.get());
    const auto maxRecSize = 8224;
    const auto continueElemSize = 2;
    while(!castedPtr->rgSxivd.empty())
    {
        Continue continueRecord;
        if(castedPtr->rgSxivd.size() < maxRecSize/continueElemSize)
            continueRecord.m_iDataSize = castedPtr->rgSxivd.size() * continueElemSize;
        else
            continueRecord.m_iDataSize = maxRecSize;
        continueRecord.m_pData = new char[continueRecord.m_iDataSize];
        memcpy(continueRecord.m_pData, castedPtr->rgSxivd.data(), continueRecord.m_iDataSize);
        proc.mandatory(continueRecord);
        castedPtr->rgSxivd.erase(castedPtr->rgSxivd.begin(),
            castedPtr->rgSxivd.begin() + (continueRecord.m_iDataSize/continueElemSize));
    }
    return true;
}

int PIVOTIVD::serialize(std::wostream & strm)
{
	SxIvd* vd = dynamic_cast<SxIvd*>(m_SxIvd.get());

	CP_XML_WRITER(strm)
	{
		for (size_t i = 0; i < vd->rgSxivd.size(); i++)
		{
			CP_XML_NODE(L"field")
			{ 
				CP_XML_ATTR(L"x", vd->rgSxivd[i]); 
			}
		}
	}
	return 0;
}
} // namespace XLS

