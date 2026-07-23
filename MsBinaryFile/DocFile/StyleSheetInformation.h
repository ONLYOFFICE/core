/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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