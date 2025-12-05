/*
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

#include "SHAPEPROPS.h"
#include "../Biff_records/ShapePropsStream.h"
#include "../Biff_records/ContinueFrt12.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

SHAPEPROPS::SHAPEPROPS()
{
}


SHAPEPROPS::~SHAPEPROPS()
{
}


BaseObjectPtr SHAPEPROPS::clone()
{
	return BaseObjectPtr(new SHAPEPROPS(*this));
}


// SHAPEPROPS = ShapePropsStream *ContinueFrt12
const bool SHAPEPROPS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<ShapePropsStream>())
	{
		return false;
	}
	m_ShapePropsStream = elements_.back();
	elements_.pop_back();

	proc.repeated<ContinueFrt12>(0, 0);

	return true;
}

const bool SHAPEPROPS::saveContent(BinProcessor& proc)
{
	if(m_ShapePropsStream == nullptr)
		return false;
	proc.mandatory(*m_ShapePropsStream);
	auto castedPtr = static_cast<ShapePropsStream*>(m_ShapePropsStream.get());
	const auto limit = 8200;
	if(castedPtr->xml_.size() > limit)
	{
		auto xmlPos = limit;
		while(xmlPos < castedPtr->xml_.size())
		{
			ContinueFrt12 continueRecord;
			continueRecord.frtHeader.rt = rt_ContinueFrt12;
			auto continueLength = 0;
			if((castedPtr->xml_.size() - xmlPos) > limit)
				continueLength = limit;
			else
				continueLength = castedPtr->xml_.size() - xmlPos;
			continueRecord.rgb = std::vector<char>(
				castedPtr->xml_.begin() + xmlPos,
				castedPtr->xml_.begin() + xmlPos + continueLength);
			proc.mandatory(continueRecord);
			xmlPos += continueLength;

		}
	}
	return true;
}

int SHAPEPROPS::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
	}
	return 0;
}

} // namespace XLS

