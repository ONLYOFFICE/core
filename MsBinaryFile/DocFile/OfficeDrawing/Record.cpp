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
#include "Record.h"

namespace DocFileFormat
{
	Record::~Record()
	{
		RELEASEARRAYOBJECTS( RawData );
		RELEASEOBJECT( Reader );
	}
	Record::Record():
		HeaderSize(0), BodySize(0), RawData(NULL), SiblingIdx(0), TypeCode(0), Version(0), Instance(0), Reader(NULL),
		_ParentRecord(NULL)
	{
	}
	Record::Record( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance ):
		HeaderSize(Record::HEADER_SIZE_IN_BYTES), BodySize(bodySize), RawData(NULL), SiblingIdx(0), TypeCode(typeCode), Version(version), Instance(instance), Reader(NULL),
		_ParentRecord(NULL)
	{
		HeaderSize = Record::HEADER_SIZE_IN_BYTES;

		int real_size = _reader->GetSize() - _reader->GetPosition();

		if (real_size < BodySize)
		{
			BodySize = real_size;
		}

		RawData = _reader->ReadBytes( BodySize, true );
		Reader = new MemoryStream( RawData, BodySize, false );
	}

	unsigned int Record::GetTotalSize() const
	{
		return ( HeaderSize + BodySize );
	}
}
