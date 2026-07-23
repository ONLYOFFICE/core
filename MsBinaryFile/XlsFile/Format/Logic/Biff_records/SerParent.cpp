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

#include "../Biff_unions/SERIESFORMAT.h"

#include "SerParent.h"
#include "SerAuxErrBar.h"

namespace XLS
{

SerParent::SerParent()
{
}


SerParent::~SerParent()
{
}


BaseObjectPtr SerParent::clone()
{
	return BaseObjectPtr(new SerParent(*this));
}

void SerParent::readFields(CFRecord& record)
{
	record >> series;
}

void SerParent::writeFields(CFRecord& record)
{
	record << series;
}

void SerParent::recalc(size_t current_index, std::vector<BaseObjectPtr> & arSERIESFORMAT)
{
	SerAuxErrBar * err_bars_current	= dynamic_cast<SerAuxErrBar*>(m_SerAuxErrBar.get());

	for (size_t i = 0; i < arSERIESFORMAT.size(); i++)
	{
		if (current_index == i) continue;

		SERIESFORMAT * series_ext = dynamic_cast<SERIESFORMAT *>(arSERIESFORMAT[i].get());
		if (series_ext)
		{
			SerParent *parent = dynamic_cast<SerParent*>(series_ext->m_SerParent.get());
			if (parent)
			{
				SerAuxErrBar * err_bars	= dynamic_cast<SerAuxErrBar*>(parent->m_SerAuxErrBar.get());
				if (err_bars_current && err_bars)
				{
					if ((err_bars_current->sertm == 1 && err_bars->sertm == 2) ||
						(err_bars_current->sertm == 2 && err_bars->sertm == 1) )
					{
						err_bars_current->sertm = 5;
						
						parent->m_SerAuxErrBar = BaseObjectPtr();
					}
					if ((err_bars_current->sertm == 3 && err_bars->sertm == 4) ||
						(err_bars_current->sertm == 4 && err_bars->sertm == 3) )
					{
						err_bars_current->sertm = 6;

						parent->m_SerAuxErrBar = BaseObjectPtr();
					}
				}
			}
		}

	}
}


} // namespace XLS

