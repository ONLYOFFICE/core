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

#include "EncryptionStream.h"
#include "../Crypt/Decryptor.h"
#include "../Binary/CFStream.h"

#include "../../../../OOXML/Base/Base.h"
#include "../../../../DesktopEditor/common/Types.h"
#include "../../../../DesktopEditor/common/File.h"

namespace XLS
{
	EncryptionSummaryStream::EncryptionSummaryStream(XLS::CFStreamPtr stream, CRYPT::DecryptorPtr& decryptor)
	{
		if (!stream) return;
		if (!decryptor) return;

		char buf0[8];
		stream->read(buf0, 8);
		decryptor->Decrypt(buf0, 8, 0);

		_UINT32 StreamDescriptorArrayOffset, StreamDescriptorArraySize, EncryptedStreamDescriptorCount;

		StreamDescriptorArrayOffset = ((_UINT32*)buf0)[0];
		StreamDescriptorArraySize = ((_UINT32*)buf0)[1];
	// data
		unsigned long size = StreamDescriptorArrayOffset - 8;

		char *buf1 = new char[size];
		stream->read(buf1, size);

		decryptor->Decrypt(buf1, size, 0);

	//descriptions array
		char *buf2 = new char[StreamDescriptorArraySize];
		stream->read(buf2, StreamDescriptorArraySize);
		decryptor->Decrypt(buf2, StreamDescriptorArraySize, 0);

		EncryptedStreamDescriptorCount = ((_UINT32*)buf2)[0]; 
		_UINT32 pos = 4;

		for (size_t i = 0; i < EncryptedStreamDescriptorCount && pos < StreamDescriptorArraySize; ++i)
		{
			_UINT32 StreamOffset = ((_UINT32*)(buf2 + pos))[0]; pos += 4;
			_UINT32 StreamSize = ((_UINT32*)(buf2 + pos))[0]; pos += 4;
			_UINT16 Block = ((_UINT16*)(buf2 + pos))[0]; pos += 2;
			BYTE NameSize = ((BYTE*)(buf2 + pos))[0]; pos += 1;
			BYTE fStream = ((BYTE*)(buf2 + pos))[0]; pos += 1;
			pos += 4;

			std::wstring name;
			
			if (pos + NameSize < StreamDescriptorArraySize)
			{
				name = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)(buf2 + pos), NameSize);
			}
			pos += NameSize * 2;
			
			if (pos + 1 < StreamDescriptorArraySize && buf2[pos] == 0 && buf2[pos + 1] == 0)
				pos += 2; // padding???

			std::pair<boost::shared_array<unsigned char>, size_t> data;

			if (StreamSize + StreamOffset <= StreamDescriptorArrayOffset)
			{
				data.first = boost::shared_array<BYTE>(new BYTE[StreamSize]);
				data.second = StreamSize;

				memcpy(data.first.get(), buf1 + StreamOffset - 8, StreamSize); // 8 = start stream offset
			}
			streams.insert(std::make_pair(name, data));

		}

		delete[]buf1;
		delete[]buf2;
	}
	std::pair<boost::shared_array<unsigned char>, size_t> EncryptionSummaryStream::GetStream(const std::wstring &name)
	{
		std::map<std::wstring, std::pair<boost::shared_array<unsigned char>, size_t>>::iterator pFind =
			streams.find(name);

		if (pFind != streams.end())
			return pFind->second;
		else
		{
			std::pair<boost::shared_array<unsigned char>, size_t> empty(NULL, 0);
			return empty;
		}
	}
} // namespace XLS
