/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "PropertyExceptions.h"

namespace DocFileFormat
{
	class CharacterPropertyExceptions: public PropertyExceptions
	{
		public:
		  /// Creates a CHPX wich doesn't modify anything.
		  /// The grpprl list is empty
		CharacterPropertyExceptions(): PropertyExceptions()
		{
		}

		  /// Parses the bytes to retrieve a CHPX
		CharacterPropertyExceptions( unsigned char* bytes, int size, int nWordVersion) :
														PropertyExceptions( bytes, size, nWordVersion )
		{
			if (nWordVersion == 2 && size > 0)
			{
				RELEASEOBJECT( grpprl );
				grpprl = new std::list<SinglePropertyModifier>();
				
				MemoryStream oStream(bytes, size);
				int pos = 0;
				
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

				//if (fsKul)
				{
					grpprl->push_back(SinglePropertyModifier(sprmOldCKul,	1, &kul));
				}
				//if (fsIco)
				{
					grpprl->push_back(SinglePropertyModifier(sprmOldCIco,	1, &ico));
				}
				if (pos + 2 > size) return;
				unsigned int  xxx	= oStream.ReadUInt16();	pos += 2;// ???
				
				if (pos + 2 > size) return;
				unsigned int  yyy	= oStream.ReadUInt16();	pos += 2;// ???

				if (pos + 2 > size) return;
				unsigned int	fcPic	= oStream.ReadUInt16();	pos += 2;	
				grpprl->push_back(SinglePropertyModifier(sprmOldCPicLocation, 4, (BYTE*)&fcPic));
				
				if (pos + 2 > size) return;
				unsigned short  zzz	= oStream.ReadUInt16();	pos += 2;// ??? type

				//if (pos + 2 > size) return;
				//unsigned short  dx	= oStream.ReadUInt16();	pos += 2;// ???
				//
				//if (pos + 2 > size) return;
				//unsigned short  dy	= oStream.ReadUInt16();	pos += 2;// ???

				//short scale_complex[6] = {dx, dy, 0, 0, 0, 0};
				//grpprl->push_back(SinglePropertyModifier(sprmOldPicScale, 12, (BYTE*)scale_complex));
			}
		}
	};
}