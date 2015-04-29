#pragma once
#ifndef TXT_FILE_INCLUDE_H_
#define TXT_FILE_INCLUDE_H_

#include <list>
#include <string>

namespace Txt
{
	class File
	{
	public:
		File();
		~File();

        void read			(const std::wstring& filename);
        void read			(const std::wstring& filename, int code_page);
		
        void write			(const std::wstring& filename) const;
		
        void writeCodePage	(const std::wstring& filename, int code_page) const;
        void writeUtf8		(const std::wstring& filename) const;
        void writeUnicode	(const std::wstring& filename) const;
        void writeBigEndian	(const std::wstring& filename) const;
        void writeAnsi		(const std::wstring& filename) const;
		
        const bool isValid	(const std::wstring& filename) const;
		
		std::list<std::wstring>		m_listContent;			//unicode  (ранее было utf8)
		int							m_listContentSize;		//для вывода процентов конвертации
		int							m_nEncoding;
	private:
		void correctUnicode(std::list<std::wstring>& oList);
		bool IsUnicodeSymbol( wchar_t symbol );
	};
} // namespace Txt

#endif // TXT_FILE_INCLUDE_H_
