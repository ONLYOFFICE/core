
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XTextContainer.h"
#include "XNodeContainer.h"
#include "./../XText.h"

namespace XML
{
	namespace Private
	{
		XTextContainer::XTextContainer(const XNodeContainer& nodes)	: m_container(nodes.container())
		{
		}

		const bool XTextContainer::empty() const
		{
			return size() == 0;
		}

		const size_t XTextContainer::size() const
		{
			size_t size = 0;
			for (const_iterator i = begin(); i != end(); ++i)
				++size;
			return size;
		}

		void XTextContainer::push_back(const XText& text)
		{
			if (text.is_init())
			{
				m_container->push_back(text);
			}
		}

		void XTextContainer::Add(const XML::XText& text)
		{
			push_back(text);
		}

		const XString XTextContainer::text() const
		{
			std::string text;
			for (const_iterator i = begin(); i != end(); ++i)
			{
				text += XText(*i)->Value;
			}
			return text.empty() ? XString() : text;
		}

		XTextContainer::iterator XTextContainer:: begin()
		{
			return m_container->begin();
		}

		XTextContainer::iterator XTextContainer::end()
		{
			return m_container->end();
		}

		XTextContainer::const_iterator XTextContainer::begin() const
		{
			return m_container->begin();
		}

		XTextContainer::const_iterator XTextContainer::end() const
		{
			return m_container->end();
		}

	} // namespace Private
} // namespace XML