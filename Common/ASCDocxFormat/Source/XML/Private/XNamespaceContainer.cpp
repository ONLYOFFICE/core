
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
			BOOST_FOREACH(const XNamespace& current, m_container)
			{
				if (current->GetPrefix() == name)
					return true;
			}
			return false;
		}



		XNamespace& XNamespaceContainer::operator[] (const std::string& name)
		{
			BOOST_FOREACH(XNamespace& current, m_container)
			{
				if (current->GetPrefix() == name)
					return current;
			}
			return nullXNamespace;
		}


		const XNamespace& XNamespaceContainer::operator[] (const std::string& name) const
		{
			BOOST_FOREACH(const XNamespace& current, m_container)
			{
				if (current->GetPrefix() == name)
					return current;
			}
			return nullXNamespace;
		}


		void XNamespaceContainer::erase(const std::string& name)
		{
			//for (std::set<XNamespace>::iterator i = m_container.begin(); i != m_container.end(); ++i)
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
			BOOST_FOREACH(const XNamespace& current, rhs.m_container)
			{
				erase(current->GetPrefix());
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