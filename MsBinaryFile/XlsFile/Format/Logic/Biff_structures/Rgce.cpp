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

#include "Rgce.h"
#include "PtgFactory.h"

#include "PtgExp.h"
#include "PtgTbl.h"

#include "PtgRefN.h"
#include "PtgAreaN.h"
#include "PtgRef3d.h"
#include "PtgArea3d.h"

namespace XLS
{

Rgce::Rgce(const CellRef& cell_base_ref_init) :	cell_base_ref(cell_base_ref_init)
{
}

void Rgce::set_base_ref(const CellRef& cell_base_ref_new)
{
	cell_base_ref = cell_base_ref_new;

	for (size_t i = 0; i < sequence.size(); ++i)
	{
		PtgArea3d* area3d = dynamic_cast<PtgArea3d*>(sequence[i].get());
		if (area3d)
		{
			area3d->set_base_ref(cell_base_ref_new);
		}
		else
		{
			PtgRef3d* ref3d = dynamic_cast<PtgRef3d*>(sequence[i].get());
			if (ref3d)
			{
				ref3d->set_base_ref(cell_base_ref_new);
			}
			else 
			{
				PtgRefN* refN = dynamic_cast<PtgRefN*>(sequence[i].get());
				if (refN)
				{
					refN->set_base_ref(cell_base_ref_new);
				}
				else 
				{
					PtgAreaN* areaN = dynamic_cast<PtgAreaN*>(sequence[i].get());
					if (areaN)
					{
						areaN->set_base_ref(cell_base_ref_new);
					}
				}
			}
		}
	}
}

BiffStructurePtr Rgce::clone()
{
	return BiffStructurePtr(new Rgce(*this));
}

void Rgce::load(CFRecord& record, const size_t cce_val)
{
	cce = cce_val;
	size_t where_to_stop = record.getRdPtr() + cce_val;
	
	while(record.getRdPtr() < where_to_stop)
	{
        PtgPtr ptg = PtgFactory::loadPtg(record, cell_base_ref);
		if (ptg.get() == NULL)
			break;

		sequence.push_back(ptg);
	}
}

void Rgce::load(CFRecord& record)
{
	size_t where_to_stop = record.getRdPtr() + cce.get_value_or(0);
	while(record.getRdPtr() < where_to_stop)
	{
		sequence.push_back(PtgFactory::loadPtg(record, cell_base_ref));
	}
}


const size_t Rgce::getCCE() const
{
	return cce.get_value_or(0);
}


void Rgce::setCCE(const size_t cce_val)
{
	cce = cce_val;
}


const PtgVector& Rgce::getPtgs() const
{
	return sequence;
}


void Rgce::addPtg(const PtgPtr ptg)
{
	sequence.push_back(ptg);
}


const bool Rgce::isEmpty() const
{
	return sequence.empty();
}


const CellRef& Rgce::getLocation() const
{
	return cell_base_ref;
}

} // namespace XLS

