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

#include "VerticalPageBreaks.h"

namespace XLS
{
	VerticalPageBreaks::VerticalPageBreaks() {}
	VerticalPageBreaks::~VerticalPageBreaks() {}

	BaseObjectPtr VerticalPageBreaks::clone()
	{
		return BaseObjectPtr(new VerticalPageBreaks(*this));
	}

	void VerticalPageBreaks::readFields(CFRecord& record)
	{
		record >> cbrk;
		for (int i = 0; i < cbrk; ++i)
		{
			VertBrkPtr vb(new VertBrk);
			record >> *vb;
			rgbrk.push_back(vb);
		}
	}
    void VerticalPageBreaks::writeFields(CFRecord& record)
    {
        cbrk = rgbrk.size();
        record << cbrk;
        for (auto i:rgbrk)
        {
            record << *i;
        }
    }
	int VerticalPageBreaks::serialize(std::wostream & stream)
	{
		if (rgbrk.empty()) return 0;

		CP_XML_WRITER(stream)
		{
			CP_XML_NODE(L"colBreaks")
			{
				CP_XML_ATTR(L"count", cbrk);
				CP_XML_ATTR(L"manualBreakCount", cbrk);

				for (size_t i = 0; i < rgbrk.size(); ++i)
				{
					if (rgbrk[i])
						rgbrk[i]->serialize(CP_XML_STREAM());
				}
			}
		}
		return 0;
	}
} // namespace XLS

