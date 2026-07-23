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
#pragma once

#include "../BiffStructure.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

template<class Type>
class IMsoArray : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(IMsoArray)
public:
	XLS::BiffStructurePtr clone(){return XLS::BiffStructurePtr(new IMsoArray(*this));}

	static const XLS::ElementType type = XLS::typeIMsoArray;

	void load(IBinaryReader* reader)
	{
		unsigned short nElems, nElemsAlloc;
		unsigned short cbElem;
				
		nElems		= reader->ReadUInt16();
		nElemsAlloc = reader->ReadUInt16();
		cbElem		= reader->ReadUInt16();
		
		for (unsigned short i = 0; i < nElems; i++)
		{
			Type element(cbElem);
			element.load(reader);

			data.push_back(element);
		}
	}

	virtual void load(XLS::CFRecord& record)
	{
		unsigned short nElems, nElemsAlloc;
		unsigned short cbElem;
		
		size_t pos1 = record.getRdPtr();
		
		record >> nElems >> nElemsAlloc >> cbElem;
		
		for (unsigned short i = 0; i < nElems; i++)
		{
			Type element(cbElem);
			record >> element;
			data.push_back(element);
		}
	}

	std::vector<Type>	data;
	unsigned int		op;
};



} // namespace XLS
