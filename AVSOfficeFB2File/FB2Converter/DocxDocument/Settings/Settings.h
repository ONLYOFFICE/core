#pragma once
#include <string>
#include "./../File.h"

namespace FB2Converter
{
	class Settings : public File
	{
	public:
		Settings(const CString filepath);
		virtual ~Settings();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

	private:
		CString		m_filepath;
	};
}