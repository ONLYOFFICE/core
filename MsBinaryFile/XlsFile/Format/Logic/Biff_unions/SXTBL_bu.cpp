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

#include "SXTBL.h"
#include "DREF.h"

#include "../Biff_records/SXTbl.h"
#include "../Biff_records/SxTbpg.h"
#include "../Biff_records/SXTBRGIITM.h"
#include "../Biff_records/SXString.h"

#include "../Biff_records/DConName.h"
#include "../Biff_records/DConBin.h"
#include "../Biff_records/DConRef.h"
namespace XLS
{


SXTBL::SXTBL()
{
}


SXTBL::~SXTBL()
{
}

class Parenthesis_SXTBL_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXTBL_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXTBL_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXTBRGIITM>())
		{
			return false;
		}

		proc.repeated<SXString>(0, 0);
		return true;
	};
};


BaseObjectPtr SXTBL::clone()
{
	return BaseObjectPtr(new SXTBL(*this));
}


// SXTBL = SXTbl *DREF *SxTbpg *(SXTBRGIITM *SXString)
const bool SXTBL::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXTbl>())
	{
		return false;
	}

	m_SXTbl = elements_.back();
	elements_.pop_back();

	int count =0;
	
	count  = proc.repeated<DREF>(0, 0);
	while(!elements_.empty())
	{
		m_arDREF.push_back(elements_.front()); 
		elements_.pop_front();
	}

	count  = proc.repeated<SxTbpg>(0, 0);
	while(!elements_.empty())
	{
		m_arSxTbpg.push_back(elements_.front()); 
		elements_.pop_front();
	}	

	count = proc.repeated<Parenthesis_SXTBL_1>(0, 0);
	while(!elements_.empty())
	{
		SXTBRGIITM* item = dynamic_cast<SXTBRGIITM*>(elements_.front().get());

		if (item)
		{
			_sxtbl_item it;
			it.item = elements_.front();
			m_arSXTBRGIITM.push_back(it); 
		}
		else
		{
			m_arSXTBRGIITM.back().strings.push_back(elements_.front());
		}
		elements_.pop_front();
	}	
	return true;
}

const bool SXTBL::saveContent(BinProcessor& proc)
{
    if(m_SXTbl == nullptr)
        return false;
    {
        auto castedPtr = static_cast<SXTbl*>(m_SXTbl.get());
        castedPtr->cdref = m_arDREF.size();
        castedPtr->csxtbpg = m_arSxTbpg.size();
    }
    proc.mandatory(*m_SXTbl);
    for(auto i : m_arDREF)
        if(i!= nullptr)
            proc.mandatory(*i);
    for(auto i: m_arSxTbpg)
        if(i!= nullptr)
            proc.mandatory(*i);
    for(auto i: m_arSXTBRGIITM)
    {
        proc.mandatory(*i.item);
        for(auto j : i.strings)
            if(j!= nullptr)
                proc.mandatory(*j);
    }
    return true;
}

int SXTBL::serialize(std::wostream & strm)
{
	if (!m_SXTbl) return 0;

	SXTbl *tbl = dynamic_cast<SXTbl*>(m_SXTbl.get());
	
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"cacheSource")
		{
			CP_XML_ATTR(L"type", L"consolidation");

			CP_XML_NODE(L"consolidation")
			{
				CP_XML_NODE(L"pages")
				{
					CP_XML_ATTR(L"count", m_arSXTBRGIITM.size());

					for (size_t i = 0; i < m_arSXTBRGIITM.size(); i++)
					{
						SXTBRGIITM * item = dynamic_cast<SXTBRGIITM*>(m_arSXTBRGIITM[i].item.get());
						
						CP_XML_NODE(L"page")
						{
							CP_XML_ATTR(L"count", item->cItems);

							for (size_t j = 0; j < m_arSXTBRGIITM[i].strings.size(); j++)
							{
								SXString* str = dynamic_cast<SXString*>(m_arSXTBRGIITM[i].strings[j].get());
								CP_XML_NODE(L"pageItem")
								{
									CP_XML_ATTR(L"name", str->segment.value());
								}
							}
						}
					}
				}
				CP_XML_NODE(L"rangeSets")
				{
					CP_XML_ATTR(L"count", m_arDREF.size());
					for (size_t i = 0; i < m_arDREF.size(); i++)
					{
						DREF*		dref = dynamic_cast<DREF*>(m_arDREF[i].get());

						DConName*	name	= dynamic_cast<DConName*>(dref->m_DCon.get());
						DConBin*	bin		= dynamic_cast<DConBin*>(dref->m_DCon.get());
						DConRef*	ref		= dynamic_cast<DConRef*>(dref->m_DCon.get());

						CP_XML_NODE(L"rangeSet")
						{
							CP_XML_ATTR(L"i1", i);
							if (name)
							{
								CP_XML_ATTR(L"name", name->stName.value());
							}
							if (bin)
							{
								switch(bin->nBuiltin)
								{
									case 0x0000:	CP_XML_ATTR(L"name", L"_xlnm.Consolidate_Area"); break;
									case 0x0001:	CP_XML_ATTR(L"name", L"_xlnm.Auto_Open");		break;
									case 0x0002:	CP_XML_ATTR(L"name", L"_xlnm.Auto_Close");		break;
									case 0x0003:	CP_XML_ATTR(L"name", L"_xlnm.Extract");			break;
									case 0x0004:	CP_XML_ATTR(L"name", L"_xlnm.Database");		break;
									case 0x0005:	CP_XML_ATTR(L"name", L"_xlnm.Criteria");		break;
									case 0x0006:	CP_XML_ATTR(L"name", L"_xlnm.Print_Area");		break;
									case 0x0007:	CP_XML_ATTR(L"name", L"_xlnm.Print_Titles");	break;
									case 0x0008:	CP_XML_ATTR(L"name", L"_xlnm.Recorder");		break;
									case 0x0009:	CP_XML_ATTR(L"name", L"_xlnm.Data_Form");		break;
									case 0x000a:	CP_XML_ATTR(L"name", L"_xlnm.Auto_Activate");	break;
									case 0x000b:	CP_XML_ATTR(L"name", L"_xlnm.Auto_Deactivate");	break;
									case 0x000c:	CP_XML_ATTR(L"name", L"_xlnm.Sheet_Title");		break;
									case 0x000d:	CP_XML_ATTR(L"name", L"_xlnm._FilterDatabase");	break;	//??
								}
							}
							if (ref)
							{
								ref->check_external();

								CP_XML_ATTR(L"ref", ref->ref.toString());
								CP_XML_ATTR(L"sheet", ref->sheet_name);

								if (ref->index_external >= 0)
								{
									CP_XML_ATTR(L"r:id", L"extId" + std::to_wstring(ref->index_external + 1));
								}
							}	
						}
					}
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

