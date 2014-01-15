
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XStringSource.h"
#include "Exception\not_implement.h"


namespace XML
{
	namespace Private
	{

		XStringSource::XStringSource(const std::string& source)
			: m_stream(source)
		{
			setBegin();
		}


		XStringSource::~XStringSource()
		{
		}


		void XStringSource::setBegin()
		{
			m_iterator = m_stream;
		}

	} // namespace Prviate
} // namespace XML