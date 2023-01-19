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

#include "CharacterPropertyExceptions.h"

namespace DocFileFormat
{	
	CharacterPropertyExceptions::CharacterPropertyExceptions(): PropertyExceptions()
	{
	}

	/// Parses the bytes to retrieve a CHPX
	CharacterPropertyExceptions::CharacterPropertyExceptions( unsigned char* bytes, int size, int nWordVersion) :
													PropertyExceptions( bytes, size, nWordVersion )
	{
		if (nWordVersion > 1)
		{
			RELEASEOBJECT( grpprl );
			grpprl = new std::list<SinglePropertyModifier>();

			MemoryStream oStream(bytes, size);
			int pos = 0;

			if (nWordVersion == 3)
			{
				if (pos + 2 > size) return;
				unsigned short  fChar	= oStream.ReadUInt16(); pos += 2;

				unsigned char val;
				val = GETBIT(fChar, 0); grpprl->push_back(SinglePropertyModifier(sprmOldCFBold,		1, &val));
				val = GETBIT(fChar, 1); grpprl->push_back(SinglePropertyModifier(sprmOldCFItalic,	1, &val));
				val = GETBIT(fChar, 2); grpprl->push_back(SinglePropertyModifier(sprmOldCFStrike,	1, &val));
				val = GETBIT(fChar, 3); grpprl->push_back(SinglePropertyModifier(sprmOldCFOutline,	1, &val));
				val = GETBIT(fChar, 4); grpprl->push_back(SinglePropertyModifier(sprmOldCFFldVanish, 1, &val));
				val = GETBIT(fChar, 5); grpprl->push_back(SinglePropertyModifier(sprmOldCFSmallCaps, 1, &val));
				val = GETBIT(fChar, 6); grpprl->push_back(SinglePropertyModifier(sprmOldCFCaps,		1, &val));
				val = GETBIT(fChar, 7); grpprl->push_back(SinglePropertyModifier(sprmOldCFVanish,	1, &val));
				val = GETBIT(fChar, 8); grpprl->push_back(SinglePropertyModifier(sprmOldCFRMark,	1, &val));
				val = GETBIT(fChar, 9); grpprl->push_back(SinglePropertyModifier(sprmOldCFSpec,		1, &val));

				bool fsIco		=  GETBIT(fChar, 10);
				bool fsFtc		=  GETBIT(fChar, 11);
				bool fsHps		=  GETBIT(fChar, 12);
				bool fsKul		=  GETBIT(fChar, 13);
				bool fsPos		=  GETBIT(fChar, 14);
				bool fsSpace	=  GETBIT(fChar, 15);

				if (pos + 2 > size) return;
				int fff = oStream.ReadUInt16();	pos += 2;//?????

				if (pos + 2 > size) return;
				unsigned short	ftc		= oStream.ReadUInt16();	pos += 2;		// Font Code
				grpprl->push_back(SinglePropertyModifier(sprmOldCFtc, 2, (unsigned char*)&ftc));

				if (pos + 1 > size) return;
				unsigned char	hps		= oStream.ReadByte();	pos += 1;		// Font size in half points

				if (hps > 0)
				{
					grpprl->push_back(SinglePropertyModifier(sprmOldCHps, 1, &hps));
				}

				if (pos + 1 > size) return;
				unsigned char   hpsPos	= oStream.ReadByte();	pos += 1;		// Sub/Superscript ( signed number, 0 = normal )
				grpprl->push_back(SinglePropertyModifier(sprmOldCHpsPos, 1, &hpsPos));

				if (pos + 2 > size) return;
				unsigned short  fText	= oStream.ReadUInt16();	pos += 2;

				unsigned short qpsSpace = GETBITS(fText, 0, 5);
				unsigned char wSpare2 = GETBITS(fText, 6, 7);
				unsigned char ico = GETBITS(fText, 8, 11);
				unsigned char kul = GETBITS(fText, 12, 14);
				bool fSysVanish = GETBIT(fChar, 15);

				grpprl->push_back(SinglePropertyModifier(sprmOldCKul,	1, &kul));
				grpprl->push_back(SinglePropertyModifier(sprmOldCIco,	1, &ico));

				//sizeof(CHP) ==  12 == 0xC
				if (pos + 4 > size) return;
				unsigned int	fcPic	= oStream.ReadUInt16();	pos += 4;	//pos = 8
				grpprl->push_back(SinglePropertyModifier(sprmOldCPicLocation, 4, (BYTE*)&fcPic));

				if (pos + 1 > size) return;
				unsigned char	fnPic	= oStream.ReadByte();	pos += 1;

				if (pos + 2 > size) return;
				unsigned short  hpsLargeChp	= oStream.ReadUInt16();	pos += 2;// ??? type
			}
			else if (nWordVersion == 2)
			{
				if (pos + 2 > size) return;
				unsigned short  fChar	= oStream.ReadUInt16(); pos += 2;

				unsigned char val;
				val = GETBIT(fChar, 0); grpprl->push_back(SinglePropertyModifier(sprmOldCFBold,		1, &val));
				val = GETBIT(fChar, 1); grpprl->push_back(SinglePropertyModifier(sprmOldCFItalic,	1, &val));
				val = GETBIT(fChar, 2); grpprl->push_back(SinglePropertyModifier(sprmOldCIbstRMark,	1, &val));
				val = GETBIT(fChar, 3); grpprl->push_back(SinglePropertyModifier(sprmOldCFOutline,	1, &val));
				val = GETBIT(fChar, 4); grpprl->push_back(SinglePropertyModifier(sprmOldCFFldVanish, 1, &val));
				val = GETBIT(fChar, 5); grpprl->push_back(SinglePropertyModifier(sprmOldCFSmallCaps, 1, &val));
				val = GETBIT(fChar, 6); grpprl->push_back(SinglePropertyModifier(sprmOldCFCaps,		1, &val));
				val = GETBIT(fChar, 7); grpprl->push_back(SinglePropertyModifier(sprmOldCFVanish,	1, &val));
				val = GETBIT(fChar, 8); grpprl->push_back(SinglePropertyModifier(sprmOldCFRMark,	1, &val));
				val = GETBIT(fChar, 9); grpprl->push_back(SinglePropertyModifier(sprmOldCFSpec,		1, &val));
				val = GETBIT(fChar, 10); grpprl->push_back(SinglePropertyModifier(sprmOldCFStrike,	1, &val));
				val = GETBIT(fChar, 11); grpprl->push_back(SinglePropertyModifier(sprmOldCFObj,		1, &val));
				val = GETBIT(fChar, 12); grpprl->push_back(SinglePropertyModifier(sprmCFBoldBi,		1, &val));
				val = GETBIT(fChar, 13); grpprl->push_back(SinglePropertyModifier(sprmCFItalicBi,	1, &val));
				val = GETBIT(fChar, 14); grpprl->push_back(SinglePropertyModifier(sprmCFBiDi,		1, &val));
				val = GETBIT(fChar, 15); grpprl->push_back(SinglePropertyModifier(sprmCFDiacColor,	1, &val));

				if (pos + 2 > size) return;
				unsigned short  fChar2 = oStream.ReadUInt16();	pos += 2;
				bool fsIco = GETBIT(fChar2, 0);
				bool fsFtc = GETBIT(fChar2, 1);
				bool fsHps = GETBIT(fChar2, 2);
				bool fsKul = GETBIT(fChar2, 3);
				bool fsPos = GETBIT(fChar2, 4);
				bool fsSpace = GETBIT(fChar2, 5);
				bool fsLid = GETBIT(fChar2, 6);
				bool fsIcoBi = GETBIT(fChar2, 7);
				bool fsFtcBi = GETBIT(fChar2, 8);
				bool fsHpsBi = GETBIT(fChar2, 9);
				bool fsLidBi = GETBIT(fChar2, 10);

				if (pos + 2 > size) return;
				unsigned short	ftc		= oStream.ReadUInt16();	pos += 2;		// Font Code
				grpprl->push_back(SinglePropertyModifier(sprmOldCFtc, 2, (unsigned char*)&ftc));

				if (pos + 1 > size) return;
				unsigned char	hps		= oStream.ReadByte();	pos += 1;		// Font size in half points

				if (hps > 0)
				{
					grpprl->push_back(SinglePropertyModifier(sprmOldCHps, 1, &hps));
				}

				if (pos + 1 > size) return;
				unsigned char   hpsPos	= oStream.ReadByte();	pos += 1;		// Sub/Superscript ( signed number, 0 = normal )
				grpprl->push_back(SinglePropertyModifier(sprmOldCHpsPos, 1, &hpsPos));

				if (pos + 2 > size) return;
				unsigned short  fText	= oStream.ReadUInt16();	pos += 2;

				unsigned short qpsSpace = GETBITS(fText, 0, 5);
				unsigned char wSpare2 = GETBITS(fText, 6, 7);
				unsigned char ico = GETBITS(fText, 8, 11);
				unsigned char kul = GETBITS(fText, 12, 14);
				bool fSysVanish = GETBIT(fChar, 15);

				grpprl->push_back(SinglePropertyModifier(sprmOldCKul,	1, &kul));
				grpprl->push_back(SinglePropertyModifier(sprmOldCIco,	1, &ico));

				//if (pos + 1 > size) return;
				//unsigned char  icoBi	= oStream.ReadUInt16();	pos += 1;//wSpare3

				if (pos + 2 > size) return;
				unsigned short  lid	= oStream.ReadUInt16();	pos += 2;
				grpprl->push_back(SinglePropertyModifier(sprmOldCLid, 2, (BYTE*)&lid));

				if (pos + 2 > size) return;
				unsigned short ftcBi	= oStream.ReadUInt16();	pos += 2;
				grpprl->push_back(SinglePropertyModifier(sprmCFtcBi, 4, (BYTE*)&ftcBi));

				//if (pos + 2 > size) return;
				//unsigned short hpsBi	= oStream.ReadUInt16();	pos += 2;
				//grpprl->push_back(SinglePropertyModifier(sprmCHpsBi, 4, (BYTE*)&hpsBi));

				//if (pos + 2 > size) return;
				//unsigned short  lidBi	= oStream.ReadUInt16();	pos += 2;
				//grpprl->push_back(SinglePropertyModifier(sprmCLidBi, 2, (BYTE*)&lidBi));

				if (pos + 4 > size) return;
				unsigned int	fcPic	= oStream.ReadUInt16();	pos += 4;	//pos = 8
				grpprl->push_back(SinglePropertyModifier(sprmOldCPicLocation, 4, (BYTE*)&fcPic));

				if (pos + 1 > size) return;
				unsigned char	fnPic	= oStream.ReadByte();	pos += 1;

				if (pos + 2 > size) return;
				unsigned short  hpsLargeChp	= oStream.ReadUInt16();	pos += 2;// ??? type
			}
		}
	}
}
