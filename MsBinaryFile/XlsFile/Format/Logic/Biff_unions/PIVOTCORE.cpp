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

#include "PIVOTCORE.h"
#include "PIVOTVD.h"
#include "PIVOTIVD.h"
#include "PIVOTPI.h"
#include "PIVOTLI.h"
#include "PIVOTEX.h"

#include "../Biff_records/SXDI.h"
#include "../Biff_records/SxView.h"

namespace XLS
{

PIVOTCORE::PIVOTCORE() : bOLAP( false)
{
}

PIVOTCORE::~PIVOTCORE()
{
}

BaseObjectPtr PIVOTCORE::clone()
{
	return BaseObjectPtr(new PIVOTCORE(*this));
}

// PIVOTCORE = SxView *PIVOTVD *2PIVOTIVD [PIVOTPI] *SXDI *PIVOTLI PIVOTEX
const bool PIVOTCORE::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SxView>())
	{
		return false;
	}
	m_SxView = elements_.back();
	elements_.pop_back();

	SxView* sxView = dynamic_cast<SxView*>(m_SxView.get());

	int count = 0;
	
	count = proc.repeated<PIVOTVD>(0, 0);
	while(count--)
	{
		m_arPIVOTVD.push_back(elements_.front());	elements_.pop_front();
	}
	
	count = proc.repeated<PIVOTIVD>(0, 2);
	while(count--)
	{
		m_arPIVOTIVD.push_back(elements_.front());	elements_.pop_front();
	}
	if (proc.optional<PIVOTPI>())
	{
		m_PIVOTPI= elements_.back();
		elements_.pop_back();
	}
	
	count = proc.repeated<SXDI>(0, 0);
	while(count--)
	{
		m_arSXDI.push_back(elements_.front());	elements_.pop_front();
	}
	PIVOTLI rwLines(sxView->cDimRw);
	if (proc.optional(rwLines))
	{
		m_arPIVOTLI.push_back(elements_.front());	elements_.pop_front();
	}
	PIVOTLI colLines(sxView->cDimCol);
	if (proc.optional(colLines))
	{
		m_arPIVOTLI.push_back(elements_.front());	elements_.pop_front();
	}	

	if (proc.mandatory<PIVOTEX>())
	{
		m_PIVOTEX = elements_.back();
		elements_.pop_back();
	}
	
	return true;
}

const bool PIVOTCORE::saveContent(BinProcessor& proc)
{
    if(m_SxView == nullptr)
        return false;
    proc.mandatory(*m_SxView);
    for(auto i : m_arPIVOTVD)
        if(i!= nullptr)
            proc.mandatory(*i);
	for(auto i : m_arPIVOTIVD)	//rows beforee columns
        if(i!= nullptr)
            proc.mandatory(*i);
    if(m_PIVOTPI != nullptr)
        proc.mandatory(*m_PIVOTPI);
    for(auto i : m_arSXDI)
        if(i!= nullptr)
            proc.mandatory(*i);
    for(auto i : m_arPIVOTLI)
        if(i!= nullptr)
            proc.mandatory(*i);
    if(m_PIVOTEX != nullptr)
        proc.mandatory(*m_PIVOTEX);
	else
		proc.mandatory<PIVOTEX>();
    return true;
}

} // namespace XLS

