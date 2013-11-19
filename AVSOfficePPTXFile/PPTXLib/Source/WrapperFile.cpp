#include "WrapperFile.h"
#include "Exception/not_implement.h"

namespace PPTX
{
	WrapperFile::WrapperFile()
	{
		m_written = false;
		m_WrittenFileName = L"";
	}

	WrapperFile::~WrapperFile()
	{
	}

	void WrapperFile::read(const boost::filesystem::wpath& filename)
	{
		throw not_implement("read(filename) in PPTX::WrapperFile");
	}

	bool WrapperFile::GetWrittenStatus()const
	{
		return m_written;
	}

	void WrapperFile::WrittenSetFalse()
	{
		m_written = false;
		m_WrittenFileName = L"";
	}

	const boost::filesystem::wpath WrapperFile::GetWrittenFileName()const
	{
		return m_WrittenFileName;
	}
} // namespace PPTX
