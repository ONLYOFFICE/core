#pragma once

#include "objidl.h"
#include "BinSmartPointers.h"


namespace XLS
{;


class CompoundFile
{
public:
	enum ReadWriteMode
	{
		cf_WriteMode,
		cf_ReadMode
	};

public:
	CompoundFile(const std::wstring & file_path, const ReadWriteMode mode);
	~CompoundFile();

	CFStreamPtr getWorkbookStream();
	CFStreamPtr createWorkbookStream();

	CFStreamPtr getSummaryInformationStream();
	CFStreamPtr createSummaryInformationStream();
	void closeSummaryInformationStream();

	CFStreamPtr getDocumentSummaryInformationStream();
	CFStreamPtr createDocumentSummaryInformationStream();
	void closeDocumentSummaryInformationStream();

private:
	IStream* openStream		(const std::wstring & stream_name); // Opens a stream in the storage (shall be called not more than once per stream)
	IStream* createStream	(const std::wstring & stream_name); // Creates a new stream in the storage

	CFStreamPtr getNamedStream		(const std::wstring& name);
	CFStreamPtr createNamedStream	(const std::wstring& name);
	void closeNamedStream			(const std::wstring& name);

private:
	std::map<std::wstring, CFStreamPtr> streams;
	IStorage* storage_;
	ReadWriteMode rwMode;
};

} // namespace XLS

