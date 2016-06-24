/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
  class CubeType: public ShapeType
  {
    public:
	  CubeType():
      ShapeType(msosptCube)
      {
        this->ShapeConcentricFill = true;

        this->Joins = miter;

        this->Path = _T( "m@0,l0@0,,21600@1,21600,21600@2,21600,xem0@0nfl@1@0,21600,em@1@0nfl@1,21600e" );

        this->Formulas.push_back( _T( "val #0" ) );
        this->Formulas.push_back( _T( "sum width 0 #0" ) );
        this->Formulas.push_back( _T( "sum height 0 #0" ) );
        this->Formulas.push_back( _T( "mid height #0" ) );
        this->Formulas.push_back( _T( "prod @1 1 2" ) );
        this->Formulas.push_back( _T( "prod @2 1 2" ) );
        this->Formulas.push_back( _T( "mid width #0" ) );

        this->AdjustmentValues = _T( "5400" );

        this->ConnectorLocations = _T( "@6,0;@4,@0;0,@3;@4,21600;@1,@3;21600,@5" );

        this->ConnectorAngles = _T( "270,270,180,90,0,0" );

        this->TextBoxRectangle = _T( "0,@0,@1,21600" );

        Handle HandleOne;
        HandleOne.position = _T( "topLeft,#0" );
        HandleOne.switchHandle = _T( "true" );
        HandleOne.yrange = _T( "0,21600" );
        this->Handles.push_back( HandleOne );

        this->Limo = _T( "10800,10800" );
      }

	  virtual ~CubeType()
	  {
	  }
  };
}