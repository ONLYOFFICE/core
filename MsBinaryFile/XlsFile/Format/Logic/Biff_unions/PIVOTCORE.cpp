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

} // namespace XLS

