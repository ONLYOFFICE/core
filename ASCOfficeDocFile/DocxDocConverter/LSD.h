#pragma once

namespace AVSDocFileFormat
{
	static const unsigned int LatentStylesTemplate [267] =
	{
		8, 152, 158, 158, 158, 158, 158, 158, 158, 158, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 630,
		630, 630, 630, 630, 630, 630, 630, 630, 1590, 1590, 1590, 1590, 1590, 1590, 574, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 168, 1590, 1590, 22, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 184,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 360, 328, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 944, 1590, 1586, 24, 960, 976, 992, 1008, 1024, 1040, 1056, 1072,
		1088, 1104, 1120, 1136, 1152, 1168, 960, 976, 992, 1008, 1024, 1040, 1586, 552, 472, 488, 1056, 1072, 1088,
		1104, 1120, 1136, 1152, 1168, 960, 976, 992, 1008, 1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152,
		1168, 960, 976, 992, 1008, 1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 960, 976, 992, 1008,
		1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 960, 976, 992, 1008, 1024, 1040, 1056, 1072,
		1088, 1104, 1120, 1136, 1152, 1168, 960, 976, 992, 1008, 1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136,
		1152, 1168, 312, 344, 504, 520, 536, 598, 638
	};

	union LSD
	{
	private:
		struct
		{
			unsigned int fLocked			:	1;
			unsigned int fSemiHidden		:	1;
			unsigned int fUnhideWhenUsed	:	1;
			unsigned int fQFormat			:	1;
			unsigned int iPriority			:	12;
			unsigned int fReserved			:	16;
		} LSDStruct;
		unsigned int LSDUnsignedInt;

	public:

		LSD () : LSDUnsignedInt(0)
		{

		}

		LSD (unsigned int _lsd): LSDUnsignedInt(_lsd)
		{

		}

		LSD (bool _fLocked, bool _fSemiHidden, bool _fUnhideWhenUsed, bool _fQFormat, unsigned short _iPriority) : LSDUnsignedInt(0)
		{
			( _fLocked ) ? ( LSDStruct.fLocked = 1 ) : ( LSDStruct.fLocked = 0 );
			( _fSemiHidden ) ? ( LSDStruct.fSemiHidden = 1 ) : ( LSDStruct.fSemiHidden = 0 );
			( _fUnhideWhenUsed ) ? ( LSDStruct.fUnhideWhenUsed = 1 ) : ( LSDStruct.fUnhideWhenUsed = 0 );
			( _fQFormat ) ? ( LSDStruct.fQFormat = 1 ) : ( LSDStruct.fQFormat = 0 );

			if ( _iPriority > 0x0063 )
			{
				LSDStruct.iPriority = 0x0063;  
			}
			else
			{
				LSDStruct.iPriority = _iPriority;
			}
		}

		operator unsigned int() const
		{
			return LSDUnsignedInt;
		}
	};
}