#ifndef XML_PRIVATE_XNODE_CONTAINER_INCLUDE_H_
#define XML_PRIVATE_XNODE_CONTAINER_INCLUDE_H_

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <list>


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
			//typedef std::vector<XNode>::iterator iterator;
			//typedef std::vector<XNode>::const_iterator const_iterator;
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
			//const boost::shared_ptr<std::vector<XNode> >	container() const;
			const boost::shared_ptr<std::list<XNode> >	container() const;

		private:
			//boost::shared_ptr<std::vector<XNode> >	m_container;
			boost::shared_ptr<std::list<XNode> >	m_container;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XNODE_CONTAINER_INCLUDE_H_