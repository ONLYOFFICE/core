
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XElementContainer.h"
#include "XNodeContainer.h"
#include "./../XNode.h"
#include "./../XElement.h"
#include "./../XName.h"
#include "NullXElement.h"

namespace XML
{
	namespace Private
	{
		static NullXElement nullXElement;

		XElementContainer::XElementContainer(const XNodeContainer& nodes) : m_container(nodes.container())
		{
		}

		const bool XElementContainer::empty() const
		{
			return size() == 0;
		}

		const size_t XElementContainer::size() const
		{
			size_t size = 0;
			for (const_iterator i = begin(); i != end(); ++i)
				++size;
			return size;
		}

		void XElementContainer::push_back(const XElement& element)
		{
			if (element.is_init() && element.exist())
			{
				m_container->push_back(element);
			}
		}

		void XElementContainer::Add(const XElement& element)
		{
			push_back(element);
		}

		const bool XElementContainer::exist(const XName& xname) const
		{
			//for (const_iterator i = begin(); i != end(); ++i)
			//{
			for (std::list<XNode>::iterator i = m_container->begin(); i != m_container->end(); ++i)
			{
				if(i->isElement())
				{
					if (XElement(*i)->XName == xname)
						return true;
				}
			}
			return false;
		}

		XElement XElementContainer::operator[] (const XName& xname)
		{
			//for (iterator i = begin(); i != end(); ++i)
			for (std::list<XNode>::iterator i = m_container->begin(); i != m_container->end(); ++i)
			{
				if(i->isElement())
				{
					if (XElement(*i)->XName->Equal(xname))
						return *i;
				}
			}
			//for (iterator i = begin(); i != end(); ++i)
			for (std::list<XNode>::iterator i = m_container->begin(); i != m_container->end(); ++i)
			{
				if(i->isElement())
				{
					if (XElement(*i)->XName == xname)
						return *i;
				}
			}
			return nullXElement;
		}

		const XElement XElementContainer::operator[] (const XName& xname) const
		{
			//for (const_iterator i = begin(); i != end(); ++i)
			//{
			for (std::list<XNode>::iterator i = m_container->begin(); i != m_container->end(); ++i)
			{
				if(i->isElement())
				{
					if (XElement(*i)->XName->Equal(xname))
						return *i;
				}
			}
			//for (const_iterator i = begin(); i != end(); ++i)
			//{
			for (std::list<XNode>::iterator i = m_container->begin(); i != m_container->end(); ++i)
			{
				if(i->isElement())
				{
					if (XElement(*i)->XName == xname)
						return *i;
				}
			}
			return nullXElement;
		}

		XElement XElementContainer::get(const XName& xname, const std::string& value)
		{
			for (iterator i = begin(); i != end(); ++i)
			{
				XElement element(*i);
				if (element.Attributes.exist(xname) && element.Attributes[xname].value().ToString() == value)
					return *i;
			}
			return nullXElement;
		}

		const XElement XElementContainer::get(const XName& xname, const std::string& value) const
		{
			for (const_iterator i = begin(); i != end(); ++i)
			{
				XElement element(*i);
				if (element.Attributes.exist(xname) && element.Attributes[xname].value().ToString() == value)
					return *i;
			}
			return nullXElement;
		}

		XElementContainer::iterator XElementContainer:: begin()
		{
			return iterator(m_container->begin(), m_container->end());
		}

		XElementContainer::iterator XElementContainer::end()
		{
			return iterator(m_container->end(), m_container->end());
		}

		XElementContainer::const_iterator XElementContainer::begin() const
		{
			return const_iterator(m_container->begin(), m_container->end());
		}

		XElementContainer::const_iterator XElementContainer::end() const
		{
			return const_iterator(m_container->end(), m_container->end());
		}

		const XNamespaceContainer XElementContainer::usedNamespace() const
		{
			XNamespaceContainer container;

			for (const_iterator i = begin(); i != end(); ++i)
				container.merge(XElement(*i)->usedNamespace());

			return container;
		}

	} // namespace Private
} // namespace XML