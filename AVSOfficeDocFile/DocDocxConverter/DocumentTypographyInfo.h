#pragma once

namespace DocFileFormat
{
	class DocumentTypographyInfo
	{
		friend class SettingsMapping;  

	private:
		/// True if we're kerning punctation
		bool fKerningPunct;
		/// Kinsoku method of justification:
		/// 0 = always expand
		/// 1 = compress punctation
		/// 2 = compress punctation and kana
		short iJustification;
		/// Level of kinsoku:
		/// 0 = level 1
		/// 1 = Level 2
		/// 2 = Custom
		short iLevelOfKinsoku;
		/// "2 page on 1" feature is turned on
		bool f2on1;
		/// Old East Asian feature
		bool fOldDefineLineBaseOnGrid;
		/// Custom Kinsoku
		short iCustomKsu;
		/// When set to true, use strict (level 2) Kinsoku rules
		bool fJapaneseUseLevel2;
		/// Length of rgxchFPunct
		short cchFollowingPunct;
		/// Length of rgxchLPunct
		short cchLeadingPunct;
		/// Array of characters that should never appear at the start of a line
		wstring rgxchFPunct;
		/// Array of characters that should never appear at the end of a line
		wstring rgxchLPunct;

	public:
		virtual ~DocumentTypographyInfo()
		{
		}

		DocumentTypographyInfo():
		fKerningPunct(false), iJustification(0), iLevelOfKinsoku(0), f2on1(false), fOldDefineLineBaseOnGrid(false),
			iCustomKsu(0), fJapaneseUseLevel2(false), cchFollowingPunct(0), cchLeadingPunct(0), rgxchFPunct( _T( "" ) ),
			rgxchLPunct( _T( "" ) )
		{
		}

		/// Parses the bytes to retrieve a DocumentTypographyInfo
		DocumentTypographyInfo( byte* bytes, int size ):
		fKerningPunct(false), iJustification(0), iLevelOfKinsoku(0), f2on1(false), fOldDefineLineBaseOnGrid(false),
			iCustomKsu(0), fJapaneseUseLevel2(false), cchFollowingPunct(0), cchLeadingPunct(0), rgxchFPunct( _T( "" ) ),
			rgxchLPunct( _T( "" ) )
		{
			if ( size == 310 )
			{
				//split byte 0 and 1 into bits
				this->fKerningPunct = FormatUtils::GetBitFromBytes( bytes, size, 0 );
				this->iJustification = (short)FormatUtils::GetUIntFromBytesBits( bytes, size, 1, 2 );
				this->iLevelOfKinsoku = (short)FormatUtils::GetUIntFromBytesBits( bytes, size, 3, 2 );
				this->f2on1 = FormatUtils::GetBitFromBytes( bytes, size, 5 );
				this->fOldDefineLineBaseOnGrid = FormatUtils::GetBitFromBytes( bytes, size, 6 );
				this->iCustomKsu = (short)FormatUtils::GetUIntFromBytesBits( bytes, size, 7, 3 );
				this->fJapaneseUseLevel2 = FormatUtils::GetBitFromBytes( bytes, size, 10 );

				this->cchFollowingPunct = FormatUtils::BytesToInt16( bytes, 2, size );
				this->cchLeadingPunct = FormatUtils::BytesToInt16( bytes, 4, size );

				byte fpunctBytes[202];
				memcpy( fpunctBytes, ( bytes + 6 ), 202 );
				FormatUtils::GetSTLCollectionFromBytes<wstring>( &(this->rgxchFPunct), fpunctBytes, 202, ENCODING_UNICODE );

				byte lpunctBytes[102];
				memcpy( lpunctBytes, ( bytes + 208 ), 102 );
				FormatUtils::GetSTLCollectionFromBytes<wstring>( &(this->rgxchLPunct), lpunctBytes, 102, ENCODING_UNICODE );
			}
			else
			{
				//throw new ByteParseException("Cannot parse the struct DOPTYPOGRAPHY, the length of the struct doesn't match");
			}
		}
	};
}