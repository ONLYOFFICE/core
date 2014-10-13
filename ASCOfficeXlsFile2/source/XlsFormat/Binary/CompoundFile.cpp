#include "precompiled_xls.h"
#include "CompoundFile.h"
#include "CFStream.h"
//#include <Exception/CompoundFileFormatError.h>
#include <sstream>

namespace XLS
{;


CompoundFile::~CompoundFile()
{
	storage_->Commit(0);
	streams.clear();
	storage_->Release();
}

CompoundFile::CompoundFile(const std::wstring & file_path, const ReadWriteMode mode)
:	rwMode(mode)
{
	HRESULT hres = S_OK;
	unsigned int grfMode = 0;

	switch(rwMode)
	{
		case cf_ReadMode:
			hres = StgIsStorageFile(file_path.c_str());
			switch(hres)
			{
			case S_FALSE:
				throw;// EXCEPT::RT::CompoundFileFormatError("The specified file \"" + std::string(static_cast<char*>(file_path)) + "\" is not a valid Compound File.", hres);;
			case STG_E_FILENOTFOUND:
				throw;// EXCEPT::RT::CompoundFileFormatError("Not found the path \"" + std::string(static_cast<char*>(file_path)) + "\"", hres);;
			}
			grfMode = STGM_READ | STGM_DIRECT | STGM_SHARE_DENY_WRITE;
			if(FAILED( hres = StgOpenStorage(file_path.c_str(), NULL, grfMode, NULL, 0, &storage_)))
			{
				throw;// EXCEPT::RT::CompoundFileFormatError("Error opening \"" + std::string(static_cast<char*>(file_path)) + "\"", hres);
			}
			break;
		case cf_WriteMode:
			grfMode = STGM_CREATE | STGM_READWRITE | STGM_SIMPLE | STGM_SHARE_EXCLUSIVE;
			if(FAILED( hres = StgCreateStorageEx(file_path.c_str(), grfMode, STGFMT_STORAGE, 0, NULL, NULL,
						IID_IStorage, reinterpret_cast<void**>(&storage_))))
			{
				throw;// EXCEPT::RT::CompoundFileFormatError("Error creating \"" + std::string(static_cast<char*>(file_path)) + "\"", hres);
			}
			break;
	}



/*
	IEnumSTATSTG* storageInfo = NULL;
	if(FAILED(hres = storage_->EnumElements(0, 0, 0, &storageInfo)))
	{
		throw 1;
	}
	
	Log::info("The Compound File contains the following streams:");
	STATSTG elem;
	while(S_OK == storageInfo->Next(1, &elem, 0))
	{
		Log::info(std::string("name: \"") + static_cast<char*>(std::wstring (elem.pwcsName)) + "\"  type: " + STR::int2str(elem.type) + " size: " +
			STR::int2hex_wstr(elem.cbSize.HighPart) + STR::int2hex_wstr(elem.cbSize.LowPart));
	}
*/
}

// Opens "Workbook" stream and returns the only reference
CFStreamPtr CompoundFile::getWorkbookStream()
{
	return getNamedStream(L"Workbook");
}


// Creates "Workbook" stream and returns the only reference
CFStreamPtr CompoundFile::createWorkbookStream()
{
	return createNamedStream(L"Workbook");
}


// Opens "SummaryInformation" stream and returns the only reference
CFStreamPtr CompoundFile::getSummaryInformationStream()
{
	return getNamedStream(L"\005SummaryInformation");
}


// Creates "SummaryInformation" stream and returns the only reference
CFStreamPtr CompoundFile::createSummaryInformationStream()
{
	return createNamedStream(L"\005SummaryInformation");
}


// Closes "SummaryInformation" stream
void CompoundFile::closeSummaryInformationStream()
{
	return closeNamedStream(L"\005SummaryInformation");
}


// Opens "SummaryInformation" stream and returns the only reference
CFStreamPtr CompoundFile::getDocumentSummaryInformationStream()
{
	return getNamedStream(L"\005DocumentSummaryInformation");
}


// Creates "SummaryInformation" stream and returns the only reference
CFStreamPtr CompoundFile::createDocumentSummaryInformationStream()
{
	return createNamedStream(L"\005DocumentSummaryInformation");
}


// Closes "SummaryInformation" stream
void CompoundFile::closeDocumentSummaryInformationStream()
{
	closeNamedStream(L"\005DocumentSummaryInformation");
}


CFStreamPtr CompoundFile::getNamedStream(const std::wstring& name)
{
	if(!streams[name])
	{
		streams[name].reset(new CFStream(openStream(name.c_str())));
	}
	return streams[name];
}


CFStreamPtr CompoundFile::createNamedStream(const std::wstring& name)
{
	if(!streams[name])
	{
		streams[name].reset(new CFStream(createStream(name.c_str())));
	}
	return streams[name];
}


void CompoundFile::closeNamedStream(const std::wstring& name)
{
	streams[name].reset();
}


// Opens a stream in the storage (shall be called not more than once per stream)
IStream* CompoundFile::openStream(const std::wstring & stream_name)
{
	IStream* pStream = NULL;
	HRESULT hres = storage_->OpenStream(stream_name.c_str(), NULL, STGM_READ | STGM_DIRECT | STGM_SHARE_EXCLUSIVE, NULL, &pStream );
	if(FAILED(hres))
	{
		throw;// EXCEPT::RT::CompoundFileFormatError(std::string("Error opening \"") + static_cast<char*>(stream_name) + "\" stream", hres);
	}
	return pStream;
}


// Creates a new stream in the storage
IStream* CompoundFile::createStream(const std::wstring & stream_name)
{
	IStream* pStream = NULL;
	HRESULT hres = storage_->CreateStream(stream_name.c_str(), STGM_READWRITE | STGM_SHARE_EXCLUSIVE, NULL, NULL, &pStream);
	if(FAILED(hres))
	{
		throw;// EXCEPT::RT::CompoundFileFormatError(std::string("Error creating \"") + static_cast<char*>(stream_name) + "\" stream", hres);
	}
	return pStream;
}


} // namespace XLS


