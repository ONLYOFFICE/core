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
  class PlaqueType: public ShapeType
  {
    public: 
	  PlaqueType():
      ShapeType(msosptPlaque)
      {
        ShapeConcentricFill = true;
        Joins = miter;
        Path = L"m@0,qy0@0l0@2qx@0,21600l@1,21600qy21600@2l21600@0qx@1,xe";
        Formulas.push_back( L"val #0");
        Formulas.push_back( L"sum width 0 #0");
        Formulas.push_back( L"sum height 0 #0");
        Formulas.push_back( L"prod @0 7071 10000");
        Formulas.push_back( L"sum width 0 @3");
        Formulas.push_back( L"sum height 0 @3");
        Formulas.push_back( L"val width");
        Formulas.push_back( L"val height");
        Formulas.push_back( L"prod width 1 2");
        Formulas.push_back( L"prod height 1 2");

		Adjustments.push_back(3600);

        ConnectorLocations = L"@8,0;0,@9;@8,@7;@6,@9";

        TextBoxRectangle = L"@3,@3,@4,@5";
        Handle HandleOne;
        HandleOne.position = L"#0,topLeft";
        HandleOne.switchHandle = L"true";
        HandleOne.xrange = L"0,10800";
        Handles.push_back( HandleOne );

        Limo = L"10800,10800";
	  }

	  virtual ~PlaqueType()
	  {
	  }
  };
}
