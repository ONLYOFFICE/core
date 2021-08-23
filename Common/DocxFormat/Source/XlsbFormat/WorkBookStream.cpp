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

#include "WorkBookStream.h"

#include "Biff12_records/CommonRecords.h"
#include "Biff12_records/BeginBook.h"
#include "Biff12_records/FileVersion.h"
#include "Biff12_records/FileSharingIso.h"
#include "Biff12_records/WbProp.h"
#include "Biff12_unions/ACABSPATH.h"
#include "Biff12_unions/BOOKVIEWS.h"
#include "Biff12_unions/BUNDLESHS.h"
#include "Biff12_unions/FNGROUP.h"
#include "Biff12_unions/EXTERNALS.h"
#include "Biff12_records/CalcProp.h"
#include "Biff12_records/OleSize.h"
#include "Biff12_records/EndBook.h"


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

WorkBookStream::WorkBookStream(const unsigned short code_page) :	code_page_(code_page)
{
}

WorkBookStream::~WorkBookStream()
{
}


BaseObjectPtr WorkBookStream::clone()
{
        return BaseObjectPtr(new WorkBookStream(*this));
}

const bool WorkBookStream::loadContent(BinProcessor& proc)
{
	int	count = 0;

	global_info_ = proc.getGlobalWorkbookInfo();
	
	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		if (type == rt_NONE) break;

		switch(type)
		{
            case rt_BeginBook:
            {
                if (proc.optional<BeginBook>())
                {
                    m_BrtBeginBook = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_FileVersion:
            {
                if (proc.optional<FileVersion>())
                {
                    m_BrtFileVersion = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_FileSharingIso:
            {
                if (proc.optional<FileSharingIso>())
                {
                    m_BrtFileSharingIso = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_FileSharing:
            {
                if (proc.optional<FileSharing>())
                {
                    m_BrtFileSharing = elements_.back();
                    elements_.pop_back();
                }

            }break;

            case rt_WbProp:
            {
                if (proc.optional<WbProp>())
                {
                    m_BrtWbProp = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_ACBegin:
            {
                if (proc.optional<ACABSPATH>())
                {
                    m_ACABSPATH = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginBookViews:
            {
                if (proc.optional<BOOKVIEWS>())
                {
                    m_BOOKVIEWS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginBundleShs:
            {
                if (proc.optional<BUNDLESHS>())
                {
                    m_BUNDLESHS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginFnGroup:
            {
                if (proc.optional<FNGROUP>())
                {
                    m_FNGROUP = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginExternals:
            {
                if (proc.optional<EXTERNALS>())
                {
                    m_EXTERNALS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_Name:
            {
                count = proc.repeated<Name>(0, 0);
                int start_pos = m_arBrtName.size();
                while(count > 0)
                {
                    m_arBrtName.insert(m_arBrtName.begin() + start_pos, elements_.back());
                    elements_.pop_back();
                    count--;
                }
            }break;

            case rt_CalcProp:
            {
                if (proc.optional<CalcProp>())
                {
                    m_BrtCalcProp = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_OleSize:
            {
                if (proc.optional<OleSize>())
                {
                    m_BrtOleSize = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_UserBookView:
            {
                count = proc.repeated<UserBookView>(0, 0);
                while(count > 0)
                {
                    m_arBrtUserBookView.insert(m_arBrtUserBookView.begin(), elements_.back());
                    elements_.pop_back();
                    count--;
                }
            }break;

            case rt_WebOpt:
            {
                if (proc.optional<WebOpt>())
                {
                    m_BrtWebOpt = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_FileRecover:
            {
                count = proc.repeated<FileRecover>(0, 0);
                while(count > 0)
                {
                    m_arBrtFileRecover.insert(m_arBrtFileRecover.begin(), elements_.back());
                    elements_.pop_back();
                    count--;
                }
            }break;

            case rt_EndBook:
            {
                if (proc.optional<EndBook>())
                {
                    m_BrtEndBook = elements_.back();
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
