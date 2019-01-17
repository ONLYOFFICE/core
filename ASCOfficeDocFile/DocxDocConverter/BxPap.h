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
  class BxPap: public IStreamWriter
  {
  public:
    static const unsigned char RESERVED_SIZE = 12;  

  private:
	unsigned char bOffset; //An unsigned integer that specifies the offset of a PapxInFkp in a PapxFkp. The offset of the PapxInFkp is bOffset*2.
	unsigned char reserved[RESERVED_SIZE];

  public:
  	BxPap():
    bOffset(0)
	{
	  memset( this->reserved, 0, RESERVED_SIZE );
	}

	BxPap( unsigned char _bOffset, const unsigned char* _reserved = NULL ):
    bOffset(_bOffset)
	{
	  if ( _reserved != NULL )
	  {
	    memcpy( this->reserved, _reserved, RESERVED_SIZE ); 
	  }
	  else
	  {
	    memset( this->reserved, 0, RESERVED_SIZE );
	  }
	}

	BxPap( const BxPap& bxp ):
	bOffset(bxp.bOffset)
    {
	  memcpy( this->reserved, bxp.reserved, RESERVED_SIZE );  
	}

	virtual ~BxPap()
	{
	}

	bool operator == ( const BxPap& bxp )
	{
	  return ( ( this->bOffset == bxp.bOffset ) && ( memcmp( this->reserved, bxp.reserved, RESERVED_SIZE ) == 0 ) );
	}

	bool operator != ( const BxPap& bxp )
	{
	  return !( *this == bxp );
	}

	BxPap& operator = ( const BxPap& bxp )
	{
	  if ( *this != bxp )
	  {
	    this->bOffset = bxp.bOffset;
	    memcpy( this->reserved, bxp.reserved, RESERVED_SIZE );
	  }

	  return *this;
	}

	virtual unsigned char* GetBytes( unsigned long* size ) const
    {
      unsigned char* bytes = NULL;

	  if ( size != NULL )
	  {
		*size = sizeof(this->bOffset) + RESERVED_SIZE;
		bytes = new unsigned char[*size];
		
        if ( bytes != NULL )
		{
	      memset( bytes, 0, *size );
		  bytes[0] = this->bOffset;

          memcpy( ( bytes + sizeof(this->bOffset) ), this->reserved, RESERVED_SIZE );
		}
	  }

	  return bytes;
    }

	unsigned long Size() const
	{
	  return ( sizeof(this->bOffset) + RESERVED_SIZE );
	}

	unsigned char GetOffset() const
	{
	  return this->bOffset;
	}
  };
}