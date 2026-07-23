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

#include "BopPop.h"
#include "BopPopCustom.h"

namespace XLS
{

BopPop::BopPop()
{
}


BopPop::~BopPop()
{
}


BaseObjectPtr BopPop::clone()
{
	return BaseObjectPtr(new BopPop(*this));
}


void BopPop::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> pst >> fAutoSplit >> split >> iSplitPos >> pcSplitPercent >> pcPie2Size >> pcGap >> numSplitValue >> flags;
	
	fHasShadow = GETBIT(flags, 0);
}

void BopPop::writeFields(CFRecord& record)
{
    unsigned short flags = 0;
    SETBIT(flags, 0, fHasShadow);
    record << pst << fAutoSplit << split << iSplitPos << pcSplitPercent << pcPie2Size << pcGap << numSplitValue << flags;
}

int BopPop::serialize(std::wostream & _stream)
{
 	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:ofPieType")
		{
			if (pst == (unsigned char)1)	CP_XML_ATTR (L"val" , L"pie");
			else							CP_XML_ATTR (L"val" , L"bar");
		}
		CP_XML_NODE(L"c:gapWidth")
		{
			CP_XML_ATTR (L"val", pcGap);
		}

		if (fAutoSplit == (unsigned char)0)
		{
			CP_XML_NODE(L"c:splitType")
			{
				switch(split)
				{
				case 0: CP_XML_ATTR (L"val" , L"pos");		break;
				case 1: CP_XML_ATTR (L"val" , L"val");		break;
				case 2: CP_XML_ATTR (L"val" , L"percent");	break;
				case 3: CP_XML_ATTR (L"val" , L"cust");		break;
				}
			}
			if (split < 3)
			{
				CP_XML_NODE(L"c:splitPos")
				{
					if (split == 0)
						CP_XML_ATTR (L"val", iSplitPos);
					else if (split == 1)
						CP_XML_ATTR (L"val", numSplitValue.data.value);
					else if (split == 2)
						CP_XML_ATTR (L"val", pcSplitPercent);
				}
			}
		}
		else
		{
		}
		
		BopPopCustom * custom = dynamic_cast<BopPopCustom *>(m_Custom.get());
		if (split == (_UINT16)3 && custom)
		{
			CP_XML_NODE(L"c:custSplit")
			{
				for (size_t i = 0 ; i < custom->rggrbit.pie_indices.size(); i++)
				{
					CP_XML_NODE(L"c:secondPiePt")
					{
						CP_XML_ATTR (L"val", custom->rggrbit.pie_indices[i]);
					}
				}
			}
		}
		CP_XML_NODE(L"c:secondPieSize")
		{
			CP_XML_ATTR (L"val", pcPie2Size);
		}
	}
	return 0;
}
} // namespace XLS

