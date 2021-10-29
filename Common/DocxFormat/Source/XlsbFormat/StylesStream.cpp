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

#include "StylesStream.h"

#include "Biff12_records/CommonRecords.h"
#include "Biff12_records/BeginStyleSheet.h"
#include "Biff12_unions/FMTS.h"
#include "Biff12_unions/FONTS.h"
#include "Biff12_unions/FILLS.h"
#include "Biff12_unions/BORDERS.h"
#include "Biff12_unions/CELLSTYLEXFS.h"
#include "Biff12_unions/CELLXFS.h"
#include "Biff12_unions/STYLES.h"
#include "Biff12_unions/DXFS.h"
#include "Biff12_unions/TABLESTYLES.h"
#include "Biff12_unions/COLORPALETTE.h"
#include "Biff12_unions/FRTSTYLESHEET.h"
#include "Biff12_records/EndStyleSheet.h"

using namespace XLS;

namespace XLSB
{

StylesStream::StylesStream()
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

            case rt_BeginStyles:
            {
                if (proc.optional<STYLES>())
                {
                    m_STYLES = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginDXFs:
            {
                if (proc.optional<DXFS>())
                {
                    m_DXFS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginTableStyles:
            {
                if (proc.optional<TABLESTYLES>())
                {
                    m_TABLESTYLES = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginColorPalette:
            {
                if (proc.optional<COLORPALETTE>())
                {
                    m_COLORPALETTE = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_FRTBegin:
            {
                if (proc.optional<FRTSTYLESHEET>())
                {
                    m_FRTSTYLESHEET = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_EndStyleSheet:
            {
                if (proc.optional<EndStyleSheet>())
                {
                    m_BrtEndStyleSheet = elements_.back();
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
