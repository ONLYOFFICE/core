#pragma once
#include <string>
#include "./../File.h"

namespace FB2Converter
{
	class App : public File
	{
	public:
		App(const CString filepath);
		virtual ~App();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

	private:
		CString		m_filepath;
	};
}