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

#include "BeginPCDField.h"

using namespace XLS;

namespace XLSB
{

    BeginPCDField::BeginPCDField()
    {
    }

    BeginPCDField::~BeginPCDField()
    {
    }

    BaseObjectPtr BeginPCDField::clone()
    {
        return BaseObjectPtr(new BeginPCDField(*this));
    }

    void BeginPCDField::readFields(XLS::CFRecord& record)
    {
        _UINT16 flags;
        _UINT32 value;
        record >> flags;

        fServerBased            = GETBIT(flags, 0);
        fCantGetUniqueItems     = GETBIT(flags, 1);
        fSrcField               = GETBIT(flags, 2);
        fCaption                = GETBIT(flags, 3);
        fOlapMemPropField       = GETBIT(flags, 4);
        fLoadFmla               = GETBIT(flags, 8);
        fLoadPropName           = GETBIT(flags, 9);

        record >> ifmt >> wTypeSql >> ihdb >> isxtl >> cIsxtmps;

        record >> stFldName;

        if(fCaption)
            record >> stFldCaption;

        if(fLoadFmla)
            record >> fldFmla;

        if(cIsxtmps > 0)
            record >> cbRgisxtmp;

        for(size_t i = 0; i < cIsxtmps; ++i)
        {
            record >> value;
            rgisxtmp.push_back(value);
        }

        if(fLoadPropName)
            record >> stMemPropName;
    }

	void BeginPCDField::writeFields(XLS::CFRecord& record)
	{
		_UINT16 flags = 0;

		SETBIT(flags, 0, fServerBased)
		SETBIT(flags, 1, fCantGetUniqueItems)
		SETBIT(flags, 2, fSrcField)
		SETBIT(flags, 3, fCaption)
		SETBIT(flags, 4, fOlapMemPropField)
		SETBIT(flags, 8, fLoadFmla)
		SETBIT(flags, 9, fLoadPropName)

		record << flags;

		cIsxtmps = rgisxtmp.size();

		record << ifmt << wTypeSql << ihdb << isxtl << cIsxtmps;

		record << stFldName;

		if (fCaption)
			record << stFldCaption;

		if (fLoadFmla)
			record << fldFmla;

		if (cIsxtmps > 0)
			record << cbRgisxtmp;

		for (auto& item : rgisxtmp)
		{
			record << item;
		}

		if (fLoadPropName)
			record << stMemPropName;
	}

} // namespace XLSB

