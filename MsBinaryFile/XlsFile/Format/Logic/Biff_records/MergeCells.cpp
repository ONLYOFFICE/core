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

#include "MergeCells.h"

namespace XLS
{

MergeCells::MergeCells()
{
}


MergeCells::~MergeCells()
{
}


BaseObjectPtr MergeCells::clone()
{
	return BaseObjectPtr(new MergeCells(*this));
}


void MergeCells::readFields(CFRecord& record)
{
	record >> cmcs;
	for(int i = 0; i < cmcs; ++i)
	{
		BiffStructurePtr ref8(new Ref8);
		record >> *ref8;
		rgref.push_back(ref8);
	}
}

void MergeCells::writeFields(CFRecord& record)
{
    cmcs = rgref.size();
	record << cmcs;
    for(auto i : rgref)
        record << *i;
}

int MergeCells::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {		
        for (size_t i = 0 ; i < rgref.size(); i++)
		{	
			Ref8* ref = dynamic_cast<Ref8*>(rgref[i].get());
			CP_XML_NODE(L"mergeCell")
			{
				CP_XML_ATTR(L"ref", ref->toString());
			}	
		}
	}
	return 0;
}

} // namespace XLS

