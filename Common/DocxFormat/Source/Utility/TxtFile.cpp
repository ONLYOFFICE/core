#ifdef _HTML_FILE

#include "TxtFile.h"
#include "Encoding.h"
#include "Utility.h"
#include <boost/foreach.hpp>
#include "codecvt.h"
#include <streambuf>
#include <boost/algorithm/string/erase.hpp>

static const std::string BadSymbols = "\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19";

TxtFile::TxtFile(const boost::filesystem::wpath& path)
	: m_path(path)
{
}


const std::list<std::string> TxtFile::readAnsi() const
{
	std::ifstream file(m_path.string().c_str());
	if (file.bad())
		throw log_runtime_error("can't open text file");

	std::list<std::string> result;
	while (!file.eof())
	{
		std::string str;
		std::getline(file, str);

		size_t pos = str.find_first_of(BadSymbols);
		while(pos != str.npos)
		{
			str.erase(pos, 1);
			pos = str.find_first_of(BadSymbols, pos);
		}

/*
		boost::erase_all(str, "\x0A");
		boost::erase_all(str, "\x0B");
		boost::erase_all(str, "\x0C");
		boost::erase_all(str, "\x0D");
		boost::erase_all(str, "\x0E");
		boost::erase_all(str, "\x0F");
		boost::erase_all(str, "\x10");
		boost::erase_all(str, "\x11");
		boost::erase_all(str, "\x12");
		boost::erase_all(str, "\x13");
		boost::erase_all(str, "\x14");
		boost::erase_all(str, "\x15");
		boost::erase_all(str, "\x16");
		boost::erase_all(str, "\x17");
		boost::erase_all(str, "\x18");
		boost::erase_all(str, "\x19");
*/
		result.push_back(str);
	}
	return result;
}


const std::list<std::wstring> TxtFile::readUnicode() const
{
	std::wifstream file(m_path.string().c_str(), std::ios_base::binary);
	if (file.bad())
		throw log_runtime_error("can't open text file");

	file.imbue(std::locale(std::locale(), new ucs2_conversion()));
	std::list<std::wstring> result;

	while (!file.eof())
	{
		std::wstring str;
		file.ignore();
		std::getline(file, str, L'\x0D');
		result.push_back(str);
	}
	return result;
}


const std::list<std::wstring> TxtFile::readUnicodeWithOutBOM() const
{
	std::wifstream file(m_path.string().c_str(), std::ios_base::binary);
	if (file.bad())
		throw log_runtime_error("can't open text file");

	file.imbue(std::locale(std::locale(), new ucs2_conversion()));
	std::list<std::wstring> result;

	while (!file.eof())
	{
		std::wstring str;
		std::getline(file, str, L'\x0D');
		file.ignore();
		result.push_back(str);
	}
	return result;
}


const std::list<std::wstring> TxtFile::readBigEndian() const
{
	std::wifstream file(m_path.string().c_str(), std::ios_base::binary);
	if (file.bad())
		throw log_runtime_error("can't open text file");

	file.imbue(std::locale(std::locale(), new ube_conversion()));
	std::list<std::wstring> result;

	while (!file.eof())
	{
		std::wstring str;
		file.ignore();
		std::getline(file, str, L'\x0D');
		result.push_back(str);
	}
	return result;
}


const std::list<std::string> TxtFile::readUtf8() const
{
	std::ifstream file(m_path.string().c_str());
	if (file.bad())
		throw log_runtime_error("can't open text file");

	std::list<std::string> result;
	file.ignore(3);

	while (!file.eof())
	{
		std::string str;
		std::getline(file, str);
		result.push_back(str);
	}
	return result;
}


const std::list<std::string> TxtFile::readUtf8withoutPref() const
{
	std::ifstream file(m_path.string().c_str());
	if (file.bad())
		throw log_runtime_error("can't open text file");

	std::list<std::string> result;

	while (!file.eof())
	{
		std::string str;
		std::getline(file, str);
		result.push_back(str);
	}
	return result;
}


