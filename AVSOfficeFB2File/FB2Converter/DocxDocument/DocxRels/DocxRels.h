#pragma once
#include <string>
#include "./../File.h"

namespace FB2Converter
{
	class DocxRels : public File
	{
	public:
		DocxRels(const CString filepath);
		virtual ~DocxRels();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

	private:
		CString		m_filepath;
	};
}