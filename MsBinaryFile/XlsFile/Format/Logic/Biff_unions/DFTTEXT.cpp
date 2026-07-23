/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

