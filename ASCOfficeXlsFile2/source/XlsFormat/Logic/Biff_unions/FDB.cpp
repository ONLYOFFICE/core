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

#include "FDB.h"

#include "SXFMLA_bu.h"
#include "SXOPER.h"
#include "SXRANGE.h"

#include "../Biff_records/SXFDB.h"
#include "../Biff_records/SXFDBType.h"

namespace XLS
{


FDB::FDB()
{
}


FDB::~FDB()
{
}


BaseObjectPtr FDB::clone()
{
	return BaseObjectPtr(new FDB(*this));
}

// FDB			= SXFDB SXFDBType [SXFMLA / (*GRPSXOPER [SXRANGE / *(SxIsxoper *Continue)])] *SRCSXOPER
// SXRANGE		= SXOPER
// SRCSXOPER	= SXOPER

const bool FDB::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXFDB>())
	{
		return false;
	}
	m_SXFDB = elements_.back();
	elements_.pop_back();

	if(proc.mandatory<SXFDBType>())
	{
		m_SXFDBType = elements_.back();
		elements_.pop_back();
	}

	if(proc.optional<SXFMLA>())
	{
		m_SXFMLA = elements_.back();
		elements_.pop_back();
	}
	
	int count = proc.repeated<SXOPER>(0, 0);
	while(count--)
	{
		m_arSRCSXOPER.push_back(elements_.front());	elements_.pop_front();
	}

	return true;
}

} // namespace XLS

