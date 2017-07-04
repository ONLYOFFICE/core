/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "SXLI.h"

namespace XLS
{

SXLI::SXLI()
{
}


SXLI::~SXLI()
{
}

BaseObjectPtr SXLI::clone()
{
	return BaseObjectPtr(new SXLI(*this));
}

void SXLI::readFields(CFRecord& record)
{
	while(true)
	{
		int size_item = record.getDataSize() - record.getRdPtr(); 

		if (size_item < 8)
		{
			break;
		}
		SXLIItem item;
		
		unsigned short flags;

		record >> item.cSic >> item.itmType >> item.isxviMac >> flags;
		
		item.fMultiDataName		= GETBIT(flags, 0);
		item.iData				= GETBITS(flags, 1, 8);
		item.fSbt				= GETBIT(flags, 9);
		item.fBlock				= GETBIT(flags, 10);
		item.fGrand				= GETBIT(flags, 11);
		item.fMultiDataOnAxis	= GETBIT(flags, 12);

		//if (!item.fGrand && item.itmType != 0x000E)
		{
			for (short i = 0; i < item.isxviMac; i++)
			{
				short val; record >> val;
				item.rgisxvi.push_back(val);
			}
		}
		m_arItems.push_back(item);
	}
}

} // namespace XLS

