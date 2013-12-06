
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XContainer.h"
#include "XNode.h"
#include "XElement.h"
#include "XText.h"
#include "Private/XList.h"

namespace XML
{
	XContainer::XContainer()
		: base(new Private::NodeContainer())
	{
	}

	XContainer::XContainer(const XNode& xnode)
		: base(xnode.get_ptr().smart_dynamic_cast<Private::NodeContainer>())
	{
	}

	XContainer::XContainer(const XElement& element)
		: base(new Private::NodeContainer(element))
	{
	}

	XContainer::XContainer(const XText& text)
		: base(new Private::NodeContainer(text))
	{
	}

	XContainer::XContainer(const Private::XList& list)
		: base(new Private::NodeContainer(list))
	{
	}

	void XContainer::Add(const XNode& node)
	{
		m_ptr->Add(node);
	}

	void XContainer::Add(const XElement& element)
	{
		m_ptr->Add(element);
	}

	void XContainer::Add(const XText& text)
	{
		m_ptr->Add(text);
	}

	void XContainer::Add(const Private::XList& list)
	{
		m_ptr->Add(list);
	}

} // namespace XML