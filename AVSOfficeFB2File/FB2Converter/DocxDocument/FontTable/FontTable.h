#pragma once
#include <string>
#include "./../File.h"

namespace FB2Converter
{
	class FontTable : public File
	{
	public:
		FontTable(const CString filepath);
		virtual ~FontTable();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

	private:
		CString		m_filepath;
	};
}