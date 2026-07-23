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
  class PlusType: public ShapeType
  {
    public: 
	  PlusType():
      ShapeType(msosptPlus)
      {
        this->ShapeConcentricFill = true;

        this->Joins = miter;

        this->Path = L"m@0,l@0@0,0@0,0@2@0@2@0,21600@1,21600@1@2,21600@2,21600@0@1@0@1,xe";

        this->Formulas.push_back( L"val #0" );
        this->Formulas.push_back( L"sum width 0 #0" );
        this->Formulas.push_back( L"sum height 0 #0" );
        this->Formulas.push_back( L"prod @0 2929 10000" );
        this->Formulas.push_back( L"sum width 0 @3" );
        this->Formulas.push_back( L"sum height 0 @3" );
        this->Formulas.push_back( L"val width" );
        this->Formulas.push_back( L"val height" );
        this->Formulas.push_back( L"prod width 1 2" );
        this->Formulas.push_back( L"prod height 1 2" );

		Adjustments.push_back(5400);

        this->ConnectorLocations = L"@8,0;0,@9;@8,@7;@6,@9";

        this->TextBoxRectangle = L"0,0,21600,21600;5400,5400,16200,16200;10800,10800,10800,10800";

        Handle HandleOne;
        HandleOne.position = L"#0,topLeft";
        HandleOne.switchHandle = L"true";
        HandleOne.xrange = L"0,10800";
        this->Handles.push_back( HandleOne );

        this->Limo = L"10800,10800";
	  }

	  virtual ~PlusType()
	  {
	  }
  };
}
