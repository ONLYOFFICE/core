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

#include "IVisitable.h"
#include "CharacterPropertyExceptions.h"
#include "StructuredStorageReader.h"
#include "VirtualStreamReader.h"

namespace DocFileFormat
{
	class FormFieldData : public IVisitable
	{
 	public:

		struct _HFD
		{
			bool bExist = false;

			bool fNew = false;
			bool fNoHist = false;
			bool fImageMap = false;
			bool fLocation = false;
			bool fTooltip = false;

			std::wstring clsid;
			
			_UINT32	streamVersion = 0;
			bool	hlstmfHasMoniker = false;
			bool	hlstmfIsAbsolute = false;
			bool	hlstmfSiteGaveDisplayName = false;
			bool	hlstmfHasLocationStr = false;
			bool	hlstmfHasDisplayName = false;
			bool	hlstmfHasGUID = false;
			bool	hlstmfHasCreationTime = false;
			bool	hlstmfHasFrameName = false;
			bool	hlstmfMonikerSavedAsStr = false;
			bool	hlstmfAbsFromGetdataRel = false;

			std::wstring displayName;
			std::wstring targetFrameName;
			std::wstring moniker;

			std::wstring	oleMoniker_clsid;
			std::wstring	oleMoniker_data;
			std::wstring	location;

			std::wstring	guid;
			_UINT64			fileTime = 0;

			void read(VirtualStreamReader* reader, int size);
		};
		struct _FFData
		{
			bool		bExist = false;
			_UINT32		version = 0;
			
			unsigned char	iType = 0;
			unsigned char	iRes = 0;
			bool			fOwnHelp = false;
			bool			fOwnStat = false;
			bool			fProt = false;
			bool			iSize = false;
			unsigned char	iTypeTxt = 0;
			bool			fRecalc = false;
			bool			fHasListBox = false;
			
			unsigned short	cch_field = 0;
			unsigned short	hps = 0;
			std::wstring	xstzName;
			std::wstring	xstzTextDef;
			unsigned short	wDef = 0;
			std::wstring	xstzTextFormat;
			std::wstring	xstzHelpText;
			std::wstring	xstzStatText;
			std::wstring	xstzEntryMcr;
			std::wstring	xstzExitMcr;

			//STTB			hsttbDropList;
			void read(VirtualStreamReader* reader, int size);
		};
		FormFieldData( int type, const CharacterPropertyExceptions* chpx, POLE::Stream* stream, int nWordVersion );
		virtual ~FormFieldData() {}
	private:
		friend class FormFieldDataMapping;  

		std::wstring name;
		std::wstring rsid;

		_FFData FFData;
		_HFD	HFD;

		std::shared_ptr<unsigned char> binary_data;
		int binary_data_size;

};
}
