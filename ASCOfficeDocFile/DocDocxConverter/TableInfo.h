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

#include "ParagraphPropertyExceptions.h"

namespace DocFileFormat
{
  class TableInfo
  {
    public:
	  bool fInTable;
      bool fTtp;
      bool fInnerTtp;
      bool fInnerTableCell;
      unsigned int iTap;

      TableInfo( ParagraphPropertyExceptions* papx ):
      fInTable(false), fTtp(false), fInnerTtp(false), fInnerTableCell(false), iTap(0)
      {
	    if ( papx != NULL )
		{
		  for ( list<SinglePropertyModifier>::iterator iter = papx->grpprl->begin(); iter != papx->grpprl->end(); iter++ )
	      {
	        if ( iter->OpCode == sprmPFInTable )
            {
			  this->fInTable = ( iter->Arguments[0] == 1 ) ? (true) : (false);
            }
          
		    if ( iter->OpCode == sprmPFTtp )
            {
              this->fTtp = ( iter->Arguments[0] == 1 ) ? (true) : (false);
            }
          
		    if ( iter->OpCode == sprmPFInnerTableCell )
            {
              this->fInnerTableCell = ( iter->Arguments[0] == 1 ) ? (true) : (false);
            }
          
		    if ( iter->OpCode == sprmPFInnerTtp )
            {
              this->fInnerTtp = ( iter->Arguments[0] == 1 ) ? (true) : (false);
            }
          
		    if ( iter->OpCode == sprmPItap )
            {
		      this->iTap = FormatUtils::BytesToUInt32( iter->Arguments, 0, iter->argumentsSize );
          
			  if ( this->iTap > 0 )
			  {
			    this->fInTable = true;
			  }
            }
          
		    if ( (int)( iter->OpCode ) == 0x66A )
            {
              //add value!
              this->iTap = FormatUtils::BytesToUInt32( iter->Arguments, 0, iter->argumentsSize );

			  if ( this->iTap > 0 )
			  {
                this->fInTable = true;
			  }
            }
          }
		}
      }
  };
}