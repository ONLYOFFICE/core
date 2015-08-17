#pragma once

#include "../../../Common/3dParty/pole/pole.h"

#include "BinSmartPointers.h"

#include <map>

namespace XLS
{


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
	void		closeSummaryInformationStream();

	CFStreamPtr getDocumentSummaryInformationStream();
	CFStreamPtr createDocumentSummaryInformationStream();
	void		closeDocumentSummaryInformationStream();

private:
	POLE::Stream* openStream		(const std::string & stream_name); // Opens a stream in the storage (shall be called not more than once per stream)
	POLE::Stream* createStream		(const std::string & stream_name); // Creates a new stream in the storage

	CFStreamPtr getNamedStream		(const std::string& name);
	CFStreamPtr createNamedStream	(const std::string& name);
	void		closeNamedStream	(const std::string& name);

private:
	std::map<std::string, CFStreamPtr>	streams;
	POLE::Storage						*storage_;
	ReadWriteMode						rwMode;
};

} // namespace XLS

