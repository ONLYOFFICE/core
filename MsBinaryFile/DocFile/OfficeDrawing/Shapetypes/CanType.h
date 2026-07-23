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
  class CanType: public ShapeType
  {
    public: 
	  CanType():
      ShapeType(msosptCan)
      {
        ShapeConcentricFill = true;

        Joins = round;

        Path = L"m10800,qx0@1l0@2qy10800,21600,21600@2l21600@1qy10800,xem0@1qy10800@0,21600@1nfe";

        Formulas.push_back( L"val #0" );
        Formulas.push_back( L"prod #0 1 2" );
        Formulas.push_back( L"sum height 0 @1" );

		Adjustments.push_back(5400);

        ConnectorLocations = L"10800,@0;10800,0;0,10800;10800,21600;21600,10800";

		ConnectorAngles.push_back(270);
		ConnectorAngles.push_back(270);
		ConnectorAngles.push_back(180);
		ConnectorAngles.push_back(90);
		ConnectorAngles.push_back(0);

        TextBoxRectangle = L"0,@0,21600,@2";

        Handle HandleOne;
        HandleOne.position = L"center,#0";
        HandleOne.yrange = L"0,10800";
        Handles.push_back( HandleOne );
	  }

	  virtual ~CanType()
	  {
	  }
  };
}
