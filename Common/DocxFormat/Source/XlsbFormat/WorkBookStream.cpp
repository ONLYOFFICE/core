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
#include "Biff12_records/BookProtection.h"
#include "Biff12_records/BookProtectionIso.h"
#include "Biff12_unions/FRT.h"

using namespace XLS;

namespace XLSB
{

WorkBookStream::WorkBookStream()
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

            case rt_BookProtectionIso:
            {
                if (proc.optional<BookProtectionIso>())
                {
                    m_BrtBookProtectionIso = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BookProtection:
            {
                if (proc.optional<BookProtection>())
                {
                    m_BrtBookProtection = elements_.back();
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
                while (true)
                {
                    if(proc.optional<UserBookView>())
                    {
                        m_arBrtUserBookView.push_back(elements_.back());
                        elements_.pop_back();

                        while (proc.optional<FRT>())
                        {
                            //m_arFRT.push_back(elements_.back());
                            elements_.pop_back();
                        }
                    }
                    else break;
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

	return true;
}

} // namespace XLSB
