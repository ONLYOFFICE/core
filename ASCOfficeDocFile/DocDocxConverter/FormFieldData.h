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

			void read(VirtualStreamReader* reader);
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
			void read(VirtualStreamReader* reader);
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
