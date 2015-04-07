#pragma once

#include "../Common/IOperand.h"

namespace ASCDocFileFormat
{
	class COLORREF : public IOperand
	{
	public:
		static const int cvAuto = 0xFF000000;

		COLORREF() : red(0), green(0), blue(0), fAuto(0xFF)
		{
			this->bytes[0] = this->red;
			this->bytes[1] = this->green;
			this->bytes[2] = this->blue;
			this->bytes[3] = this->fAuto;
		}

		explicit COLORREF( unsigned char _red, unsigned char _green, unsigned char _blue, unsigned char _fAuto = 0xFF ) : red(_red), green(_green), blue(_blue), fAuto(_fAuto)
		{
			this->bytes[0] = this->red;
			this->bytes[1] = this->green;
			this->bytes[2] = this->blue;
			this->bytes[3] = this->fAuto;
		}

		explicit COLORREF( unsigned int _value ) : red(0), green(0), blue(0), fAuto(0)
		{
			this->red = ( _value >> 16 ) & 0x000000FF;
			this->green = ( _value >> 8 ) & 0x000000FF;
			this->blue = _value & 0x000000FF;
			this->fAuto = ( _value >> 24 ) & 0x000000FF;

			this->bytes[0] = this->red;
			this->bytes[1] = this->green;
			this->bytes[2] = this->blue;
			this->bytes[3] = this->fAuto;
		}

		COLORREF( const COLORREF& _colorref ) : red(_colorref.red), green(_colorref.green), blue(_colorref.blue), fAuto(_colorref.fAuto)
		{
			this->bytes[0] = this->red;
			this->bytes[1] = this->green;
			this->bytes[2] = this->blue;
			this->bytes[3] = this->fAuto;
		}

		unsigned char GetRed() const
		{
			return this->red;
		}

		unsigned char GetGreen() const
		{
			return this->green;
		}

		unsigned char GetBlue() const
		{
			return this->blue;
		}

		unsigned char GetAuto() const
		{
			return this->fAuto;
		}

		virtual ~COLORREF()
		{
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)this->bytes;
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)this->bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeof(this->bytes);
		}

	private:

		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char fAuto;

		static const unsigned char SIZE_IN_BYTES = 4;

		unsigned char bytes[SIZE_IN_BYTES];
	};
}