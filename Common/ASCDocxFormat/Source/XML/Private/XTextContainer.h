#pragma once
#ifndef XML_PRIVATE_XTEXT_CONTAINER_INCLUDE_H_
#define XML_PRIVATE_XTEXT_CONTAINER_INCLUDE_H_

#include <vector>
#include <list>

#include "Filter/Text.h"
#include "XString.h"

#include "../../../../../Common/DocxFormat/Source/Base/SmartPtr.h"

namespace XML
{
	namespace Private
	{
		class XNodeContainer;

		class XTextContainer
		{
		public:
			XTextContainer(const XNodeContainer& nodes);

		public:
			const bool empty() const;
			const size_t size() const;
			void push_back(const XText& text);
			void Add(const XText& text);

			const XString text() const;

		public:
		
			typedef std::list<XNode>::iterator iterator;
			typedef std::list<XNode>::const_iterator const_iterator;

		public:
			XTextContainer const* const	operator->() const	{return this;}
			XTextContainer*				operator->()		{return this;}

		public:
			iterator begin();
			iterator end();
			const_iterator begin() const;
			const_iterator end() const;

		private:
			NSCommon::smart_ptr<std::list<XNode> >	m_container;
		};
	} // namespace Private
} // namespace XML


#endif // XML_PRIVATE_XTEXT_CONTAINER_INCLUDE_H_