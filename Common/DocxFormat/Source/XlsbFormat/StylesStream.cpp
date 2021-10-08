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
#include "../../../../Common/MS-LCID.h"

#include "StylesStream.h"

#include "Biff12_records/CommonRecords.h"
#include "Biff12_records/BeginStyleSheet.h"
#include "Biff12_unions/FMTS.h"
#include "Biff12_unions/FONTS.h"
#include "Biff12_unions/FILLS.h"
#include "Biff12_unions/BORDERS.h"
#include "Biff12_unions/CELLSTYLEXFS.h"
#include "Biff12_unions/CELLXFS.h"

namespace XLSB
{

static const int aCodePages[][2] = {
    //charset	codepage
    0,	1252, //ANSI
    1,	0,//Default
    2,	42,//Symbol
    77,	10000,//Mac Roman
    78,	10001,//Mac Shift Jis
    79,	10003,//Mac Hangul
    80,	10008,//Mac GB2312
    81,	10002,//Mac Big5
    83,	10005,//Mac Hebrew
    84,	10004,//Mac Arabic
    85,	10006,//Mac Greek
    86,	10081,//Mac Turkish
    87,	10021,//Mac Thai
    88,	10029,//Mac East Europe
    89,	10007,//Mac Russian
    128,	932,//Shift JIS
    129,	949,//Hangul
    130,	1361,//Johab
    134,	936,//GB2312
    136,	950,//Big5
    238,	1250,//Greek
    161,	1253,//Greek
    162,	1254,//Turkish
    163,	1258,//Vietnamese
    177,	1255,//Hebrew
    178,	1256, //Arabic
    186,	1257,//Baltic
    204,	1251,//Russian
    222,	874,//Thai
    238,	1250,//Eastern European
    254,	437,//PC 437
    255,	850//OEM
};

StylesStream::StylesStream(const unsigned short code_page) :	code_page_(code_page)
{
}

StylesStream::~StylesStream()
{
}


BaseObjectPtr StylesStream::clone()
{
        return BaseObjectPtr(new StylesStream(*this));
}

const bool StylesStream::loadContent(BinProcessor& proc)
{
	global_info_ = proc.getGlobalWorkbookInfo();
	
	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		if (type == rt_NONE) break;

		switch(type)
		{
            case rt_BeginStyleSheet:
            {
                if (proc.optional<BeginStyleSheet>())
                {
                    m_BrtBeginStyleSheet = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginFmts:
            {
                if (proc.optional<FMTS>())
                {
                    m_FMTS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginFonts:
            {
                if (proc.optional<FONTS>())
                {
                    m_FONTS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginFills:
            {
                if (proc.optional<FILLS>())
                {
                    m_FILLS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginBorders:
            {
                if (proc.optional<BORDERS>())
                {
                    m_BORDERS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginCellStyleXFs:
            {
                if (proc.optional<CELLSTYLEXFS>())
                {
                    m_CELLSTYLEXFS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginCellXFs:
            {
                if (proc.optional<CELLXFS>())
                {
                    m_CELLXFS = elements_.back();
                    elements_.pop_back();
                }
            }break;

			default://skip					
			{
				proc.SkipRecord();	
            }break;
		}
	}

	if (global_info_->CodePage == 0)
	{	//try from charsets ... todooo make local set on each record (aka Label)
		//from ixfe -> ifnt from xf -> arFonts
		for (std::map<int, int>::iterator it = global_info_->fonts_charsets.begin()
			; global_info_->CodePage == 0 && it != global_info_->fonts_charsets.end()
			; ++it)
		{
			for (int i = 0 ; i < sizeof(aCodePages) / 2; i++)
			{
				if (aCodePages[i][0] == it->first)
				{
					global_info_->CodePage = aCodePages[i][1];
					break;
				}
			}
		}
	}
	if (global_info_->CodePage == 0 && global_info_->lcid_user > 0)
	{
		global_info_->CodePage = msLCID2DefCodePage(global_info_->lcid_user);
	}
	

	return true;
}

} // namespace XLSB
