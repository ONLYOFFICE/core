
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XNamespaceContainer.h"
#include "NullXNamespace.h"


namespace XML
{
	namespace Private
	{
		static NullXNamespace nullXNamespace;

		const bool XNamespaceContainer::exist(const std::string& name) const
		{
			for (std::list<XNamespace>::const_iterator current = m_container.begin(); current != m_container.end(); ++current)
			{
				if ((*current)->GetPrefix() == name)
					return true;
			}
		
			return false;
		}

		XNamespace& XNamespaceContainer::operator[] (const std::string& name)
		{
			for (std::list<XNamespace>::iterator current = m_container.begin(); current != m_container.end(); ++current)
			{
				if ((*current)->GetPrefix() == name)
					return (*current);
			}
			return nullXNamespace;
		}

		const XNamespace& XNamespaceContainer::operator[] (const std::string& name) const
		{
			for (std::list<XNamespace>::const_iterator current = m_container.begin(); current != m_container.end(); ++current)
			{
				if ((*current)->GetPrefix() == name)
					return (*current);
			}
		
			return nullXNamespace;
		}

		void XNamespaceContainer::erase(const std::string& name)
		{
			for (std::list<XNamespace>::iterator i = m_container.begin(); i != m_container.end(); ++i)
			{
				if ((*i)->GetPrefix() == name)
				{
					m_container.erase(i);
					return;
				}
			}
		}

		void XNamespaceContainer::erase(const XNamespaceContainer& rhs)
		{
			for (std::list<XNamespace>::iterator current = m_container.begin(); current != m_container.end(); ++current)
			{
				erase((*current)->GetPrefix());
			}
		}

		XNamespaceContainer::iterator XNamespaceContainer:: begin()
		{
			return m_container.begin();
		}

		XNamespaceContainer::iterator XNamespaceContainer::end()
		{
			return m_container.end();
		}

		XNamespaceContainer::const_iterator XNamespaceContainer::begin() const
		{
			return m_container.begin();
		}

		XNamespaceContainer::const_iterator XNamespaceContainer::end() const
		{
			return m_container.end();
		}

	} // namespace Private
} // namespace XML