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

#include "VbaBinary.h"

#include <boost/bind.hpp>

#include <algorithm>
#include <sstream>



CVbaFile::~CVbaFile()
{
	streams.clear();

	if (storage_)delete storage_;
	storage_ = NULL;
}

bool CVbaFile::isError()
{
	if (storage_ != NULL) return false;
	return true;
}

bool CVbaFile::Open(const std::wstring & file_path)
{
	if (storage_) delete storage_;
	storage_ = NULL;

	storage_ = new POLE::Storage(file_path.c_str());
	if (storage_ == NULL) return false;

	if (storage_->open(false, false) == false)
	{
		delete storage_;
		storage_ = NULL;
	}

	if (storage_ == NULL) return false;
	return true;
}
CVbaFile::CVbaFile(const std::wstring & file_path)
{
	storage_ = NULL;
	Open(file_path);
}
CVbaFileStreamPtr CVbaFile::getNamedStream(const std::wstring& name, _UINT32 offset)
{
	if (!streams[name])
	{
		POLE::Stream * pStream = openStream(name.c_str());
		if (pStream)
			streams[name].reset(new CVbaFileStream(pStream, offset));
	}
	return streams[name];
}

void CVbaFile::closeNamedStream(const std::wstring& name)
{
	streams[name].reset();
}

POLE::Stream* CVbaFile::openStream(const std::wstring & stream_name)
{
	if (storage_ == NULL) return NULL;

	POLE::Stream* pStream = new POLE::Stream(storage_, stream_name);
	if (pStream == NULL)
	{
		return NULL;
	}
	if ((pStream) && (pStream->size() > 0))
		return pStream;
	else return NULL;
}
const unsigned char VBASTREAM_SIGNATURE = 1;

const _UINT16 CHUNK_SIGMASK = 0x7000;
const _UINT16 CHUNK_SIG = 0x3000;
const _UINT16 CHUNK_COMPRESSED = 0x8000;
const _UINT16 CHUNK_LENMASK = 0x0FFF;

inline _UINT16 extract(_UINT16 nBitField, unsigned char nStartBit, unsigned char nBitCount)
{
	_UINT64 nMask = 1; nMask <<= nBitCount; --nMask;
	return static_cast< _UINT16 >(nMask & (nBitField >> nStartBit));
}
CVbaFileStream::CVbaFileStream(POLE::Stream* stream, _UINT32 offset)
{
	size_t dataSize = stream->size();

	unsigned char sig = 0;
	stream->read(&sig, 1);

	if (sig != 1)
	{
		stream->seek(0);
		arrChunks.resize(dataSize);
		stream->read(arrChunks.data(), dataSize);

		return;
	}
	dataSize -= 1;

	if (offset > dataSize)
	{	
		return; //error;
	}
	
	stream->seek(stream->tell() + offset);
	dataSize -= offset;
//readchunks
	unsigned char *data = new unsigned char[dataSize];
	stream->read(data, dataSize);

	unsigned char *dataCur = data;

	while (dataCur - data <  dataSize - 2)
	{
		std::vector<unsigned char> arrChunk;
		_UINT16 header = *((_UINT32*)dataCur); dataCur += 2;

		bool bCompressed = ((header & CHUNK_COMPRESSED) != 0);
		_UINT16 chunkSize = (header & CHUNK_LENMASK) + 1;

		bool bUnknown = false;
		if ((header & CHUNK_SIGMASK) != CHUNK_SIG)
		{
			bCompressed = true;
			chunkSize = 4094; //по факту
			bUnknown = true;
		}
		unsigned char *dataNext = dataCur + chunkSize;

		if (bCompressed)
		{
			unsigned char nBitCount = 4;
			_UINT16 chunkPos = 0;

			bool bEof = stream->eof();
			while (!bEof && ((dataCur - data) > 0) && (chunkPos < chunkSize))
			{
				unsigned char nToken = *dataCur; dataCur++;
				++chunkPos;
			
				for (int bit = 0; !bEof && ((dataCur - data) > 0) && (bit < 8) && (chunkPos < chunkSize); ++bit, nToken >>= 1)
				{
					if (nToken & 1)
					{
						_UINT16 nCopyToken = *((_UINT32*)dataCur); dataCur += 2;
						chunkPos += 2;

						while ((static_cast<size_t>(1) << nBitCount) < arrChunk.size())
							++nBitCount;
						
						_UINT16 nLength = extract(nCopyToken, 0, 16 - nBitCount) + 3;					
						_UINT16 nOffset = extract(nCopyToken, 16 - nBitCount, nBitCount) + 1;

						bEof = (nOffset > arrChunk.size()) || (arrChunk.size() + nLength > 4096);
					
						if (!bEof)
						{
							arrChunk.resize(arrChunk.size() + nLength);
							
							unsigned char* pnTo = &*(arrChunk.end() - nLength);
							
							const unsigned char* pnEnd = pnTo + nLength;
							const unsigned char* pnFrom = pnTo - nOffset;

							size_t nRunLen = (std::min)(nLength, nOffset);
							while (pnTo < pnEnd)
							{
								size_t nStepLen = (std::min)(nRunLen, (size_t)(pnEnd - pnTo));
								memcpy(pnTo, pnFrom, nStepLen);
								pnTo += nStepLen;
							}
						}
					}
					else
					{
						arrChunk.push_back(*dataCur); dataCur++;
						++chunkPos;
					}
				}
			}
		}
		else
		{
			arrChunk.resize(chunkSize);
			memcpy(arrChunk.data(), dataCur, chunkSize); dataCur += chunkSize;
		}

		dataCur = dataNext;
		arrChunks.insert(arrChunks.end(), arrChunk.begin(), arrChunk.end());
	}
}
CVbaFileStream::~CVbaFileStream()
{
	arrChunks.clear();
}
void CVbaFileStream::read(void* buf, size_t size)
{
	if (NULL == buf || arrChunks.empty())
	{
		return;
	}
	if (pos + size > arrChunks.size())
		size = arrChunks.size() - pos;
	memcpy(buf, arrChunks.data() + pos, size);
	pos += size;

}
