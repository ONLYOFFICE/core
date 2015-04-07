#pragma once
#ifndef TXT_FILE_INCLUDE_H_
#define TXT_FILE_INCLUDE_H_

//#include <vector>
#include <list>
#include <string>
#include <boost/filesystem.hpp>
//#include "property.h"


namespace Txt
{
	class File
	{
	public:
		File();
		~File();

		void read			(const boost::filesystem::wpath& filename);
		void read			(const boost::filesystem::wpath& filename, int code_page);
		
		void write			(const boost::filesystem::wpath& filename) const;
		
		void writeCodePage	(const boost::filesystem::wpath& filename, int code_page) const;
		void writeUtf8		(const boost::filesystem::wpath& filename) const;
		void writeUnicode	(const boost::filesystem::wpath& filename) const;
		void writeBigEndian	(const boost::filesystem::wpath& filename) const;
		void writeAnsi		(const boost::filesystem::wpath& filename) const;
		
		const bool isValid	(const boost::filesystem::wpath& filename) const;
		
		std::list<std::wstring>		m_listContent;			//unicode  (ранее было utf8)
		int							m_listContentSize;		//для вывода процентов конвертации
		int							m_nEncoding;
	private:
		void correctUnicode(std::list<std::wstring>& oList);
		bool IsUnicodeSymbol( wchar_t symbol );
	};
} // namespace Txt

#endif // TXT_FILE_INCLUDE_H_