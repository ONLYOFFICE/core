#pragma once
#ifndef XML_PRIVATE_XWSTRING_SOURCE_INCLUDE_H_
#define XML_PRIVATE_XWSTRING_SOURCE_INCLUDE_H_

#include "XWideSource.h"
#include <sstream>
#include <iterator>


namespace XML
{
	namespace Private
	{
		class XWStringSource : public XWideSource
		{
		public:
			XWStringSource(const std::wstring& source);
			virtual ~XWStringSource();

		private:
			void setBegin();

		private:
			std::wistringstream								m_stream;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XWSTRING_SOURCE_INCLUDE_H_