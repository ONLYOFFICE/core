﻿/*
 * (c) Copyright Ascensio System SIA 2010-2021
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "BeginPCDSCSet.h"

using namespace XLS;

namespace XLSB
{

    BeginPCDSCSet::BeginPCDSCSet()
    {
    }

    BeginPCDSCSet::~BeginPCDSCSet()
    {
    }

    BaseObjectPtr BeginPCDSCSet::clone()
    {
        return BaseObjectPtr(new BeginPCDSCSet(*this));
    }

    void BeginPCDSCSet::readFields(XLS::CFRecord& record)
    {
        _UINT32 index;
        BYTE    flags;

        for(size_t i = 0; i < 4; ++i)
        {
            record >> index;
            rgiItem[i] = index;
        }

        record >> fName >> fBuiltIn >> flags;

        fLoadRelId  = GETBIT(flags, 0);
        fLoadSheet  = GETBIT(flags, 1);

        if(fLoadSheet)
            record >> irstSheet;

        if(fLoadRelId)
            record >> irstRelId;

        if(!fName.value().get())
            record >> rfx;
        else
            record >> irstName;
    }

	void BeginPCDSCSet::writeFields(XLS::CFRecord& record)
	{
		_UINT32 index;
		BYTE    flags = 0;

		for (auto& item : rgiItem)
		{
			record << item;
		}

		SETBIT(flags, 0, fLoadRelId)
		SETBIT(flags, 1, fLoadSheet)

		record << fName << fBuiltIn << flags;

		if (fLoadSheet)
			record << irstSheet;

		if (fLoadRelId)
			record << irstRelId;

		if (!fName.value().get())
			record << rfx;
		else
			record << irstName;
	}

} // namespace XLSB

