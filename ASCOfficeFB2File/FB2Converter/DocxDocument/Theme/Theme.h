#pragma once
#include <string>
#include "./../File.h"

namespace FB2Converter
{
	class Theme : public File
	{
	public:
		Theme(const CString filepath);
		virtual ~Theme();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

	private:
		CString		m_filepath;
	};
}