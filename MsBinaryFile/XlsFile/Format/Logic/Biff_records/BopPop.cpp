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

