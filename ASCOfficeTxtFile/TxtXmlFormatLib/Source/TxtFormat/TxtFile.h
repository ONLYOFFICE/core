#pragma once
#ifndef UTILITY_TXT_FILE_INCLUDE_H_
#define UTILITY_TXT_FILE_INCLUDE_H_

#include <string>
#include <list>

#include "../../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"

class TxtFile
{
public:
	TxtFile(const std::wstring & path);

	const std::list<std::string>	readAnsiOrCodePage();
	const std::list<std::wstring>	readUnicode();
	//const std::list<std::wstring>	readUnicodeWithOutBOM(); /// не используем
	const std::list<std::wstring>	readBigEndian();
	const std::list<std::string>	readUtf8();

	void writeAnsiOrCodePage	(const std::list<std::string>& content);
	void writeUnicode			(const std::list<std::wstring>& content);
	void writeBigEndian			(const std::list<std::wstring>& content);
	void writeUtf8				(const std::list<std::string>& content);

	const bool	isUnicode();
	const bool	isBigEndian();
	const bool	isUtf8();

	const int	getLinesCount();

private:
	std::wstring	m_path;
	int				m_linesCount;
};

#endif // UTILITY_TXT_FILE_INCLUDE_H_