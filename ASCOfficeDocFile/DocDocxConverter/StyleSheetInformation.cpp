
#include "StyleSheetInformation.h"

namespace DocFileFormat
{
	/// Parses the bytes to retrieve a StyleSheetInformation
	StyleSheetInformation::StyleSheetInformation (unsigned char* bytes, int size)
	{
		if ((0 != size) || (NULL != bytes))
		{
			cstd								=	FormatUtils::BytesToUInt16 ( bytes, 0, size );
			cbSTDBaseInFile						=	FormatUtils::BytesToUInt16 ( bytes, 2, size );

			if ( bytes[4] == 1 )
			{
				fStdStylenamesWritten			=	true;
			}

			//unsigned char 5 is spare
			this->stiMaxWhenSaved				=	FormatUtils::BytesToUInt16 ( bytes, 6, size );
			this->istdMaxFixedWhenSaved			=	FormatUtils::BytesToUInt16 ( bytes, 8, size );
			this->nVerBuiltInNamesWhenSaved		=	FormatUtils::BytesToUInt16 ( bytes, 10, size );

			this->rgftcStandardChpStsh			=	std::vector<unsigned short>( 4 );

			this->rgftcStandardChpStsh[0]		=	FormatUtils::BytesToUInt16 ( bytes, 12, size );
			this->rgftcStandardChpStsh[1]		=	FormatUtils::BytesToUInt16 ( bytes, 14, size );
			this->rgftcStandardChpStsh[2]		=	FormatUtils::BytesToUInt16 ( bytes, 16, size );

			if ( size > 18 )
				rgftcStandardChpStsh[3]			=	FormatUtils::BytesToUInt16 ( bytes, 18, size );


			// not all stylesheet contain latent styles
			if ( size > 20 )
			{
				cbLSD							=	FormatUtils::BytesToUInt16 ( bytes, 20, size );

				for ( int i = 0; i < stiMaxWhenSaved; ++i)
				{
					unsigned int iLSD			=	FormatUtils::BytesToUInt32 ( bytes, ( 22 + (i * cbLSD) ), size );
					LSD lsd						=	*(LSD*)&iLSD;

					mpstilsd.push_back( lsd );
				}
			}
		}
	}
}