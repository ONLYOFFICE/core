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

