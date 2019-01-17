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

#include "LPUpxPapx.h"
#include "LPUpxChpx.h"
#include "LPUpxTapx.h"
#include "Constants.h"

namespace ASCDocFileFormat
{
	//!!!TODO: The revision-marking information and formatting for the style.!!!
	class StkParaGRLPUPX: public IOperand
	{
	private:
		LPUpxPapx lPUpxPapx;
		LPUpxChpx lPUpxChpx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;

	public:

		StkParaGRLPUPX () : lPUpxPapx(), lPUpxChpx(), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
		}

		explicit StkParaGRLPUPX( const LPUpxPapx& _lPUpxPapx, const LPUpxChpx& _lPUpxChpx ) : lPUpxPapx(_lPUpxPapx), lPUpxChpx(_lPUpxChpx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			this->sizeInBytes = ( _lPUpxPapx.Size() + _lPUpxChpx.Size() );
			this->sizeInBytesWithoutPadding = ( _lPUpxPapx.SizeWithoutPadding() + _lPUpxChpx.SizeWithoutPadding() );

			if ( this->sizeInBytes > 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, (unsigned char*)_lPUpxPapx, _lPUpxPapx.Size() );
					memcpy( ( this->bytes + _lPUpxPapx.Size() ), (unsigned char*)_lPUpxChpx, _lPUpxChpx.Size() );
				}
			}
		}

		StkParaGRLPUPX( const StkParaGRLPUPX& _stkParaGRLPUPX ) : lPUpxPapx(_stkParaGRLPUPX.lPUpxPapx), lPUpxChpx(_stkParaGRLPUPX.lPUpxChpx), bytes(NULL), sizeInBytes(_stkParaGRLPUPX.sizeInBytes), sizeInBytesWithoutPadding(_stkParaGRLPUPX.sizeInBytesWithoutPadding)
		{
			if ( this->sizeInBytes > 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, _stkParaGRLPUPX.bytes, this->sizeInBytes );
				}
			}
		}

		virtual ~StkParaGRLPUPX()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		const vector<Prl> GetProperties() const
		{
			vector<Prl> chpxProperties = this->lPUpxChpx.GetProperties();
			vector<Prl> papxProperties = this->lPUpxPapx.GetProperties();

			vector<Prl> properties;

			for ( vector<Prl>::const_iterator iter = chpxProperties.begin(); iter != chpxProperties.end(); iter++ )
			{
				properties.push_back( *iter );  
			}

			for ( vector<Prl>::const_iterator iter = papxProperties.begin(); iter != papxProperties.end(); iter++ )
			{
				properties.push_back( *iter );  
			}

			return properties;
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

		unsigned int SizeWithoutPadding() const
		{
			return this->sizeInBytesWithoutPadding;
		}
	};

	//!!!TODO: The revision-marking information and formatting for the style.!!!
	class StkCharGRLPUPX: public IOperand
	{
	private:
		LPUpxChpx lPUpxChpx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;

	public:

		StkCharGRLPUPX() : lPUpxChpx(), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
		}

		explicit StkCharGRLPUPX( const LPUpxChpx& _lPUpxChpx ) : lPUpxChpx(_lPUpxChpx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			this->sizeInBytes = _lPUpxChpx.Size();
			this->sizeInBytesWithoutPadding = _lPUpxChpx.SizeWithoutPadding();

			if ( this->sizeInBytes > 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, (unsigned char*)_lPUpxChpx, _lPUpxChpx.Size() );
				}
			}
		}

		StkCharGRLPUPX( const StkCharGRLPUPX& _stkCharGRLPUPX ) : lPUpxChpx(_stkCharGRLPUPX.lPUpxChpx), bytes(NULL), sizeInBytes(_stkCharGRLPUPX.sizeInBytes), sizeInBytesWithoutPadding(_stkCharGRLPUPX.sizeInBytesWithoutPadding)
		{
			if ( this->sizeInBytes > 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, _stkCharGRLPUPX.bytes, this->sizeInBytes );
				}
			}
		}

		virtual ~StkCharGRLPUPX()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		const vector<Prl> GetProperties() const
		{
			return this->lPUpxChpx.GetProperties();
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

		unsigned int SizeWithoutPadding() const
		{
			return this->sizeInBytesWithoutPadding;
		}
	};

	class StkTableGRLPUPX: public IOperand
	{
	private:
		LPUpxTapx lPUpxTapx;
		LPUpxPapx lPUpxPapx;
		LPUpxChpx lPUpxChpx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;

	public:
		StkTableGRLPUPX():
		  lPUpxTapx(), lPUpxPapx(), lPUpxChpx(), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		  {
		  }

		  explicit StkTableGRLPUPX( const LPUpxTapx& _lPUpxTapx, const LPUpxPapx& _lPUpxPapx, const LPUpxChpx& _lPUpxChpx ):
		  lPUpxTapx(_lPUpxTapx), lPUpxPapx(_lPUpxPapx), lPUpxChpx(_lPUpxChpx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		  {
			  this->sizeInBytes = ( _lPUpxTapx.Size() + _lPUpxPapx.Size() + _lPUpxChpx.Size() );
			  this->sizeInBytesWithoutPadding = ( _lPUpxTapx.SizeWithoutPadding() + _lPUpxPapx.SizeWithoutPadding() + _lPUpxChpx.SizeWithoutPadding() );

			  if ( this->sizeInBytes > 0 )
			  {
				  this->bytes = new unsigned char[this->sizeInBytes];

				  if ( this->bytes != NULL )
				  {
					  memset( this->bytes, 0, this->sizeInBytes );

					  memcpy( this->bytes, (unsigned char*)_lPUpxTapx, _lPUpxTapx.Size() );
					  memcpy( ( this->bytes + _lPUpxTapx.Size() ), (unsigned char*)_lPUpxPapx, _lPUpxPapx.Size() );
					  memcpy( ( this->bytes + _lPUpxTapx.Size() + _lPUpxPapx.Size() ), (unsigned char*)_lPUpxChpx, _lPUpxChpx.Size() );
				  }
			  }
		  }

		  StkTableGRLPUPX( const StkTableGRLPUPX& _stkTableGRLPUPX ):
		  lPUpxTapx(_stkTableGRLPUPX.lPUpxTapx), lPUpxPapx(_stkTableGRLPUPX.lPUpxPapx), lPUpxChpx(_stkTableGRLPUPX.lPUpxChpx), bytes(NULL), sizeInBytes(_stkTableGRLPUPX.sizeInBytes), sizeInBytesWithoutPadding(_stkTableGRLPUPX.sizeInBytesWithoutPadding)
		  {
			  if ( this->sizeInBytes > 0 )
			  {
				  this->bytes = new unsigned char[this->sizeInBytes];

				  if ( this->bytes != NULL )
				  {
					  memset( this->bytes, 0, this->sizeInBytes );

					  memcpy( this->bytes, _stkTableGRLPUPX.bytes, this->sizeInBytes );
				  }
			  }
		  }

		  virtual ~StkTableGRLPUPX()
		  {
			  RELEASEARRAYOBJECTS (bytes);
		  }

		  const vector<Prl> GetProperties() const
		  {
			  vector<Prl> chpxProperties = this->lPUpxChpx.GetProperties();
			  vector<Prl> papxProperties = this->lPUpxPapx.GetProperties();
			  vector<Prl> tapxProperties = this->lPUpxTapx.GetProperties();

			  vector<Prl> properties;

			  for ( vector<Prl>::const_iterator iter = chpxProperties.begin(); iter != chpxProperties.end(); iter++ )
			  {
				  properties.push_back( *iter );  
			  }

			  for ( vector<Prl>::const_iterator iter = papxProperties.begin(); iter != papxProperties.end(); iter++ )
			  {
				  properties.push_back( *iter );  
			  }

			  for ( vector<Prl>::const_iterator iter = tapxProperties.begin(); iter != tapxProperties.end(); iter++ )
			  {
				  properties.push_back( *iter );  
			  }

			  return properties;
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

		  unsigned int SizeWithoutPadding() const
		  {
			  return this->sizeInBytesWithoutPadding;
		  }
	};

	class StkListGRLPUPX: public IOperand
	{
	private:
		LPUpxPapx lPUpxPapx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;

	public:
		StkListGRLPUPX():
		  lPUpxPapx(), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		  {
		  }

		  explicit StkListGRLPUPX( const LPUpxPapx& _lPUpxPapx ):
		  lPUpxPapx(_lPUpxPapx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		  {
			  this->sizeInBytes = _lPUpxPapx.Size();
			  this->sizeInBytesWithoutPadding = _lPUpxPapx.SizeWithoutPadding();

			  if ( this->sizeInBytes > 0 )
			  {
				  this->bytes = new unsigned char[this->sizeInBytes];

				  if ( this->bytes != NULL )
				  {
					  memset( this->bytes, 0, this->sizeInBytes );

					  memcpy( this->bytes, (unsigned char*)_lPUpxPapx, _lPUpxPapx.Size() );
				  }
			  }
		  }

		  StkListGRLPUPX( const StkListGRLPUPX& _stkListGRLPUPX ):
		  lPUpxPapx(_stkListGRLPUPX.lPUpxPapx), bytes(NULL), sizeInBytes(_stkListGRLPUPX.sizeInBytes), sizeInBytesWithoutPadding(_stkListGRLPUPX.sizeInBytesWithoutPadding)
		  {
			  if ( this->sizeInBytes > 0 )
			  {
				  this->bytes = new unsigned char[this->sizeInBytes];

				  if ( this->bytes != NULL )
				  {
					  memset( this->bytes, 0, this->sizeInBytes );

					  memcpy( this->bytes, _stkListGRLPUPX.bytes, this->sizeInBytes );
				  }
			  }
		  }

		  virtual ~StkListGRLPUPX()
		  {
			  RELEASEARRAYOBJECTS (bytes);
		  }

		  const vector<Prl> GetProperties() const
		  {
			  return this->lPUpxPapx.GetProperties();
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

		  unsigned int SizeWithoutPadding() const
		  {
			  return this->sizeInBytesWithoutPadding;
		  }
	};

	class GrLPUpxSw: public IOperand
	{
	public:
		GrLPUpxSw() :  bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
		}

		explicit GrLPUpxSw( const StkParaGRLPUPX& _stkParaGRLPUPX ) : bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			this->sizeInBytes = _stkParaGRLPUPX.Size();
			this->sizeInBytesWithoutPadding = _stkParaGRLPUPX.SizeWithoutPadding();
			this->properties = _stkParaGRLPUPX.GetProperties();

			if ( this->sizeInBytes > 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, (unsigned char*)_stkParaGRLPUPX, _stkParaGRLPUPX.Size() );
				}
			}
		}

		explicit GrLPUpxSw( const StkCharGRLPUPX& _stkCharGRLPUPX ) : bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			this->sizeInBytes = _stkCharGRLPUPX.Size();
			this->sizeInBytesWithoutPadding = _stkCharGRLPUPX.SizeWithoutPadding();
			this->properties = _stkCharGRLPUPX.GetProperties();

			if ( this->sizeInBytes > 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, (unsigned char*)_stkCharGRLPUPX, _stkCharGRLPUPX.Size() );
				}
			}
		}

		explicit GrLPUpxSw( const StkTableGRLPUPX& _stkTableGRLPUPX ) : bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			this->sizeInBytes = _stkTableGRLPUPX.Size();
			this->sizeInBytesWithoutPadding = _stkTableGRLPUPX.SizeWithoutPadding();
			this->properties = _stkTableGRLPUPX.GetProperties();

			if ( this->sizeInBytes > 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, (unsigned char*)_stkTableGRLPUPX, _stkTableGRLPUPX.Size() );
				}
			}
		}

		explicit GrLPUpxSw( const StkListGRLPUPX& _stkListGRLPUPX ) : bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			this->sizeInBytes = _stkListGRLPUPX.Size();
			this->sizeInBytesWithoutPadding = _stkListGRLPUPX.SizeWithoutPadding();
			this->properties = _stkListGRLPUPX.GetProperties();

			if ( this->sizeInBytes > 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, (unsigned char*)_stkListGRLPUPX, _stkListGRLPUPX.Size() );
				}
			}
		}

		explicit GrLPUpxSw( const Constants::StyleType _styleType, const ASCDocFileFormat::LPUpxPapx& _lPUpxPapx, const ASCDocFileFormat::LPUpxChpx& _lPUpxChpx, const ASCDocFileFormat::LPUpxTapx& _lPUpxTapx ) : bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			switch ( _styleType )
			{
			case Constants::styleTypeParagraph:
				{
					*this = GrLPUpxSw( ASCDocFileFormat::StkParaGRLPUPX( _lPUpxPapx, _lPUpxChpx ) );
				}
				break;

			case Constants::styleTypeCharacter:
				{
					*this = GrLPUpxSw( ASCDocFileFormat::StkCharGRLPUPX( _lPUpxChpx ) );
				}
				break;

			case Constants::styleTypeTable:
				{
					*this = GrLPUpxSw( ASCDocFileFormat::StkTableGRLPUPX( _lPUpxTapx, _lPUpxPapx, _lPUpxChpx ) );
				}
				break;

			case Constants::styleTypeNumbering:
				{
					*this = GrLPUpxSw( ASCDocFileFormat::StkListGRLPUPX( _lPUpxPapx ) );
				}
				break;
			}
		}

		GrLPUpxSw( const GrLPUpxSw& _grLPUpxSw ) : properties(_grLPUpxSw.properties), bytes(NULL), sizeInBytes(_grLPUpxSw.sizeInBytes), sizeInBytesWithoutPadding(_grLPUpxSw.sizeInBytesWithoutPadding)
		{
			if ( this->sizeInBytes > 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, _grLPUpxSw.bytes, this->sizeInBytes );
				}
			}
		}

		bool operator == ( const GrLPUpxSw& _grLPUpxSw )
		{
			return ( ( this->sizeInBytes == _grLPUpxSw.sizeInBytes ) && 
				( this->sizeInBytesWithoutPadding == _grLPUpxSw.sizeInBytesWithoutPadding ) &&
				( memcmp( this->bytes, _grLPUpxSw.bytes, this->sizeInBytes ) == 0 ) );
		}

		bool operator != ( const GrLPUpxSw& _grLPUpxSw )
		{
			return !( this->operator == ( _grLPUpxSw ) );
		}

		GrLPUpxSw& operator = ( const GrLPUpxSw& _grLPUpxSw )
		{
			if ( *this != _grLPUpxSw )
			{
				RELEASEARRAYOBJECTS (bytes);

				this->sizeInBytes = _grLPUpxSw.sizeInBytes;
				this->sizeInBytesWithoutPadding = _grLPUpxSw.sizeInBytesWithoutPadding;
				this->properties = _grLPUpxSw.properties;

				if ( this->sizeInBytes > 0 )
				{
					this->bytes = new unsigned char[this->sizeInBytes];

					if ( this->bytes != NULL )
					{
						memset( this->bytes, 0, this->sizeInBytes );

						memcpy( this->bytes, _grLPUpxSw.bytes, this->sizeInBytes );
					}
				}
			}

			return *this;
		}

		virtual ~GrLPUpxSw()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		const vector<Prl> GetProperties() const
		{
			return this->properties;
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

		unsigned int SizeWithoutPadding() const
		{
			return this->sizeInBytesWithoutPadding;
		}
	private:

		vector<Prl> properties;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;
	};
}