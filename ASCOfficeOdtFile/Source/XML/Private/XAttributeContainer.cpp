
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XAttributeContainer.h"
#include "./../XName.h"
#include <boost/foreach.hpp>
#include <algorithm>
#include <boost/bind.hpp>
#include "NullXAttribute.h"


namespace XML
{
	namespace Private
	{
		static NullXAttribute nullXAttribute;


		const bool XAttributeContainer::exist(const XName& xname) const
		{
			//BOOST_FOREACH(const XAttribute& current, m_container)
			for(const_iterator current = begin(); current != end(); current++)
			{
				if ((*current)->XName->Equal(xname))
					return true;
			}
			//BOOST_FOREACH(const XAttribute& current, m_container)
			for(const_iterator current = begin(); current != end(); current++)
			{
				if ((*current)->XName == xname)
					return true;
			}
			return false;
		}


		XAttribute& XAttributeContainer::operator[] (const XName& xname)
		{
			//BOOST_FOREACH(XAttribute& current, m_container)
			for(iterator current = begin(); current != end(); current++)
			{
				if ((*current)->XName->Equal(xname))
					return *current;
			}
			//BOOST_FOREACH(XAttribute& current, m_container)
			for(iterator current = begin(); current != end(); current++)
			{
				if ((*current)->XName == xname)
					return *current;
			}
			return nullXAttribute;
		}


		const XAttribute& XAttributeContainer::operator[] (const XName& xname) const
		{
			//BOOST_FOREACH(const XAttribute& current, m_container)
			for(const_iterator current = begin(); current != end(); current++)
			{
				if ((*current)->XName->Equal(xname))
					return *current;
			}
			//BOOST_FOREACH(const XAttribute& current, m_container)
			for(const_iterator current = begin(); current != end(); current++)
			{
				if ((*current)->XName == xname)
					return *current;
			}
			return nullXAttribute;
		}


		const XNamespaceContainer XAttributeContainer::usedNamespace() const
		{
			XNamespaceContainer container;

			//BOOST_FOREACH(const XAttribute& current, m_container)
			//{
			//	if (current->XName->Ns.is_init())
			//		container.Add(current->XName->Ns);
			//}
			//for(std::set<XAttribute>::const_iterator current = m_container.begin(); current != m_container.end(); current++)
			//for(std::list<XAttribute>::const_iterator current = m_container.begin(); current != m_container.end(); current++)
			for(const_iterator current = begin(); current != end(); current++)
			{
				if ((*current)->XName->Ns.is_init())
					container.Add((*current)->XName->Ns);
			}

			return container;
		}


		XAttributeContainer::iterator XAttributeContainer:: begin()
		{
			return m_container.begin();
		}


		XAttributeContainer::iterator XAttributeContainer::end()
		{
			return m_container.end();
		}


		XAttributeContainer::const_iterator XAttributeContainer::begin() const
		{
			return m_container.begin();
		}


		XAttributeContainer::const_iterator XAttributeContainer::end() const
		{
			return m_container.end();
		}

	} // namespace Private
} // namespace XML