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

#include "../../../Common/3dParty/pole/pole.h"
#include "../../../Common/cfcpp/guid.h"

#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#define _CP_OPT(V1)  boost::optional<V1>

typedef struct _frmSize
{
	_UINT32 Width = 0;
	_UINT32 Height = 0;
} frmSize;

typedef struct _fmPosition
{
	_UINT32 Top = 0;
	_UINT32 Left = 0;
} fmPosition;

class CVbaFileStream
{
public:
	CVbaFileStream(POLE::Stream* stream, _UINT32 offset = 0);
	~CVbaFileStream();

	template<class Type>
	CVbaFileStream& operator >> (Type& val)
	{
		read(&val, sizeof(Type));
		return *this;
	}

	void read(void* buf, size_t size);

//---------------------------------------------------------------------
	void Align(size_t val)
	{
		size_t padding = val - (pos % val);
		
		if (padding > 0 && padding < 4)
			pos += padding;
	}
	void Seek(const size_t _pos)
	{
		pos = _pos;
	}
	_UINT32 GetDataPos()
	{
		return pos;
	}
	void skipBytes(const size_t n)
	{
		pos += n;
		if (pos > arrChunks.size())
			pos = arrChunks.size();
	}
    void RollBack(const size_t n)
	{
		if (pos - n >= 0)
		{
			pos -= n;
		}
	}
	unsigned char* getDataCurrent() { return arrChunks.data() + pos; }
	unsigned char* getData() { return arrChunks.data(); }
	size_t getDataSize() { return arrChunks.size(); }

	const bool checkFitReadSafe(const size_t size) const
	{
		return (!arrChunks.empty() && (pos + size < arrChunks.size()));
	}
	bool checkFitRead(const size_t size) const
	{
		if (!checkFitReadSafe(size))
		{
			return false;
		}
		return true;
	}
	template<class T>
	bool loadAnyData(T& val) //for static size objects
	{
		size_t size = sizeof(T);
		if (checkFitRead(size))
		{
			memcpy((unsigned char*)&val, arrChunks.data() + pos, size);
			pos += size;
			return true;
		}
		return false;
	}

	CVbaFileStream& operator >> (unsigned char& val) { loadAnyData(val);	return *this; }
	CVbaFileStream& operator >> (int& val) { loadAnyData(val);		return *this; }
	CVbaFileStream& operator >> (double& val) { loadAnyData(val);	return *this; }
	CVbaFileStream& operator >> (short& val) { loadAnyData(val);	return *this; }
	CVbaFileStream& operator >> (char& val) { loadAnyData(val);		return *this; }
	CVbaFileStream& operator >> (_GUID_& val) { loadAnyData(val);	return *this; }
	CVbaFileStream& operator >> (_UINT32& val) { loadAnyData(val);	return *this; }
	CVbaFileStream& operator >> (_UINT16& val) { loadAnyData(val);	return *this; }

	CVbaFileStream& operator >> (_CP_OPT(unsigned char)& val) { val = 0; loadAnyData(*val);	return *this; }
	CVbaFileStream& operator >> (_CP_OPT(int)& val) { val = 0; loadAnyData(*val);		return *this; }
	CVbaFileStream& operator >> (_CP_OPT(double)& val) { val = 0; loadAnyData(*val);	return *this; }
	CVbaFileStream& operator >> (_CP_OPT(short)& val) { val = 0; loadAnyData(*val);		return *this; }
	CVbaFileStream& operator >> (_CP_OPT(char)& val) { val = 0; loadAnyData(*val);		return *this; }
	CVbaFileStream& operator >> (_CP_OPT(_GUID_)& val) { val = _GUID_(); loadAnyData(*val);	return *this; }
	CVbaFileStream& operator >> (_CP_OPT(_UINT32)& val) { val = 0; loadAnyData(*val);	return *this; }
	CVbaFileStream& operator >> (_CP_OPT(_UINT16)& val) { val = 0; loadAnyData(*val);	return *this; }
	CVbaFileStream& operator >> (_CP_OPT(frmSize)& val) { val = frmSize(); loadAnyData(*val);	return *this; }
	CVbaFileStream& operator >> (_CP_OPT(fmPosition)& val) { val = fmPosition(); loadAnyData(*val);	return *this; }

	_UINT32 CodePage = 0;
private:
	std::vector<unsigned char> arrChunks;
    size_t pos = 0;
};
typedef boost::shared_ptr<CVbaFileStream> CVbaFileStreamPtr;

class CVbaFile
{
public:
	CVbaFile(const std::wstring & file_path);
	~CVbaFile();

	bool Open(const std::wstring & file_path);

	bool isError();
	bool isDirectory(const std::wstring & name) { return (storage_ ? storage_->isDirectory(name) : false); }

	CVbaFileStreamPtr getNamedStream(const std::wstring& name, _UINT32 offset = 0);
private:
	POLE::Storage *storage_;

	POLE::Stream* openStream(const std::wstring & stream_name); // Opens a stream in the storage (shall be called not more than once per stream)
	void closeNamedStream(const std::wstring& name);

	std::map<std::wstring, CVbaFileStreamPtr>	streams;
};
typedef boost::shared_ptr<CVbaFile> CVbaFilePtr;
