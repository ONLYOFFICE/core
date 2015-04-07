#pragma once

namespace OfficeArt
{
	union OfficeArtFOPTEOPID
	{
	private:
		struct
		{
			unsigned short opid:14;
			unsigned short fBid:1;
			unsigned short fComplex:1;
		} OfficeArtFOPTEOPIDStruct;
		unsigned short OfficeArtFOPTEOPIDUnsignedShort;

	public:

		OfficeArtFOPTEOPID() : OfficeArtFOPTEOPIDUnsignedShort(0)
		{
		}

		explicit OfficeArtFOPTEOPID (unsigned short _opid, bool _fBid, bool _fComplex) : OfficeArtFOPTEOPIDUnsignedShort(0)
		{
			this->OfficeArtFOPTEOPIDStruct.opid = _opid;
			( _fBid ) ? ( this->OfficeArtFOPTEOPIDStruct.fBid = 1 ) : ( this->OfficeArtFOPTEOPIDStruct.fBid = 0 );
			( _fComplex ) ? ( this->OfficeArtFOPTEOPIDStruct.fComplex = 1 ) : ( this->OfficeArtFOPTEOPIDStruct.fComplex = 0 );
		}

		inline unsigned short GetOpid() const
		{
			return this->OfficeArtFOPTEOPIDStruct.opid;
		}

		inline bool GetFBid() const
		{
			return ( ( this->OfficeArtFOPTEOPIDStruct.fBid == 1 ) ? ( true ) : ( false ) );
		}

		inline bool GetFComplex() const
		{
			return ( ( this->OfficeArtFOPTEOPIDStruct.fComplex == 1 ) ? ( true ) : ( false ) );
		}

		inline unsigned int GetValue() const
		{
			return this->OfficeArtFOPTEOPIDUnsignedShort;
		}
	};
}