void TxtFile::writeAnsi(const std::list<std::string>& content) const
{
	setAnsiStamp();
	std::ofstream file(m_path.string().c_str());

	if (file.bad())
		throw log_runtime_error("can't create text file");

	BOOST_FOREACH(const std::string& line, content)
	{
		file << line << std::endl;
	}
}


void TxtFile::writeUnicode(const std::list<std::wstring>& content) const
{
	setUnicodeStamp();
	std::wofstream file(m_path.string().c_str(), std::ios_base::binary | std::ios_base::app);

	if (file.bad())
		throw log_runtime_error("can't create text file");

	file.imbue(std::locale(std::locale(), new ucs2_conversion()));
	BOOST_FOREACH(const std::wstring& line, content)
	{
		file << line << L'\x0D' << L'\x0A';
	}
}


void TxtFile::writeBigEndian(const std::list<std::wstring>& content) const
{
	setBigEndianStamp();
	std::wofstream file(m_path.string().c_str(), std::ios_base::binary | std::ios_base::app);

	if (file.bad())
		throw log_runtime_error("can't create text file");

	file.imbue(std::locale(std::locale(), new ube_conversion()));
	BOOST_FOREACH(const std::wstring& line, content)
	{
		file << line << L'\x0D' << L'\x0A';
	}
}


void TxtFile::writeUtf8(const std::list<std::string>& content) const
{
	setUtf8Stamp();
	std::ofstream file(m_path.string().c_str(), std::ios_base::app);

	if (file.bad())
		throw log_runtime_error("can't create text file");

	BOOST_FOREACH(const std::string& line, content)
	{
		file << line << std::endl;
	}
}


void TxtFile::writeUtf8withoutPref(const std::list<std::string>& content) const
{
	std::ofstream file(m_path.string().c_str());

	if (file.bad())
		throw log_runtime_error("can't create text file");

	BOOST_FOREACH(const std::string& line, content)
	{
		file << line << std::endl;
	}
}


const bool TxtFile::isAnsi() const
{
	return true;
}


const bool TxtFile::isUnicode() const
{
	std::ifstream file(m_path.string().c_str(), std::ios_base::binary);
	if (file.bad())
		throw log_runtime_error("can't open text file");

	char symbol;

	file >> symbol;
	if (symbol != '\xFF')
		return false;

	file >> symbol;
	if (symbol != '\xFE')
		return false;

	return true;
}


const bool TxtFile::isUnicodeWithOutBOM() const
{
	std::ifstream file(m_path.string().c_str(), std::ios_base::binary);
	if (file.bad())
		throw log_runtime_error("can't open text file");

	std::string str;
	std::getline(file, str, '\x0A');
	bool bUnicode = (str.find('\x0') != str.npos);

	return bUnicode;
}


const bool TxtFile::isBigEndian() const
{
	std::ifstream file(m_path.string().c_str(), std::ios_base::binary);
	if (file.bad())
		throw log_runtime_error("can't open text file");

	char symbol;

	file >> symbol;
	if (symbol != '\xFE')
		return false;

	file >> symbol;
	if (symbol != '\xFF')
		return false;

	return true;
}


const bool TxtFile::isUtf8() const
{
	std::ifstream file(m_path.string().c_str(), std::ios_base::binary);
	if (file.bad())
		throw log_runtime_error("can't open text file");

	char symbol;

	file >> symbol;
	if (symbol != '\xEF')
		return false;

	file >> symbol;
	if (symbol != '\xBB')
		return false;

	file >> symbol;
	if (symbol != '\xBF')
		return false;

	return true;
}


void TxtFile::setAnsiStamp() const
{
}


void TxtFile::setUnicodeStamp() const
{
	std::wofstream file(m_path.string().c_str(), std::ios_base::binary);
	file << '\xFF' << '\xFE';
}


void TxtFile::setBigEndianStamp() const
{
	std::wofstream file(m_path.string().c_str(), std::ios_base::binary);
	file << '\xFE' << '\xFF';
}


void TxtFile::setUtf8Stamp() const
{
	std::wofstream file(m_path.string().c_str(), std::ios_base::binary);
	file << '\xEF' << '\xBB' << '\xBF';
}

#endif