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

#include "../Common/SPRMCodes.h"

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
