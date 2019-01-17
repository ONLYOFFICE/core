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

#include "Constants.h"

namespace ASCDocFileFormat
{
	class FtsWWidth_Table: public IOperand
	{
	private:
		Constants::Fts ftsWidth;
		unsigned short wWidth;
		static const unsigned char SIZE_IN_BYTES = 3;

		unsigned char bytes[SIZE_IN_BYTES];

	public:
		explicit FtsWWidth_Table( Constants::Fts _ftsWidth, unsigned short _wWidth = 0 )
		{
			if ( _ftsWidth == Constants::ftsDxaSys )
			{
				this->ftsWidth = Constants::ftsNil;  
			}
			else
			{
				this->ftsWidth = _ftsWidth;
			}

			if ( ( this->ftsWidth == Constants::ftsNil ) || ( this->ftsWidth == Constants::ftsAuto ) )
			{
				this->wWidth = 0;
			}
			else if ( this->ftsWidth == Constants::ftsPercent )
			{
				if ( _wWidth > 30000 )
				{
					this->wWidth = 30000;
				}
				else
				{
					this->wWidth = _wWidth;
				}
			}
			else if ( this->ftsWidth == Constants::ftsDxa )
			{
				if ( _wWidth > 31680 )
				{
					this->wWidth = 31680;
				}
				else
				{
					this->wWidth = _wWidth;
				}  
			}

			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		FtsWWidth_Table( const FtsWWidth_Table& ftsWWidth_Table ):
		ftsWidth(ftsWWidth_Table.ftsWidth), wWidth(ftsWWidth_Table.wWidth)
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		virtual ~FtsWWidth_Table()
		{
		}

		virtual unsigned int Size() const
		{
			return SIZE_IN_BYTES;  
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(this->bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)(this->bytes);
		}
	};

	class FtsWWidth_TablePart: public IOperand
	{
	private:
		Constants::Fts ftsWidth;
		unsigned short wWidth;
		static const unsigned char SIZE_IN_BYTES = 3;

		unsigned char bytes[SIZE_IN_BYTES];

	public:
		explicit FtsWWidth_TablePart( Constants::Fts _ftsWidth, unsigned short _wWidth = 0 )
		{
			if ( _ftsWidth == Constants::ftsDxaSys )
			{
				this->ftsWidth = Constants::ftsNil;  
			}
			else if(_ftsWidth == Constants::ftsDxa && 0 == _wWidth)
			{
				this->ftsWidth = Constants::ftsAuto;
			}
			else
			{
				this->ftsWidth = _ftsWidth;
			}

			if ( ( this->ftsWidth == Constants::ftsNil ) || ( this->ftsWidth == Constants::ftsAuto ) )
			{
				this->wWidth = 0;
			}
			else if ( this->ftsWidth == Constants::ftsPercent )
			{
				if ( _wWidth > 5000 )
				{
					this->wWidth = 5000;
				}
				else
				{
					this->wWidth = _wWidth;
				}
			}
			else if ( this->ftsWidth == Constants::ftsDxa )
			{
				if ( _wWidth > 31680 )
				{
					this->wWidth = 31680;
				}
				else
				{
					this->wWidth = _wWidth;
				}  
			}

			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		FtsWWidth_TablePart( const FtsWWidth_TablePart& ftsWWidth_TablePart ):
		ftsWidth(ftsWWidth_TablePart.ftsWidth), wWidth(ftsWWidth_TablePart.wWidth)
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		virtual ~FtsWWidth_TablePart()
		{
		}

		virtual unsigned int Size() const
		{
			return SIZE_IN_BYTES;  
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(this->bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)(this->bytes);
		}
	};

	class FtsWWidth_Indent: public IOperand
	{
	private:
		Constants::Fts ftsWidth;
		short wWidth;
		static const unsigned char SIZE_IN_BYTES = 3;

		unsigned char bytes[SIZE_IN_BYTES];

	public:
		explicit FtsWWidth_Indent( Constants::Fts _ftsWidth, short _wWidth = 0 )
		{
			if ( ( _ftsWidth == Constants::ftsPercent ) || ( _ftsWidth == Constants::ftsDxaSys ) )
			{
				this->ftsWidth = Constants::ftsNil;  
			}
			else
			{
				this->ftsWidth = _ftsWidth;
			}

			if ( ( this->ftsWidth == Constants::ftsNil ) || ( this->ftsWidth == Constants::ftsAuto ) )
			{
				this->wWidth = 0;
			}
			else if ( this->ftsWidth == Constants::ftsDxa )
			{
				if ( _wWidth > 31680 )
				{
					this->wWidth = 31680;
				}
				else if ( _wWidth < -31560 )
				{
					this->wWidth = -31560;
				}
				else
				{
					this->wWidth = _wWidth;
				}
			}

			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		FtsWWidth_Indent( const FtsWWidth_Indent& ftsWWidth_Indent ):
		ftsWidth(ftsWWidth_Indent.ftsWidth), wWidth(ftsWWidth_Indent.wWidth)
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			this->bytes[0] = this->ftsWidth;
			memcpy( ( this->bytes + sizeof(unsigned char) ), &(this->wWidth), sizeof(this->wWidth) );
		}

		virtual ~FtsWWidth_Indent()
		{
		}

		virtual unsigned int Size() const
		{
			return SIZE_IN_BYTES;  
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(this->bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)(this->bytes);
		}
	};
}