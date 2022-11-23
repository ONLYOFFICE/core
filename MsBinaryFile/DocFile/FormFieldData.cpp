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

#include "FormFieldData.h"
#include "../../OOXML/Base/unicode_util.h"

namespace DocFileFormat
{
std::wstring readXstz(VirtualStreamReader *reader)
{
	if (!reader) return L"";

	unsigned short flags, cch, chTerm;

	cch = reader->ReadUInt16();

	std::wstring ret;

	if (cch > 0 && cch < 0x0fff)
	{
		std::shared_ptr<unsigned char>data = std::shared_ptr<unsigned char>(reader->ReadBytes(cch * 2, true));	
#if defined(_WIN32) || defined(_WIN64)
        ret = std::wstring((wchar_t*)data.get(), cch );
#else
        ret = convertUtf16ToWString((UTF16*)data.get(), cch );
#endif
	}
	chTerm  = reader->ReadUInt16();

	return ret;
};
void FormFieldData::_HFD::read(VirtualStreamReader *reader)
{
	if (!reader) return;
}
void FormFieldData::_FFData::read(VirtualStreamReader *reader)
{
	if (!reader) return;

	bExist = true;
	
	unsigned short flags;

	version		= reader->ReadUInt32();
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

	xstzName	= readXstz(reader);
	
	if (iType == 0)
	{
		xstzTextDef = readXstz(reader);
	}
	else if (iType == 1 || iType == 2)
	{
		wDef = reader->ReadUInt16();
	}
	
	xstzTextFormat	= readXstz(reader);
	xstzHelpText	= readXstz(reader);
	xstzStatText	= readXstz(reader);
	xstzEntryMcr	= readXstz(reader);
	xstzExitMcr		= readXstz(reader);

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

	for ( std::list<SinglePropertyModifier>::iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
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

		switch(type)
		{
		case 1:
			HFD.read(&reader);
		case 2:
			FFData.read(&reader);
			break;
		default:
			binary_data_size = lcb - cbHeader;
			binary_data = std::shared_ptr<unsigned char>(reader.ReadBytes(binary_data_size, true));
			break;
		}
	}
}


}
