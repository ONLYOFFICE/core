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

#include "ParagraphPropertyExceptions.h"

namespace DocFileFormat
{
	ParagraphPropertyExceptions::ParagraphPropertyExceptions( unsigned char* bytes, int size, POLE::Stream* dataStream, bool oldVersion): 
				PropertyExceptions( ( bytes + 2 ), ( size - 2 ),  oldVersion)
    {
      if ( size != 0 )
      {
	    this->istd = FormatUtils::BytesToUInt16( bytes, 0, size );
      }

	  VirtualStreamReader *reader = NULL;

      //There is a SPRM that points to an offset in the data stream, 
      //where a list of SPRM is saved.
      for ( std::list<SinglePropertyModifier>::iterator iter = this->grpprl->begin(); iter != this->grpprl->end(); iter++ )
      {
	    SinglePropertyModifier sprm( *iter );
		  
		if( ( sprm.OpCode == sprmPHugePapx ) || ( (int)sprm.OpCode == 0x6646 ) )
        {
          unsigned int fc = FormatUtils::BytesToUInt32( sprm.Arguments, 0, sprm.argumentsSize );
		  reader = new VirtualStreamReader( dataStream, (int)fc, oldVersion);
		  
          //parse the size of the external grpprl
		  unsigned char* sizebytes = reader->ReadBytes( 2, true );
		  
		  unsigned int grpprlsize = FormatUtils::BytesToUInt16( sizebytes, 0, 2 );
          
		  RELEASEARRAYOBJECTS( sizebytes );

          //parse the external grpprl
		  unsigned char* grpprlBytes = reader->ReadBytes( grpprlsize, true );

		  PropertyExceptions externalPx( grpprlBytes, grpprlsize, oldVersion );

          //assign the external grpprl
          RELEASEOBJECT( this->grpprl );
		  this->grpprl = new std::list<SinglePropertyModifier>( *(externalPx.grpprl) );

          //remove the sprmPHugePapx
		  this->grpprl->remove( sprm );

		  RELEASEARRAYOBJECTS( grpprlBytes );
		  RELEASEOBJECT( reader )

		  break;
		}
      }
    }
}
