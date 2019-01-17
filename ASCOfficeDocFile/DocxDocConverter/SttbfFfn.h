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
#include "STTB.h"

namespace ASCDocFileFormat
{
	union FFID
	{
	private:
		struct
		{
			unsigned char prq:2;
			unsigned char fTrueType:1;
			unsigned char unused1:1;
			unsigned char ff:3;
			unsigned char unused2:1;
		} FFIDStruct;
		unsigned char FFIDByte;

	public:
		FFID():
		  FFIDByte(0)
		  {
		  }

		  FFID( unsigned char _ffid ):
		  FFIDByte(_ffid)
		  {
		  }

		  explicit FFID( Constants::CharacterPitch _characterPitch, bool _isTrueTypeFont, Constants::FontFamilyType _fontFamilyType ):
		  FFIDByte(0)
		  {
			  this->FFIDStruct.prq = (unsigned short)_characterPitch;
			  ( _isTrueTypeFont ) ? ( this->FFIDStruct.fTrueType = 1 ) : ( this->FFIDStruct.fTrueType = 0 );
			  this->FFIDStruct.ff = (unsigned short)_fontFamilyType;
		  }

		  operator unsigned char() const
		  {
			  return this->FFIDByte;
		  }
	};

	struct PANOSE
	{
		unsigned char bFamilyType;
		unsigned char bSerifStyle;
		unsigned char bWeight;
		unsigned char bProportion;
		unsigned char bContrast;
		unsigned char bStrokeVariation;
		unsigned char bArmStyle;
		unsigned char bLetterform;
		unsigned char bMidline;
		unsigned char bHeight;

		PANOSE():
		bFamilyType(PAN_ANY), bSerifStyle(PAN_ANY), bWeight(PAN_ANY), bProportion(PAN_ANY),
			bContrast(PAN_ANY), bStrokeVariation(PAN_ANY), bArmStyle(PAN_ANY), bLetterform(PAN_ANY),
			bMidline(PAN_ANY), bHeight(PAN_ANY)
		{
		}

		explicit PANOSE( unsigned char _bFamilyType, unsigned char _bSerifStyle, unsigned char _bWeight, unsigned char _bProportion, unsigned char _bContrast,
			unsigned char _bStrokeVariation, unsigned char _bArmStyle, unsigned char _bLetterform, unsigned char _bMidline, unsigned char _bHeight ):
		bFamilyType(_bFamilyType), bSerifStyle(_bSerifStyle), bWeight(_bWeight), bProportion(_bProportion),
			bContrast(_bContrast), bStrokeVariation(_bStrokeVariation), bArmStyle(_bArmStyle), bLetterform(_bLetterform),
			bMidline(_bMidline), bHeight(_bHeight)
		{
		}
	};

	class FFN: public IOperand
	{
	private:
		FFID ffid;
		short wWeight;
		unsigned char chs;
		unsigned char ixchSzAlt;
		PANOSE panose;
		FONTSIGNATURE fs;
		wstring xszFfn;
		wstring xszAlt;

		unsigned char* bytes;
		unsigned int sizeInBytes;

		void SetBytes()
		{
			this->sizeInBytes = ( sizeof(this->ffid) + sizeof(this->wWeight) + sizeof(this->chs) + 
				sizeof(this->ixchSzAlt) + sizeof(this->panose) + sizeof(this->fs) + 
				( sizeof(wchar_t) * ( this->xszFfn.size() + 1 ) ) );

			if ( this->ixchSzAlt != 0 )
			{
				this->sizeInBytes += ( sizeof(wchar_t) * ( this->xszAlt.size() + 1 ) );
			}

			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				this->bytes[0] = (unsigned char)this->ffid;
				FormatUtils::SetBytes( ( this->bytes + sizeof(this->ffid) ), this->wWeight );
				this->bytes[3] = this->chs;
				this->bytes[4] = this->ixchSzAlt;
				this->bytes[5] = this->panose.bFamilyType;
				this->bytes[6] = this->panose.bSerifStyle;
				this->bytes[7] = this->panose.bWeight;
				this->bytes[8] = this->panose.bProportion;
				this->bytes[9] = this->panose.bContrast;
				this->bytes[10] = this->panose.bStrokeVariation;
				this->bytes[11] = this->panose.bArmStyle;
				this->bytes[12] = this->panose.bLetterform;
				this->bytes[13] = this->panose.bMidline;
				this->bytes[14] = this->panose.bHeight;

				memcpy( ( this->bytes + sizeof(this->ffid) + sizeof(this->wWeight) + sizeof(this->chs) +
					sizeof(this->ixchSzAlt) + sizeof(this->panose) ), &(this->fs), sizeof(this->fs) );

				memcpy( ( this->bytes + sizeof(this->ffid) + sizeof(this->wWeight) + sizeof(this->chs) +
					sizeof(this->ixchSzAlt) + sizeof(this->panose) + sizeof(this->fs) ), this->xszFfn.c_str(), ( sizeof(wchar_t) * ( this->xszFfn.size() + 1 ) ) );

				if ( this->ixchSzAlt != 0 )
				{
					memcpy( ( this->bytes + sizeof(this->ffid) + sizeof(this->wWeight) + sizeof(this->chs) +
						sizeof(this->ixchSzAlt) + sizeof(this->panose) + sizeof(this->fs) + ( sizeof(wchar_t) * ( this->xszFfn.size() + 1 ) ) ), this->xszAlt.c_str(), ( sizeof(wchar_t) * ( this->xszAlt.size() + 1 ) ) );
				}
			}
		}

	public:

		FFN() :  ffid(), wWeight(0), chs(ANSI_CHARSET), ixchSzAlt(0), panose(), bytes(NULL), sizeInBytes(0)
		{
			memset( &(this->fs), 0, sizeof(this->fs) );

			this->SetBytes();
		}

		explicit FFN( const FFID& _ffid, short _wWeight, unsigned char _chs, const PANOSE& _panose, const FONTSIGNATURE& _fs, const wstring& _xszFfn, const wstring& _xszAlt = wstring( _T( "" ) ) ) : ffid(_ffid), chs(_chs), panose(_panose), xszFfn(_xszFfn), xszAlt(_xszAlt), bytes(NULL), sizeInBytes(0)
		{
			memcpy( &(this->fs), &_fs, sizeof(FONTSIGNATURE) );

			if ( _wWeight < 0 )
			{
				this->wWeight = 0;
			}
			else if ( _wWeight > 1000 )
			{
				this->wWeight = 1000;
			}
			else
			{
				this->wWeight = _wWeight;
			}

			if ( !_xszAlt.empty() )
			{
				this->ixchSzAlt = ( this->xszFfn.size() + 1 );
			}
			else
			{
				this->ixchSzAlt = 0;
			}

			this->SetBytes();
		}

		FFN( const FFN& _ffn ) : ffid(_ffn.ffid), wWeight(_ffn.wWeight), chs(_ffn.chs), ixchSzAlt(_ffn.ixchSzAlt), panose(_ffn.panose), xszFfn(_ffn.xszFfn), xszAlt(_ffn.xszAlt), bytes(NULL), sizeInBytes(_ffn.sizeInBytes)
		{
			memcpy( &(this->fs), &(_ffn.fs), sizeof(FONTSIGNATURE) );

			this->SetBytes();
		}

		virtual ~FFN()
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

	typedef STTB<FFN> SttbfFfn;
}