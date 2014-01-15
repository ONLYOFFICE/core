#pragma once
#ifndef XML_PRIVATE_XPOINTER_INCLUDE_H_
#define XML_PRIVATE_XPOINTER_INCLUDE_H_

#include <boost/shared_ptr.hpp>


namespace XML
{
	namespace Private
	{
		template<class T>
		class XPointer
		{
		public:
			XPointer() {}
			XPointer(T* ptr) : m_ptr(ptr) {}
			XPointer(const boost::shared_ptr<T>& ptr) : m_ptr(ptr) {}

			template<class E>
			XPointer(const XPointer<E>& rhs) : m_ptr(boost::dynamic_pointer_cast<T>(rhs.get_ptr())) {}

			boost::shared_ptr<T> get_ptr() const {return m_ptr;}

		public:
			T const* const	operator->() const	{return m_ptr.get();}
			T*							operator->()				{return m_ptr.get();}

			const T&	operator*() const {return *m_ptr;}
			T&				operator*()				{return *m_ptr;}

			const bool is_init() const {return m_ptr != 0;}

		protected:
			boost::shared_ptr<T> m_ptr;

		protected:
			typedef XPointer<T> base;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XPOINTER_INCLUDE_H_