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

#include "FormFieldData.h"
#include "../../OOXML/Base/unicode_util.h"

namespace DocFileFormat
{
std::wstring readXstz(VirtualStreamReader *reader, bool bWide)
{
	if (!reader) return L"";

	unsigned short flags, cch, chTerm;
	std::wstring ret;

	if (bWide)
	{
		cch = reader->ReadUInt16();

		if (cch > 0 && cch < 0x0fff)
		{
			std::shared_ptr<unsigned char>data = std::shared_ptr<unsigned char>(reader->ReadBytes(cch * 2, true));
#if defined(_WIN32) || defined(_WIN64)
			ret = std::wstring((wchar_t*)data.get(), cch);
#else
			ret = convertUtf16ToWString((UTF16*)data.get(), cch);
#endif
		}
		chTerm = reader->ReadUInt16();
	}
	else
	{
		short cch = reader->ReadByte();

		unsigned char* chars = reader->ReadBytes(cch, true);
		FormatUtils::GetWStringFromBytes(ret, chars, cch, ENCODING_WINDOWS_1250);

		RELEASEARRAYOBJECTS(chars);
	}

	return ret;
};
void FormFieldData::_HFD::read(VirtualStreamReader *reader, int size)
{
	if (!reader) return;
}
void FormFieldData::_FFData::read(VirtualStreamReader *reader, int size)
{
	if (!reader) return;

	bExist = true;
	
	unsigned short flags, version;
	bool bWide = true;

	if (0xff == reader->ReadByte())
	{
		version = 0xFFFFFFFF; //must be 0xFFFFFFFF
		reader->ReadBytes(3, false);
	}
	else
	{
		reader->ReadBytes(1, false);
		version = 0;
		bWide = false;
	}

	flags		= reader->ReadUInt16();

	iType		= GETBITS(flags, 0, 1);
	iRes		= GETBITS(flags, 2, 6);
	fOwnHelp	= GETBIT(flags, 7);
	fOwnStat	= GETBIT(flags, 8);
	fProt		= GETBIT(flags, 9);
	iSize		= GETBIT(flags, 10);
	iTypeTxt	= GETBITS(flags, 11, 13);
	fRecalc		= GETBIT(flags, 14);
	fHasListBox = GETBIT(flags, 15);

	cch_field	= reader->ReadUInt16();
	hps			= reader->ReadUInt16();

	xstzName	= readXstz(reader, bWide);

	if (iType == 0)
	{
		xstzTextDef = readXstz(reader, bWide);
	}
	else if (iType == 1 || iType == 2)
	{
		wDef = reader->ReadUInt16();
	}
	
	xstzTextFormat	= readXstz(reader, bWide);
	xstzHelpText	= readXstz(reader, bWide);
	xstzStatText	= readXstz(reader, bWide);
	xstzEntryMcr	= readXstz(reader, bWide);
	xstzExitMcr		= readXstz(reader, bWide);

	if (iType == 2) 
	{
		//hsttbDropList
	}
}
FormFieldData::FormFieldData( int type, const CharacterPropertyExceptions* chpx, POLE::Stream* stream, int nWordVersion )
{
	binary_data_size = 0;

	if (!chpx) return;

	int fc = -1;
	bool bNilPICFAndBinData = false;

	for ( std::vector<SinglePropertyModifier>::iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
	{
		switch ( iter->OpCode)
		{
			case sprmCFSpec:
			{
			}
			break;
			case sprmCHps:
			{
			}
			break;
			case sprmCHpsBi:
			{
			}
			break;
			case sprmCCv:
			{
			}
			break;
			case sprmCFFldVanish:
			{
			}
			break;
			case sprmCPicLocation:
			{
				fc = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
			}
			break;
			case sprmCRsidProp:
			{
			}break;
			case sprmCFBold:
			{
			}break;
			case sprmCIco:
			{
			}break;
			case sprmCRsidText:
			{
				rsid = FormatUtils::IntToFormattedWideString(FormatUtils::BytesToInt32(iter->Arguments, 0, iter->argumentsSize), L"%08X");
			}
			break;
			case sprmCFData:
			{
				bNilPICFAndBinData = true; // or bPICFAndOfficeArtData - shape, pic, ole
			}
			break;
			default:
				break;
		}
	}

	if (fc >= 0 && bNilPICFAndBinData)
	{
		VirtualStreamReader reader(stream, fc, nWordVersion);

		int sz_stream = reader.GetSize();

		int lcb = reader.ReadUInt32();
		int cbHeader = reader.ReadUInt16();

		if (cbHeader != 0x44) return; 

		//ignored
		reader.ReadBytes(62, false);

		_UINT32 pos = reader.GetPosition();

		switch(type)
		{
			case 1:
			{
				HFD.read(&reader, lcb - cbHeader);
			}break;
			case 2:
			{
				FFData.read(&reader, lcb - cbHeader);
			}break;
			default:
			{
				binary_data_size = lcb - cbHeader;
				binary_data = std::shared_ptr<unsigned char>(reader.ReadBytes(binary_data_size, true));
			}break;
		}
		reader.Seek(pos + (lcb - cbHeader), 0);
	}
}


}
