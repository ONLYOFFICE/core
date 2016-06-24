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

namespace ASCDocFileFormat
{
  class PlcBtePapx: public IStreamWriter
  {
  private:
    vector<unsigned int> aFC;
	vector<unsigned int> aPnBtePapx;
 
  public:
	PlcBtePapx( const vector<unsigned int>& _aFC, const vector<unsigned int>& _aPnBtePapx ):
    aFC(_aFC), aPnBtePapx(_aPnBtePapx)
	{
	}

	virtual ~PlcBtePapx()
	{
	}

	virtual unsigned char* GetBytes( unsigned long* size ) const
    {
      unsigned char* bytes = NULL;

	  if ( size != NULL )
	  {
		*size = ( ( sizeof(unsigned int) * this->aFC.size() ) + ( sizeof(unsigned int) * this->aPnBtePapx.size() ) );
		bytes = new unsigned char[*size];
		
        if ( bytes != NULL )
		{
	      memset( bytes, 0, *size );

		  unsigned int i = 0;

		  for ( i = 0; i < this->aFC.size(); i++ )
		  {
		    FormatUtils::SetBytes( ( bytes + ( i * sizeof(unsigned int) ) ), (int)this->aFC[i] );
		  }

		  for ( i = 0; i < this->aPnBtePapx.size(); i++ )
		  {
		    FormatUtils::SetBytes( ( bytes + ( sizeof(unsigned int) * this->aFC.size() ) + ( i * sizeof(unsigned int) ) ), (int)this->aPnBtePapx[i] );
		  }
		}
	  }

	  return bytes;
    }
  };
}