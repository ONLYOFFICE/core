
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XFileSource.h"
#include "Exception\not_implement.h"


namespace XML
{
	namespace Private
	{
		XFileSource::XFileSource(const OOX::CPath& path)
			: m_stream(path.GetPath())
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