﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
  class IsoscelesTriangleType: public ShapeType
  {
    public: 
	  IsoscelesTriangleType():
      ShapeType(msosptIsocelesTriangle)
      {
	    ShapeConcentricFill = true;
        Joins = miter;

        Path = L"m@0,l,21600r21600,xe" ;

        Formulas.push_back( L"val #0" );
        Formulas.push_back( L"prod #0 1 2" );
        Formulas.push_back( L"sum @1 10800 0");

		Adjustments.push_back(10800);

        ConnectorLocations = L"@0,0;@1,10800;0,21600;10800,21600;21600,21600;@2,10800";

        TextBoxRectangle = L"0,10800,10800,18000;5400,10800,16200,18000;10800,10800,21600,18000;0,7200,7200,21600;7200,7200,14400,21600;14400,7200,21600,21600";

        Handle HandleOne;
        HandleOne.position = L"#0,topLeft";
        HandleOne.xrange = L"0,21600";
        Handles.push_back( HandleOne );
	  }

	  virtual ~IsoscelesTriangleType()
	  {
	  }
  };
}
