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

#include "../Common/Base/FormatUtils.h"

namespace DocFileFormat
{
	struct LSD
	{
		bool fLocked = false;
		bool fSemiHidden = false;
		bool fUnhideWhenUsed = false;
		bool fQFormat = false;
		unsigned short iPriority = 0;
		unsigned short fReserved = 0;
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