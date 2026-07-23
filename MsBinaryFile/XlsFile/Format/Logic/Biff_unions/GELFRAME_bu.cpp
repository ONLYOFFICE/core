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

