
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XWFileSource.h"
#include "Exception\not_implement.h"


namespace XML
{
	namespace Private
	{
		XWFileSource::XWFileSource(const OOX::CPath& path, XWideSource::eSourceType type)
			: m_stream(path.GetPath())
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