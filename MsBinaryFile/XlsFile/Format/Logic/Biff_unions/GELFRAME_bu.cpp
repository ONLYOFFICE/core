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

#include "GELFRAME.h"
#include "PICF.h"
#include "../Biff_records/GelFrame.h"
#include "../Biff_records/Continue.h"


#include "../../../Converter/XlsConverter.h"
#include "../../../Converter/xlsx_conversion_context.h"

namespace XLS
{


GELFRAME::GELFRAME()
{
}


GELFRAME::~GELFRAME()
{
}


BaseObjectPtr GELFRAME::clone()
{
	return BaseObjectPtr(new GELFRAME(*this));
}


// GELFRAME = 1*2GelFrame *Continue [PICF]
const bool GELFRAME::loadContent(BinProcessor& proc)
{
	global_info = proc.getGlobalWorkbookInfo();

	int count = proc.repeated<GelFrame>(1, 2);

	if (elements_.empty()) return false;

	if (count > 0)
	{
		m_GelFrame = elements_.front();
		GelFrame * base = dynamic_cast<GelFrame*>(m_GelFrame.get());
		GelFrame * addit = count > 1 ? dynamic_cast<GelFrame*>(elements_.back().get()) : NULL;

		if (base && addit)
		{
			base->concatinate(addit);
		}
		elements_.clear();
	}

	if (proc.optional<PICF>())
	{
		m_PICF = elements_.back();
		elements_.pop_back();
	}

	return true;
}

int GELFRAME::serialize(std::wostream & _stream)
{
	GelFrame * frame = dynamic_cast<GelFrame*>(m_GelFrame.get());
	if (frame == NULL) return 0;

	global_info->xls_converter->xlsx_context->get_drawing_context().clear_fill();

	global_info->xls_converter->convert(&frame->OPT1);
	global_info->xls_converter->convert(&frame->OPT2);

	global_info->xls_converter->xlsx_context->get_drawing_context().serialize_fill(_stream);
	//global_info->xls_converter->xlsx_context->get_drawing_context().serialize_line(_stream);

	return 0;
}

} // namespace XLS

