#pragma once

#include "../Common/FormatUtils.h"

namespace DocFileFormat
{
	/*
	typedef struct _LatentStyleData
	{
		unsigned int grflsd;
		bool fLocked;
	} LatentStyleData;
*/
		struct LSD
		{
			unsigned int fLocked			:	1;
			unsigned int fSemiHidden		:	1;
			unsigned int fUnhideWhenUsed	:	1;
			unsigned int fQFormat			:	1;
			unsigned int iPriority			:	12;
			unsigned int fReserved			:	16;
		};


	class StyleSheetInformation
	{
		friend class StyleSheet;
		friend class StyleSheetMapping;

	private:  
		/// Count of styles in stylesheet
		unsigned short cstd;
		/// Length of STD Base as stored in a file
		unsigned short cbSTDBaseInFile;
		/// Are built-in stylenames stored?
		bool fStdStylenamesWritten;
		/// Max sti known when this file was written
		unsigned short stiMaxWhenSaved;
		/// How many fixed-index istds are there?
		unsigned short istdMaxFixedWhenSaved;
		/// Current version of built-in stylenames
		unsigned short nVerBuiltInNamesWhenSaved;
		/// This is a list of the default fonts for this style sheet.
		/// The first is for ASCII characters (0-127), the second is for East Asian characters, 
		/// and the third is the default font for non-East Asian, non-ASCII text.
		std::vector<unsigned short> rgftcStandardChpStsh;	
		/// Size of each lsd in mpstilsd
		/// The count of lsd's is stiMaxWhenSaved
		unsigned short cbLSD;
		/// latent style data (size == stiMaxWhenSaved upon save!)
		std::vector<LSD> mpstilsd;	

	public:
		/// Parses the bytes to retrieve a StyleSheetInformation
		StyleSheetInformation( unsigned char* bytes, int size );
	};  
}