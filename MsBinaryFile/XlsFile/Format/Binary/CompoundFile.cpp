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

#include "CompoundFile.h"
#include "CFStream.h"

#include <sstream>

namespace XLS
{


CompoundFile::~CompoundFile()
{
	streams.clear();

	if (storage_)delete storage_;
	storage_ = NULL;
}

bool CompoundFile::isError()
{
	if (storage_ != NULL) return false;
	return true;	
}

bool CompoundFile::Open(const std::wstring & file_path, const ReadWriteMode mode)
{
	if (storage_) delete storage_;
	storage_ = NULL;

	rwMode = mode;

	unsigned int grfMode = 0;

	storage_ = new POLE::Storage(file_path.c_str());
	if (storage_ == NULL) return false;

	switch(rwMode)
	{		
		case cf_ReadMode:
		{
			if (storage_->open(false, false) == false)
			{
				delete storage_;
				storage_ = NULL;
			}
			
		}break;
		case cf_WriteMode:
		{
			if (storage_->open(true, true) == false)
			{
				delete storage_;
				storage_ = NULL;
			}
		}break;
	}

	if (storage_ == NULL) return false;
	return true;
}


CompoundFile::CompoundFile(const std::wstring & file_path, const ReadWriteMode mode)
{
	storage_ = NULL;
	Open(file_path, mode);
}

void CompoundFile::copy_stream(std::wstring streamNameOpen, std::wstring streamNameCreate, POLE::Storage * storageOut, bool bWithRoot)
{
	POLE::Stream *stream = new POLE::Stream(storage_, streamNameOpen);
	if (!stream) return;

	stream->seek(0);
	POLE::uint64 size_stream = stream->size();

	if (bWithRoot == false)
	{
		int pos = streamNameCreate.find(L"/");
		if (pos >= 0)
			streamNameCreate = streamNameCreate.substr(pos + 1);
	}
	
	POLE::Stream *streamNew = new POLE::Stream(storageOut, streamNameCreate, true, size_stream);
	if (!streamNew) return;

	unsigned char buffer[4096];
	int bytesRead = 0;

	while(true)
	{
		int bytesToRead = size_stream - bytesRead;
		if (bytesToRead <= 0)
			break;
		if (bytesToRead > 4096)
			bytesToRead = 4096;
	
		stream->read(buffer, bytesToRead);
		streamNew->write(buffer, bytesToRead);
		
		bytesRead += bytesToRead;
	}
	//unsigned char* data_stream = new unsigned char[size_stream + 64];
	//memset(data_stream, 0, size_stream + 64);
	//if (data_stream)
	//{
	//	stream->read(data_stream, size_stream);

	//	streamNew->write(data_stream, size_stream);

	//	delete []data_stream;
	//	data_stream = NULL;
	//}

	streamNew->flush();
			
	delete streamNew;
	delete stream;
}

void CompoundFile::copy( int indent, std::wstring path, POLE::Storage * storageOut, bool bWithRoot, bool bSortFiles)
{
    std::list<std::wstring> entries, entries_files, entries_dir;
    
	entries = storage_->entries_with_prefix( path );
	
	for( std::list<std::wstring>::iterator it = entries.begin(); it != entries.end(); ++it )
	{
        std::wstring fullname = path + *it;
       
        if ((it->at(0) >= 32) && (storage_->isDirectory( fullname ) ))
		{
			entries_dir.push_back(*it);
		}
		else
		{
			entries_files.push_front(*it);
		}
	}

	for( std::list<std::wstring>::iterator it = entries_dir.begin(); it != entries_dir.end(); ++it )
	{
        std::wstring fullname = path + *it;
       
		copy( indent + 1, fullname + L"/", storageOut, bWithRoot, bSortFiles );
    }

	//entries_files.sort();

	for( std::list<std::wstring>::iterator it = entries_files.begin(); it != entries_files.end(); ++it )
	{
		std::wstring createName = path + *it;
		std::wstring openName;
		
		if (it->at(0) < 32) openName = path + it->substr(1);
		else				openName = path + *it;
	   
		copy_stream(openName, createName, storageOut, bWithRoot);
	}
}
CFStreamPtr CompoundFile::getWorkbookStream()
{
	CFStreamPtr stream = getNamedStream(L"Workbook");

	if (stream == NULL) 
		stream = getNamedStream(L"WORKBOOK"); //6447323.xls
	if (stream == NULL) 
		stream = getNamedStream(L"Book");
	if (stream == NULL) 
		stream = getNamedStream(L"BOOK");//file(6).xls
	if (stream == NULL) 
		stream = getNamedStream(L"book");
	return stream;
}

CFStreamPtr CompoundFile::getNamedStream(const std::wstring& name)
{
	if(!streams[name])
	{
		POLE::Stream * pStream = openStream(name.c_str());
		if (pStream)	
			streams[name].reset(new CFStream(pStream));
	}
	return streams[name];
}

CFStreamPtr CompoundFile::createNamedStream(const std::wstring& name)
{
	if(!streams[name])
	{
		POLE::Stream * pStream = createStream(name.c_str());
		if (pStream)	
			streams[name].reset(new CFStream(pStream));
	}
	return streams[name];
}


void CompoundFile::closeNamedStream(const std::wstring& name)
{
	streams[name].reset();
}


// Opens a stream in the storage (shall be called not more than once per stream)
POLE::Stream* CompoundFile::openStream(const std::wstring & stream_name)
{
	if (storage_ == NULL) return NULL;

	POLE::Stream* pStream = new POLE::Stream(storage_, stream_name);
	if(pStream == NULL)
	{
		return NULL;
	}
	if ((pStream) && (pStream->size() > 0))
		return pStream;
	else return NULL;
}


// Creates a new stream in the storage
POLE::Stream* CompoundFile::createStream(const std::wstring & stream_name)
{
	if (storage_ == NULL) return NULL;

	POLE::Stream* pStream = new POLE::Stream(storage_, stream_name, true);
	if(pStream == NULL)
	{
		return NULL;
	}
	return pStream;
}


} // namespace XLS


