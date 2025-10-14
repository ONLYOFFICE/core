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

#include "DFTTEXT.h"
#include "ATTACHEDLABEL.h"

#include "../Biff_records/DataLabExt.h"
#include "../Biff_records/DataLabExtContents.h"
#include "../Biff_records/StartObject.h"
#include "../Biff_records/DefaultText.h"
#include "../Biff_records/EndObject.h"

namespace XLS
{


DFTTEXT::DFTTEXT()
{
}


DFTTEXT::~DFTTEXT()
{
}


BaseObjectPtr DFTTEXT::clone()
{
	return BaseObjectPtr(new DFTTEXT(*this));
}


// DFTTEXT = [DataLabExt StartObject] DefaultText ATTACHEDLABEL [EndObject]
const bool DFTTEXT::loadContent(BinProcessor& proc)
{
	if(proc.optional<DataLabExt>())
	{
		m_DataLabExt = elements_.back();
		elements_.pop_back();

		if (proc.optional<StartObject>())	elements_.pop_back();

	}
	
	if(proc.mandatory<DefaultText>())
	{
		m_DefaultText = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<DataLabExtContents>())
	{
		m_DataLabExtContents = elements_.back();
		elements_.pop_back();
	}	
	
	if (!m_DefaultText && !m_DataLabExtContents)
		return false;

	if (proc.mandatory<ATTACHEDLABEL>())
	{
		m_ATTACHEDLABEL = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<EndObject>()) elements_.pop_back();

	return true;
}

const bool DFTTEXT::saveContent(BinProcessor& proc)
{
	if(m_DefaultText == nullptr)
		return false;
	if(m_DataLabExt != nullptr)
	{
		proc.mandatory(*m_DataLabExt);
		proc.mandatory<StartObject>();
	}
	proc.mandatory(*m_DefaultText);
	if(m_ATTACHEDLABEL != nullptr)
		proc.mandatory(*m_ATTACHEDLABEL);
	if(m_DataLabExt != nullptr)
		proc.mandatory<EndObject>();
	return true;
}

} // namespace XLS

