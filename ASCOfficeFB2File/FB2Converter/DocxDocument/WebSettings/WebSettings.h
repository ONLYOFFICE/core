#pragma once
#include <string>
#include "./../File.h"

namespace FB2Converter
{
	class WebSettings : public File
	{
	public:
		WebSettings(const CString filepath);
		virtual ~WebSettings();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

	private:
		CString		m_filepath;
	};
}