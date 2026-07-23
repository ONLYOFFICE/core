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
  class PictureFrameType: public ShapeType
  {
    public: 
	  PictureFrameType() :
      ShapeType(msosptPictureFrame)
      {
		/*this->Path = _T( "m@4@5l@4@11@9@11@9@5xe" );

		this->Formulas.push_back( std::wstring( _T( "if lineDrawn pixelLineWidth 0" ) ) );
        this->Formulas.push_back( std::wstring( _T( "sum @0 1 0" ) ) );
        this->Formulas.push_back( std::wstring( _T( "sum 0 0 @1" ) ) );
        this->Formulas.push_back( std::wstring( _T( "prod @2 1 2" ) ) );
        this->Formulas.push_back( std::wstring( _T( "prod @3 21600 pixelWidth" ) ) );
        this->Formulas.push_back( std::wstring( _T( "prod @3 21600 pixelHeight" ) ) );
        this->Formulas.push_back( std::wstring( _T( "sum @0 0 1" ) ) );
        this->Formulas.push_back( std::wstring( _T( "prod @6 1 2" ) ) );
        this->Formulas.push_back( std::wstring( _T( "prod @7 21600 pixelWidth" ) ) );
        this->Formulas.push_back( std::wstring( _T( "sum @8 21600 0" ) ) );
        this->Formulas.push_back( std::wstring( _T( "prod @7 21600 pixelHeight" ) ) );
        this->Formulas.push_back( std::wstring( _T( "sum @10 21600 0" ) ) );*/

        //pictures are not stroked or fileld by default
        this->Filled = false;
        this->Stroked = false;

  //      //pictures have a lock on the aspect ratio by default
		//this->Lock.fUsefLockAspectRatio = true;
  //      this->Lock.fLockAspectRatio = true;
      }
	  void SetType(unsigned int nType)
	  {
		  this->TypeCode = nType;
		  return;
	  }

	  virtual ~PictureFrameType()
	  {
	  }
  };
}
