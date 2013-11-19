#pragma once

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
	CompoundFile(const _bstr_t& file_path, const ReadWriteMode mode);
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
	IStream* openStream(const _bstr_t& stream_name); // Opens a stream in the storage (shall be called not more than once per stream)
	IStream* createStream(const _bstr_t& stream_name); // Creates a new stream in the storage

	CFStreamPtr getNamedStream(const std::string& name);
	CFStreamPtr createNamedStream(const std::string& name);
	void closeNamedStream(const std::string& name);

private:
	std::map<std::string, CFStreamPtr> streams;
	IStorage* storage_;
	ReadWriteMode rwMode;
};

} // namespace XLS

