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

#include "../ShapeType.h"

namespace DocFileFormat
{
	class HeartType : public ShapeType
	{
	public:
		HeartType () : ShapeType(msosptHeart)
		{
			ShapeConcentricFill	= true;
			Joins				= miter;
            Path				= L"m10860,2187c10451,1746,9529,1018,9015,730,7865,152,6685,,5415,,4175,152,2995,575,1967,1305,1150,2187,575,3222,242,4220,,5410,242,6560,575,7597l10860,21600,20995,7597v485,-1037,605,-2187,485,-3377c21115,3222,20420,2187,19632,1305,18575,575,17425,152,16275,,15005,,13735,152,12705,730v-529,288,-1451,1016,-1845,1457xe";

			Adjustments.push_back(21600);

            ConnectorLocations	= L"10860,2187;2928,10800;10860,21600;18672,10800";

			ConnectorAngles.push_back(270);
			ConnectorAngles.push_back(180);
			ConnectorAngles.push_back(90);
			ConnectorAngles.push_back(0);

            TextBoxRectangle	= L"5037,2277,16557,13677";
		}
	};
}
