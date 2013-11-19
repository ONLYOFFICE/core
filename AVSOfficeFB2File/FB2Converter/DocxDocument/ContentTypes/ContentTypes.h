#pragma once
#include <string>
#include "./../File.h"

namespace FB2Converter
{
	class ContentTypes : public File
	{
	public:
		ContentTypes(const CString filepath);
		virtual ~ContentTypes();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

	private:
		CString		m_filepath;
	};
}