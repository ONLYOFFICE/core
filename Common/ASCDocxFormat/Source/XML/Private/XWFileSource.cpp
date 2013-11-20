
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XWFileSource.h"
#include "Exception\not_implement.h"


namespace XML
{
	namespace Private
	{

		XWFileSource::XWFileSource(const boost::filesystem::path& path, XWideSource::eSourceType type)
			: m_stream(path.string().c_str())
			, XWideSource(type)
		{
			setBegin();
		}


		XWFileSource::XWFileSource(const boost::filesystem::wpath& path, XWideSource::eSourceType type)
			: m_stream(path.string().c_str())
			, XWideSource(type)
		{
			setBegin();
		}


		XWFileSource::~XWFileSource()
		{
		}


		void XWFileSource::setBegin()
		{
			m_iterator = m_stream;
		}


	} // namespace Private
} // namespace XML