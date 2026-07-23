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

#include  "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BiffAttribute.h"

namespace XLSB
{

class PivotItemType : public XLS::BiffAttributeSimple<BYTE>
{
public:
    XLS::BiffStructurePtr clone();
	PivotItemType& operator= (const BYTE& other_val);
	enum
	{
            PITDATA          = 0x00,
            PITDEFAULT       = 0x01,
            PITSUM           = 0x02,
            PITCOUNTA        = 0x03,
            PITAVG           = 0x04,
            PITMAX           = 0x05,
            PITMIN           = 0x06,
            PITPRODUCT       = 0x07,
            PITCOUNT         = 0x08,
            PITSTDDEV        = 0x09,
            PITSTDDEVP       = 0x0A,
            PITVAR           = 0x0B,
            PITVARP          = 0x0C,
            PITGRAND         = 0x0D,
            PITBLANK         = 0x0E
	};
};

} // namespace XLSB

