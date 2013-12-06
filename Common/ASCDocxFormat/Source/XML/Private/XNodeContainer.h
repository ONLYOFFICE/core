#ifndef XML_PRIVATE_XNODE_CONTAINER_INCLUDE_H_
#define XML_PRIVATE_XNODE_CONTAINER_INCLUDE_H_

#include <vector>
#include <string>
#include <list>

#include "../../../../../Common/DocxFormat/Source/Base/SmartPtr.h"

namespace XML
{
	class XNode;

	namespace Private
	{
		class XNodeContainer
		{
		public:
			XNodeContainer();

		public:
			const bool empty() const;
			const size_t size() const;
			void Add(const XNode& node);
			void push_back(const XNode& node);
			void merge(const XNodeContainer& other);

		public:
			typedef std::list<XNode>::iterator iterator;
			typedef std::list<XNode>::const_iterator const_iterator;

		public:
			XNodeContainer const* const	operator->() const	{return this;}
			XNodeContainer*							operator->()				{return this;}

		public:
			const std::string ToString() const;
			const std::wstring ToWString() const;
			virtual void SaveToStringList(std::list<std::string>& strList)const;
			virtual void SaveToWStringList(std::list<std::wstring>& strList)const;

		public:
			iterator begin()	{return m_container->begin();}
			iterator end()		{return m_container->end();}
			const_iterator begin()	const	{return m_container->begin();}
			const_iterator end()		const	{return m_container->end();}

		public:
			const NSCommon::smart_ptr<std::list<XNode> >	container() const;

		private:
			NSCommon::smart_ptr<std::list<XNode> >	m_container;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XNODE_CONTAINER_INCLUDE_H_