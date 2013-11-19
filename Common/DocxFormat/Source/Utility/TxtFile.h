#pragma once
#ifndef UTILITY_TXT_FILE_INCLUDE_H_
#define UTILITY_TXT_FILE_INCLUDE_H_

#ifdef _HTML_FILE

#include <boost/filesystem.hpp>
#include <string>
#include <list>


class TxtFile
{
public:
	TxtFile(const boost::filesystem::wpath& path);

public:
	const std::list<std::string> readAnsi() const;
	const std::list<std::wstring> readUnicode() const;
	const std::list<std::wstring> readUnicodeWithOutBOM() const;
	const std::list<std::wstring> readBigEndian() const;
	const std::list<std::string> readUtf8() const;
	const std::list<std::string> readUtf8withoutPref() const;

public:
	void writeAnsi(const std::list<std::string>& content) const;
	void writeUnicode(const std::list<std::wstring>& content) const;
	void writeBigEndian(const std::list<std::wstring>& content) const;
	void writeUtf8(const std::list<std::string>& content) const;
	void writeUtf8withoutPref(const std::list<std::string>& content) const;

public:
	const bool isAnsi() const;
	const bool isUnicode() const;
	const bool isUnicodeWithOutBOM() const;
	const bool isBigEndian() const;
	const bool isUtf8() const;

private:
	void setAnsiStamp() const;
	void setUnicodeStamp() const;
	void setBigEndianStamp() const;
	void setUtf8Stamp() const;

private:
	boost::filesystem::wpath m_path;
};

#endif // _HTML_FILE

#endif // UTILITY_TXT_FILE_INCLUDE_H_