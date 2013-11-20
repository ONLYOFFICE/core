
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XFileSource.h"
#include "Exception\not_implement.h"


namespace XML
{
	namespace Private
	{

		XFileSource::XFileSource(const boost::filesystem::path& path)
			: m_stream(path.string().c_str())
		{
			setBegin();
		}


		XFileSource::XFileSource(const boost::filesystem::wpath& path)
			: m_stream(path.string().c_str())
		{
			setBegin();
		}


		XFileSource::~XFileSource()
		{
		}


		void XFileSource::setBegin()
		{
			m_iterator = m_stream;
		}

	} // namespace Private
} // namespace XML