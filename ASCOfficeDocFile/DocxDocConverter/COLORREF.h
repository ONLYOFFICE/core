#pragma once

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

		explicit COLORREF( byte _red, byte _green, byte _blue, byte _fAuto = 0xFF ) : red(_red), green(_green), blue(_blue), fAuto(_fAuto)
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

		byte GetRed() const
		{
			return this->red;
		}

		byte GetGreen() const
		{
			return this->green;
		}

		byte GetBlue() const
		{
			return this->blue;
		}

		byte GetAuto() const
		{
			return this->fAuto;
		}

		virtual ~COLORREF()
		{
		}

		virtual operator byte*() const
		{
			return (byte*)this->bytes;
		}

		virtual operator const byte*() const
		{
			return (const byte*)this->bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeof(this->bytes);
		}

	private:

		byte red;
		byte green;
		byte blue;
		byte fAuto;

		static const byte SIZE_IN_BYTES = 4;

		byte bytes[SIZE_IN_BYTES];
	};
}