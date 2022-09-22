#pragma once

#include <string>
#include <boost/shared_ptr.hpp>

class CVbaFile;
typedef boost::shared_ptr<CVbaFile> CVbaFilePtr;

class CVbaReader
{
public:
	CVbaReader(const std::wstring & vbaFileName, const std::wstring & vbaExtractFile);
	~CVbaReader();

	const std::wstring convert();
	bool write();
private:
	std::wstring vbaExtractFile_;
	CVbaFilePtr vbaProject_file_;
};

