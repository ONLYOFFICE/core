#pragma once
#include <string>
#include "./../File.h"

namespace FB2Converter
{
	class Core : public File
	{
	public:
		Core(const CString filepath);
		virtual ~Core();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

	private:
		CString		m_filepath;
	};
}