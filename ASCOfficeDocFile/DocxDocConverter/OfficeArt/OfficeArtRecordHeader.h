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

namespace OfficeArt
{
	class OfficeArtRecordHeader
	{
	private:

		struct
		{
			unsigned int recVer:4;
			unsigned int recInstance:12;
			unsigned int recType:16;
			unsigned int recLen:32;
		} OfficeArtRecordHeaderStruct;

	public:

		OfficeArtRecordHeader()
		{
			OfficeArtRecordHeaderStruct.recVer		=	0;
			OfficeArtRecordHeaderStruct.recInstance	=	0;
			OfficeArtRecordHeaderStruct.recType		=	0xF000;
			OfficeArtRecordHeaderStruct.recLen		=	0;
		}

		explicit OfficeArtRecordHeader (unsigned char _recVer, unsigned short _recInstance, unsigned short _recType, unsigned int _recLen)
		{
			OfficeArtRecordHeaderStruct.recVer		=	_recVer;
			OfficeArtRecordHeaderStruct.recInstance	=	_recInstance;

			if ( ( _recType >= 0xF000 ) && ( _recType <= 0xFFFF ) )
			{
				OfficeArtRecordHeaderStruct.recType	=	_recType;
			}
			else
			{
				OfficeArtRecordHeaderStruct.recType	=	0xF000; 
			}

			OfficeArtRecordHeaderStruct.recLen		=	_recLen;
		}

		inline unsigned char GetVersion() const
		{
			return OfficeArtRecordHeaderStruct.recVer;
		}

		inline unsigned short GetInstance() const
		{
			return OfficeArtRecordHeaderStruct.recInstance;
		}

		inline unsigned short GetType() const
		{
			return OfficeArtRecordHeaderStruct.recType;
		}

		inline unsigned int GetLength() const
		{
			return OfficeArtRecordHeaderStruct.recLen;
		}

		inline operator const unsigned char* () const
		{
			return (const unsigned char*)(&OfficeArtRecordHeaderStruct);
		}

		inline operator unsigned char* () const
		{
			return (unsigned char*)(&OfficeArtRecordHeaderStruct);
		}
	};
}