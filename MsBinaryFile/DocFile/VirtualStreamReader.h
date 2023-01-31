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

#include "IBinaryReader.h"
#include "../Common/Base/FormatUtils.h"
#include "../../Common/3dParty/pole/pole.h"
#include "../../OOXML/Base/Base.h"

class VirtualStreamReader : public IBinaryReader
{
public:
	VirtualStreamReader (POLE::Stream* _stream, ULONG _position , int _nWordVersion);
	virtual ~VirtualStreamReader();

	virtual unsigned short ReadUInt16();
	virtual short ReadInt16();

	virtual int ReadInt32();
	virtual unsigned int ReadUInt32();

	virtual unsigned char ReadByte();
	virtual unsigned char* ReadBytes( unsigned int count, bool isResultNeeded );

	virtual unsigned long GetPosition() const;
	virtual unsigned long GetSize() const;
	virtual int Seek( int offset, int origin );

	std::wstring ReadXst();

	/// Read a length prefixed Unicode string from the given stream.
	/// The string must have the following structure:
	/// unsigned char 1 - 4: Character count (cch)
	/// unsigned char 5 - (cch*2)+4: Unicode characters terminated by \0
	std::wstring ReadLengthPrefixedUnicodeString();

	/// Read a length prefixed ANSI string from the given stream.
	/// The string must have the following structure:
	/// unsigned char 1-4: Character count (cch)
	/// unsigned char 5-cch+4:   ANSI characters terminated by \0
	std::wstring ReadLengthPrefixedAnsiString(unsigned int max_size);

	int			nWordVersion;

private:

	POLE::uint64			position;
	POLE::Stream*			stream;
};
