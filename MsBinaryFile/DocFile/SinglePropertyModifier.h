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

#include "../Common/Base/SPRMCodes.h"

namespace DocFileFormat
{
	/// Identifies the type of a SPRM
	typedef enum SprmType
	{
		PAP = 1,
		CHP = 2,
		PIC = 3,
		SEP = 4,
		TAP = 5
	} SprmType;

	static const int OpCode93To95[] = 
	{
		0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	51,	52,	65,	66,	67,	0,	82,	83,	0,	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	0,	0x085C,	0x085D,	0x4A5E,	0x485F,	0x4A60,	0x4A61,	0x085A,	0,	0,	0,	0,	0,	0,	0,	119,	120,	121,	122,	123,	124,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x322A,	0,	0x3228,	140,	141,	142,	143,	144,	145,	146,	147,	148,	149,	150,	151,	152,	153,	154,	155,	156,	157,	158,	159,	160,	161,	162,	163,	164,	165,	166,	167,	168,	169,	170,	182,	183,	184,	0x560B,	0,	0,	188,	189,	190,	191,	192,	193,	194,	195,	196,	197,	198,	199,	200,	208,
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 0,	0,	0,	0,	0,	0,	0
	};

	class SinglePropertyModifier
	{
	public:
		int nWordVersion;
		/// The operation code identifies the property of the 
		/// PAP/CHP/PIC/SEP/TAP which sould be modified
		OperationCode OpCode;
		/// This SPRM requires special handling
		bool fSpec;
		/// The type of the SPRM
		SprmType Type;
		/// The arguments which is applied to the property
		unsigned char* Arguments;
		/// The Arguments size
		unsigned int argumentsSize;

		SinglePropertyModifier( int nWordVersion);
		/// parses the unsigned char to retrieve a SPRM
		SinglePropertyModifier( unsigned char* bytes, int size, int nWordVersion );
		SinglePropertyModifier( const SinglePropertyModifier& spm);
		SinglePropertyModifier( OperationCode opCode, int argumentsSize, unsigned char* arguments);
		
		bool operator == ( const SinglePropertyModifier& spm ) const;
		bool operator != ( const SinglePropertyModifier& spm ) const;
		SinglePropertyModifier& operator = ( const SinglePropertyModifier& spm );
		~SinglePropertyModifier();
		/// Get be used to get the size of the sprm's operand.
		/// Returns 0 if the Operation failed and 255 if the size is variable
		static unsigned char GetOperandSize( unsigned char spra );
		static unsigned char GetOldOperandSize( unsigned char code);
	};
}
