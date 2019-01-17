/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

namespace ASCDocFileFormat
{
  union grfhic
  {
    private:
	  struct
	  {
	    unsigned char fhicChecked:1;
		unsigned char fhicFormat:1;
		unsigned char fhicListText:1;
		unsigned char fhicPeriod:1;
		unsigned char fhicLeft1:1;
		unsigned char fhicListTab:1;
		unsigned char unused:1;
		unsigned char fhicBullet:1;
	  } grfhicStruct;

	  unsigned char grfhicByte;

    public:
	  grfhic():
      grfhicByte(0)
	  {
	  }

	  explicit grfhic( bool _fhicChecked, bool _fhicFormat, bool _fhicListText, bool _fhicPeriod,
		      bool _fhicLeft1, bool _fhicListTab, bool _fhicBullet ):
      grfhicByte(0)
	  {
	    ( _fhicChecked ) ? ( this->grfhicStruct.fhicChecked = 1 ) : ( this->grfhicStruct.fhicChecked = 0 );
		( _fhicFormat ) ? ( this->grfhicStruct.fhicFormat = 1 ) : ( this->grfhicStruct.fhicFormat = 0 );
		( _fhicListText ) ? ( this->grfhicStruct.fhicListText = 1 ) : ( this->grfhicStruct.fhicListText = 0 );
		( _fhicPeriod ) ? ( this->grfhicStruct.fhicPeriod = 1 ) : ( this->grfhicStruct.fhicPeriod = 0 );
		( _fhicLeft1 ) ? ( this->grfhicStruct.fhicLeft1 = 1 ) : ( this->grfhicStruct.fhicLeft1 = 0 );
		( _fhicListTab ) ? ( this->grfhicStruct.fhicListTab = 1 ) : ( this->grfhicStruct.fhicListTab = 0 );
		this->grfhicStruct.unused = 0;
	    ( _fhicBullet ) ? ( this->grfhicStruct.fhicBullet = 1 ) : ( this->grfhicStruct.fhicBullet = 0 );
	  }

	  explicit grfhic( unsigned char _grfhic ):
      grfhicByte(_grfhic)
	  {
	  }

	  operator unsigned char() const
	  {
	    return this->grfhicByte;
	  }
  };
}