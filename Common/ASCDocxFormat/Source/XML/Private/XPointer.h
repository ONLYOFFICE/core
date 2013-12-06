#pragma once
#ifndef XML_PRIVATE_XPOINTER_INCLUDE_H_
#define XML_PRIVATE_XPOINTER_INCLUDE_H_

#include "../../../../../Common/DocxFormat/Source/Base/SmartPtr.h"

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
			XPointer(const NSCommon::smart_ptr<T>& ptr) : m_ptr(ptr) {}

			template<class E>
			XPointer(const XPointer<E>& rhs)
			{
				m_ptr = rhs.get_ptr().smart_dynamic_cast<T>();
			}

			NSCommon::smart_ptr<T> get_ptr() const {return m_ptr;}

		public:
			T const* const	operator->() const	{return m_ptr.operator->();}	
			T*				operator->()		{return m_ptr.operator->();}		

			const T&	operator*() const	{return (m_ptr.operator*());}
			T&			operator*()			{return (m_ptr.operator*());}

			const bool is_init() const {return m_ptr.is_init();}

		protected:
			NSCommon::smart_ptr<T> m_ptr;

		protected:
			typedef XPointer<T> base;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XPOINTER_INCLUDE_H_