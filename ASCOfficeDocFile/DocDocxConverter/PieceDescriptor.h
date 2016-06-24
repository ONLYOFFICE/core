/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

#include "../Common/FormatUtils.h"


namespace DocFileFormat
{
	class PieceDescriptor
	{
		friend class PieceTable;
	public:
		/// Parses the bytes to retrieve a PieceDescriptor
		PieceDescriptor(unsigned char *bytes, unsigned int size) : fc(0), encoding(ENCODING_INVALID_VALUE), cpStart(0), cpEnd(0)
		{
			if (8 == size)
			{
				//get the fc value
				unsigned int fcValue = FormatUtils::BytesToUInt32(bytes, 2, size);

				//get the flag
				bool flag = FormatUtils::BitmaskToBool((int)fcValue, 0x40000000);

				//delete the flag
				fcValue = fcValue & 0xBFFFFFFF;

				//find encoding and offset
				if (flag)
				{
					this->encoding = ENCODING_WINDOWS_1251;
					this->fc = (unsigned int)( fcValue / 2 );
				}
				else
				{
					this->encoding = ENCODING_UNICODE;
					this->fc = fcValue;
				}
			}
			else
			{
				//!!!TODO!!!
				//throw new ByteParseException("Cannot parse the struct PCD, the length of the struct doesn't match");
			}
		}

	public:
		/// File offset of beginning of piece.
		/// This is relative to the beginning of the WordDocument stream.
		unsigned int fc;
		/// The encoding of the piece
		ASCDocFormatUtils::Encoding encoding;
		int cpStart;
		int cpEnd;
	};
}
