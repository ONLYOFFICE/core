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
#pragma once

#include "../../../../OOXML/PPTXFormat/Logic/Timing/CTn.h"
#include "../../Records/Animations/ExtTimeNodeContainer.h"

namespace PPT
{
	namespace Intermediate
	{
		struct ShapeAnim
		{
			_INT32 spid = -1;
			_INT32 presetID = -1;   // where 1 is any animation or simpless: appear

			inline bool IsValid()const
			{
				return spid > 0 && presetID > 0;
			}
		};

		bool operator==(const ShapeAnim &s1, const ShapeAnim &s2);

		// look at enum TimeNodeLevel
		ShapeAnim ParseExisting3Level_CTn(const PPTX::Logic::CTn &oCTn);
		ShapeAnim ParseExisting3Level_ETNC(const CRecordExtTimeNodeContainer *pETNC);

		ShapeAnim ParseExisting4Level_CTn(const PPTX::Logic::CTn &oCTn);
		ShapeAnim ParseExisting4Level_ETNC(const CRecordExtTimeNodeContainer *pETNC);

		ShapeAnim ParseExisting5Level_CTn(const PPTX::Logic::CTn &oCTn);
		ShapeAnim ParseExisting5Level_ETNC(const CRecordExtTimeNodeContainer *pETNC);

		CRecordTimeBehaviorContainer* GetTimeBehaviorContainer(CRecordExtTimeNodeContainer *pETNC);
		_INT32 GetSpID(CRecordExtTimeNodeContainer *pETNC);

	}
}
