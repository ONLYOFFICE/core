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

#include "Ptg.h"

namespace XLS
{

class OperandPtg : public Ptg
{
public:
	OperandPtg() : dataType(ptg_NO_TYPE_DATA) {}

	enum PtgDataType
	{
		ptg_NO_TYPE_DATA = 0x0,	// Error value or type data is absent
		ptg_REFERENCE = 0x1,	// Specifies a reference to a range.
		ptg_VALUE = 0x2,		// Specifies a single value of a simple type. The type can be a Boolean, a number, a string, or an error code.
		ptg_ARRAY = 0x3			// Specifies an array of values.
	} dataType;
	
	OperandPtg(const unsigned short full_ptg_id) : Ptg(full_ptg_id)
	{
		dataType = (PtgDataType)GETBITS(full_ptg_id, 1, 2);
	}
};

typedef boost::shared_ptr<OperandPtg> OperandPtgPtr;

} // namespace XLS

