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

#include "StructuredStorageReader.h"

namespace DocFileFormat
{	
	StructuredStorageReader::StructuredStorageReader () : m_pStorage(NULL)
	{

	}
	StructuredStorageReader::~StructuredStorageReader()
	{
		if(m_pStorage)
		{
			delete m_pStorage;
			m_pStorage = NULL;
		}
	}

	bool StructuredStorageReader::SetFile (const wchar_t* filename)
	{
		m_pStorage = new POLE::Storage(filename);

		if (m_pStorage)//return true;
		{
			return m_pStorage->open();
		}
		return false;
	}
	bool StructuredStorageReader::isDirectory( const std::wstring& name )
	{
		if (!m_pStorage) return false;

		return m_pStorage->isDirectory(name);
	}

	bool StructuredStorageReader::GetStream (const std::wstring & path, POLE::Stream** ppStream)
	{
		if (( m_pStorage != NULL ) && ( !path.empty() ))
		{
			*ppStream = new POLE::Stream(m_pStorage, path.c_str());
		}
		if ((*ppStream)  && ((*ppStream)->size() > 0))
			return true;

		return false;
	}
	
	void StructuredStorageReader::copy( int indent, std::wstring path, POLE::Storage * storageOut, bool withRoot)
	{
		std::list<std::wstring> entries, entries_sort;
		entries = m_pStorage->entries_with_prefix( path );

		for( std::list<std::wstring>::iterator it = entries.begin(); it != entries.end(); ++it )
		{
			std::wstring name = *it;
			std::wstring fullname = path + name;

			if( m_pStorage->isDirectory( fullname ) )
			{
				entries_sort.push_back(name);
			}
			else
			{
				entries_sort.push_front(name);
			}
		}
		//for( std::list<std::string>::iterator it = entries.begin(); it != entries.end(); ++it )
		for( std::list<std::wstring>::iterator it = entries_sort.begin(); it != entries_sort.end(); ++it )
		{
			std::wstring name = *it;
			std::wstring fullname = path + name;

			if( m_pStorage->isDirectory( fullname ) )
			{
				copy( indent + 1, fullname + L"/", storageOut, withRoot );
			}
			else
			{
				copy_stream(fullname, storageOut, withRoot);
			}
		}
	}

	void StructuredStorageReader::copy_stream(std::wstring streamName, POLE::Storage * storageOut, bool withRoot)
	{
		POLE::Stream *stream = new POLE::Stream(m_pStorage, streamName);
		if (!stream) return;

		stream->seek(0);
		POLE::int64 size_stream = stream->size();

		if (withRoot == false)
		{
			int pos = (int)streamName.find(L"/");
			if (pos >= 0)
				streamName = streamName.substr(pos + 1);
		}

		POLE::Stream *streamNew = new POLE::Stream(storageOut, streamName, true, size_stream);
		if (!streamNew) return;

		unsigned char* data_stream = new unsigned char[(unsigned int)size_stream];
		if (data_stream)
		{
			stream->read(data_stream, size_stream);

			streamNew->write(data_stream, size_stream);

			delete []data_stream;
			data_stream = NULL;
		}

		streamNew->flush();

		delete streamNew;
		delete stream;
	}
}
