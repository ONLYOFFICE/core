/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

#include "BeginMdxTuple.h"

using namespace XLS;

namespace XLSB
{
    BeginMdxTuple::BeginMdxTuple()
    {
    }

    BeginMdxTuple::~BeginMdxTuple()
    {
    }

    BaseObjectPtr BeginMdxTuple::clone()
    {
        return BaseObjectPtr(new BeginMdxTuple(*this));
    }

    void BeginMdxTuple::readFields(XLS::CFRecord& record)
    {
        _UINT16 flags = 0;

        record >> cMbrs >> dwSrvFmtBack >> dwSrvFmtFore >> flags;
        fSrvFmtNum = GETBIT(flags, 0);
        fSrvFmtNumCurrency = GETBIT(flags, 1);
        fSrvFmtNumStr = GETBIT(flags, 2);
        fSrvFmtBack = GETBIT(flags, 3);
        fSrvFmtFore = GETBIT(flags, 4);
        fSrvFmtItalic = GETBIT(flags, 5);
        fSrvFmtUnderline = GETBIT(flags, 6);
        fSrvFmtBold = GETBIT(flags, 7);
        fSrvFmtStrikethrough = GETBIT(flags, 8);
        if(fSrvFmtNum)
        {
            if(fSrvFmtNumCurrency)
            {
                record >> stSfnum;
            }
            else
            {
                dSfnum = SrvFmtDataPtr(new SrvFmtData);
                dSfnum->load(record);
            }
        }
    }

	void BeginMdxTuple::writeFields(XLS::CFRecord& record)
	{
        _UINT16 flags = 0;
        SETBIT(flags, 0, fSrvFmtNum);
        SETBIT(flags, 1, fSrvFmtNumCurrency);
        SETBIT(flags, 2, fSrvFmtNumStr);
        SETBIT(flags, 3, fSrvFmtBack);
        SETBIT(flags, 4, fSrvFmtFore);
        SETBIT(flags, 5, fSrvFmtItalic);
        SETBIT(flags, 6, fSrvFmtUnderline);
        SETBIT(flags, 7, fSrvFmtBold);
        SETBIT(flags, 8, fSrvFmtStrikethrough);

        record << cMbrs << dwSrvFmtBack << dwSrvFmtFore << flags;
        if(fSrvFmtNum)
        {
            if(fSrvFmtNumCurrency)
            {
                if(stSfnum.getStructSizeWouldWritten())
                {
                    record << stSfnum;
                }
            }
            else 
            {
                if(dSfnum)
                {
                    dSfnum->save(record);
                }
                else
                {
                    SrvFmtData numData;
                    numData.save(record);
                }
            }
        } 

	}
} // namespace XLSB
