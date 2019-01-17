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

#include "Shd80.h"

namespace ASCDocFileFormat
{
	class DefTableShd80Operand: public IOperand
	{
	private:
		unsigned char* bytes;
		unsigned int sizeInBytes;

	public:
		DefTableShd80Operand():
		  bytes(0), sizeInBytes(1)
		  {
			  if ( this->sizeInBytes != 0 )
			  {
				  this->bytes = new unsigned char[this->sizeInBytes];

				  if ( this->bytes != NULL )
				  {
					  memset( this->bytes, 0, this->sizeInBytes );
				  }
			  }
		  }

		  explicit DefTableShd80Operand( const vector<Shd80>& _rgShd80 ):
		  bytes(NULL), sizeInBytes(0)
		  {
			  this->sizeInBytes = ( ( _rgShd80.size() * 2 ) + sizeof(unsigned char) );

			  if ( this->sizeInBytes != 0 )
			  {
				  this->bytes = new unsigned char[this->sizeInBytes];

				  if ( this->bytes != NULL )
				  {
					  memset( this->bytes, 0, this->sizeInBytes );

					  this->bytes[0] = ( this->sizeInBytes - sizeof(unsigned char) );

					  unsigned int offset = sizeof(unsigned char);

					  for ( vector<Shd80>::const_iterator iter = _rgShd80.begin(); iter != _rgShd80.end(); iter++ )
					  {
						  unsigned short value = (unsigned short)(*iter);

						  memcpy( ( this->bytes + offset ), &value, sizeof(unsigned short) );
						  offset += sizeof(unsigned short);
					  }
				  }
			  }
		  }

		  DefTableShd80Operand( const DefTableShd80Operand& _defTableShd80Operand ):
		  bytes(NULL), sizeInBytes(_defTableShd80Operand.sizeInBytes)
		  {
			  if ( this->sizeInBytes != 0 )
			  {
				  this->bytes = new unsigned char[this->sizeInBytes];

				  if ( this->bytes != NULL )
				  {
					  memset( this->bytes, 0, this->sizeInBytes );

					  memcpy( this->bytes, _defTableShd80Operand.bytes, this->sizeInBytes );
				  }
			  }
		  }

		  virtual ~DefTableShd80Operand()
		  {
			  RELEASEARRAYOBJECTS (bytes);
		  }

		  virtual operator unsigned char*() const
		  {
			  return this->bytes;
		  }

		  virtual operator const unsigned char*() const
		  {
			  return (const unsigned char*)this->bytes;
		  }

		  virtual unsigned int Size() const
		  {
			  return this->sizeInBytes;
		  }
	};


}