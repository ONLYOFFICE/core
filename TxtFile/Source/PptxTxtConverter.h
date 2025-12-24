#pragma once

#include <memory>
#include <string>

class CPptxTxtConverter
{
public:
	CPptxTxtConverter();
	CPptxTxtConverter(const CPptxTxtConverter& other) = delete;
	CPptxTxtConverter(CPptxTxtConverter&& other) = delete;
	virtual ~CPptxTxtConverter();

	// returns S_OK if convertation was successful
	int Convert(const std::wstring& wsSrcPptxDir, const std::wstring& wsDstTxtFile = L"./output.txt");

private:
	class CPptxTxtConverterImpl;
	std::unique_ptr<CPptxTxtConverterImpl> m_pImpl;
};

