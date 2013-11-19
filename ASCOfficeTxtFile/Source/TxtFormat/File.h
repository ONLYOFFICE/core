#pragma once
#ifndef TXT_FILE_INCLUDE_H_
#define TXT_FILE_INCLUDE_H_

//#include <vector>
#include <list>
#include <string>
#include <boost/filesystem.hpp>
#include "property.h"


namespace Txt
{
	class File
	{
	public:
		File();

	public:
		void read(const boost::filesystem::wpath& filename);
		void write(const boost::filesystem::wpath& filename) const;

		void writeUtf8(const boost::filesystem::wpath& filename) const;
		void writeUnicode(const boost::filesystem::wpath& filename) const;
		void writeBigEndian(const boost::filesystem::wpath& filename) const;
		void writeAnsi(const boost::filesystem::wpath& filename) const;
		
	public:
		const bool isValid(const boost::filesystem::wpath& filename) const;
	private:
		std::list<std::wstring> correctUnicode(std::list<std::wstring>& oList);
		bool IsUnicodeSymbol( wchar_t symbol );
	public:
		//property<std::vector<std::string> >		Content;
		property<std::list<std::string> >		Content;
		int m_nEncoding;
	};
} // namespace Txt

#endif // TXT_FILE_INCLUDE_H_