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

namespace ASCDocFileFormat
{
	static const unsigned int LatentStylesTemplate [267] =
	{
		8, 152, 158, 158, 158, 158, 158, 158, 158, 158, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 630,
		630, 630, 630, 630, 630, 630, 630, 630, 1590, 1590, 1590, 1590, 1590, 1590, 574, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 168, 1590, 1590, 22, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 184,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 360, 328, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590, 1590,
		1590, 1590, 1590, 1590, 1590, 1590, 1590, 944, 1590, 1586, 24, 960, 976, 992, 1008, 1024, 1040, 1056, 1072,
		1088, 1104, 1120, 1136, 1152, 1168, 960, 976, 992, 1008, 1024, 1040, 1586, 552, 472, 488, 1056, 1072, 1088,
		1104, 1120, 1136, 1152, 1168, 960, 976, 992, 1008, 1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152,
		1168, 960, 976, 992, 1008, 1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 960, 976, 992, 1008,
		1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 960, 976, 992, 1008, 1024, 1040, 1056, 1072,
		1088, 1104, 1120, 1136, 1152, 1168, 960, 976, 992, 1008, 1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136,
		1152, 1168, 312, 344, 504, 520, 536, 598, 638
	};

	union LSD
	{
	private:
		struct
		{
			unsigned int fLocked			:	1;
			unsigned int fSemiHidden		:	1;
			unsigned int fUnhideWhenUsed	:	1;
			unsigned int fQFormat			:	1;
			unsigned int iPriority			:	12;
			unsigned int fReserved			:	16;
		} LSDStruct;
		unsigned int LSDUnsignedInt;

	public:

		LSD () : LSDUnsignedInt(0)
		{

		}

		LSD (unsigned int _lsd): LSDUnsignedInt(_lsd)
		{

		}

		LSD (bool _fLocked, bool _fSemiHidden, bool _fUnhideWhenUsed, bool _fQFormat, unsigned short _iPriority) : LSDUnsignedInt(0)
		{
			( _fLocked ) ? ( LSDStruct.fLocked = 1 ) : ( LSDStruct.fLocked = 0 );
			( _fSemiHidden ) ? ( LSDStruct.fSemiHidden = 1 ) : ( LSDStruct.fSemiHidden = 0 );
			( _fUnhideWhenUsed ) ? ( LSDStruct.fUnhideWhenUsed = 1 ) : ( LSDStruct.fUnhideWhenUsed = 0 );
			( _fQFormat ) ? ( LSDStruct.fQFormat = 1 ) : ( LSDStruct.fQFormat = 0 );

			if ( _iPriority > 0x0063 )
			{
				LSDStruct.iPriority = 0x0063;  
			}
			else
			{
				LSDStruct.iPriority = _iPriority;
			}
		}

		operator unsigned int() const
		{
			return LSDUnsignedInt;
		}
	};
}