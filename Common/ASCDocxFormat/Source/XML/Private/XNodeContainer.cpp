
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XNodeContainer.h"
#include <algorithm>
#include <boost/bind.hpp>
#include <numeric>
#include "./../XNode.h"
#include "./../XContainer.h"


namespace XML
{
	namespace Private
	{

		XNodeContainer::XNodeContainer()
			: m_container(new std::list<XNode>())
		{
		}


		const bool XNodeContainer::empty() const
		{
			return m_container->empty();
		}


		const size_t XNodeContainer::size() const
		{
			return m_container->size();
		}


		void XNodeContainer::Add(const XNode& node)
		{
			push_back(node);
		}


		void XNodeContainer::push_back(const XNode& node)
		{
			if (node.is_init())
			{
				if (node.isElement() || node.isText() || node.isComment())
					m_container->push_back(node);
				else
					merge(XML::XContainer(node)->Nodes);
			}
		}


		void XNodeContainer::merge(const XNodeContainer& other)
		{
			for(std::list<XNode>::const_iterator current = other.m_container->begin(); current != other.m_container->end(); current++)
				push_back(*current);
		}


		const std::string XNodeContainer::ToString() const
		{
			return std::accumulate(m_container->begin(), m_container->end(), std::string(),
						boost::bind(std::plus<std::string>(), _1, boost::bind(&XNode::ToString, _2)));
		}


		const std::wstring XNodeContainer::ToWString() const
		{
			return std::accumulate(m_container->begin(), m_container->end(), std::wstring(),
						boost::bind(std::plus<std::wstring>(), _1, boost::bind(&XNode::ToWString, _2)));
		}


		void XNodeContainer::SaveToStringList(std::list<std::string>& strList)const
		{
			for(std::list<XNode>::const_iterator iter = m_container->begin(); iter != m_container->end(); iter++)
			{
				iter->SaveToStringList(strList);
			}
		}


		void XNodeContainer::SaveToWStringList(std::list<std::wstring>& strList)const
		{
			for(std::list<XNode>::const_iterator iter = m_container->begin(); iter != m_container->end(); iter++)
			{
				iter->SaveToWStringList(strList);
			}
		}


		const boost::shared_ptr<std::list<XNode> >	XNodeContainer::container() const
		{
			return m_container;
		}

	} // namespace Private
} // namespace XML