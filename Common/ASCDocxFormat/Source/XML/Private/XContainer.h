#pragma once
#ifndef XML_PRIVATE_XCONTAINER_INCLUDE_H_
#define XML_PRIVATE_XCONTAINER_INCLUDE_H_

//#include <vector>
#include <boost/foreach.hpp>
#include <algorithm>
//#include <boost/bind.hpp>
#include <string>
#include <list>
//#include <set>


namespace XML
{
	namespace Private
	{
		template<class T>
		class XContainer
		{
		public:
			void push_back(const T& value);
			void Add(const T& value);
			const bool content(const T& other) const;
			void merge(const XContainer<T>& other);

			const size_t size() const;
			const bool empty() const;

			XContainer<T> const* const	operator->() const	{return this;}
			XContainer<T>*							operator->()				{return this;}

		public:
			const std::string ToString() const;
			const std::wstring ToWString() const;
			virtual void SaveToStringList(std::list<std::string>& strList)const;
			virtual void SaveToWStringList(std::list<std::wstring>& strList)const;

		protected:
			std::list<T> m_container;
			//std::set<T> m_container;
		};


		template<class T>
		void XContainer<T>::push_back(const T& value)
		{
			if (value.is_init() && !content(value))
				/*this->*/m_container.push_back(value);
				//m_container.insert(value);
		}


		template<class T>
		void XContainer<T>::Add(const T& value)
		{
			push_back(value);
		}


		template<class T>
		const bool XContainer<T>::content(const T& other) const
		{
			//BOOST_FOREACH(const T& current, m_container)
			//{
			//	if (*current == *other)
			//		return true;
			//}
			//return false;
			for(std::list<T>::const_iterator current = m_container.begin(); current != m_container.end(); current++)
			{
				if(*(*current) == *other)
					return true;
			}
			return false;
			//return (m_container.find(other) != m_container.end());
		}


		template<class T>
		void XContainer<T>::merge(const XContainer<T>& other)
		{
			//std::for_each(other.m_container.begin(), other.m_container.end(), 
			//	boost::bind(&XContainer<T>::push_back, this, _1));
			//for(std::set<T>::const_iterator current = other.m_container.begin(); current != other.m_container.end(); current++)
			for(std::list<T>::const_iterator current = other.m_container.begin(); current != other.m_container.end(); current++)
				push_back(*current);
		}


		template<class T>
		const size_t XContainer<T>::size() const
		{
			return m_container.size();
		}


		template<class T>
		const bool XContainer<T>::empty() const 
		{
			return m_container.empty();
		}


		template<class T>
		const std::string XContainer<T>::ToString() const
		{
			std::string result;
			BOOST_FOREACH(const T& current, m_container)
			{
				result += " ";
				result += current->ToString();
			}
			return result;
		}


		template<class T>
		const std::wstring XContainer<T>::ToWString() const
		{
			std::wstring result;
			BOOST_FOREACH(const T& current, m_container)
			{
				result += L" ";
				result += current->ToWString();
			}
			return result;
		}


		template<class T>
		void XContainer<T>::SaveToStringList(std::list<std::string>& strList)const
		{
			//BOOST_FOREACH(const T& current, m_container)
			//for(std::set<T>::const_iterator current = m_container.begin(); current != m_container.end(); current++)
			for(std::list<T>::const_iterator current = m_container.begin(); current != m_container.end(); current++)
			{
				strList.push_back(" ");
				(*current)->SaveToStringList(strList);
			}
		}


		template<class T>
		void XContainer<T>::SaveToWStringList(std::list<std::wstring>& strList)const
		{
			//BOOST_FOREACH(const T& current, m_container)
			//for(std::set<T>::const_iterator current = m_container.begin(); current != m_container.end(); current++)
			for(std::list<T>::const_iterator current = m_container.begin(); current != m_container.end(); current++)
			{
				strList.push_back(L" ");
				(*current)->SaveToWStringList(strList);
			}
		}

	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XCONTAINER_INCLUDE_H_