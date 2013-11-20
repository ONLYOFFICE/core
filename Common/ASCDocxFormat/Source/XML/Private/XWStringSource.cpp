
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XWStringSource.h"
#include "Exception\not_implement.h"


namespace XML
{
	namespace Private
	{

		XWStringSource::XWStringSource(const std::wstring& source)
			:	m_stream(source)
			, XWideSource(XWideSource::estStringSource)
		{
			setBegin();
		}


		XWStringSource::~XWStringSource()
		{
		}


		void XWStringSource::setBegin()
		{
			m_iterator = m_stream;
		}

	} // namespace Private
} // namespace XML