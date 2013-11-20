#pragma once
#ifndef XML_PRIVATE_XSTRING_SOURCE_INCLUDE_H_
#define XML_PRIVATE_XSTRING_SOURCE_INCLUDE_H_

#include "XSingleSource.h"
#include <sstream>
#include <iterator>


namespace XML
{
	namespace Private
	{
		class XStringSource : public XSingleSource
		{
		public:
			XStringSource(const std::string& source);
			virtual ~XStringSource();

		private:
			void setBegin();

		private:
			std::istringstream	m_stream;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XSTRING_SOURCE_INCLUDE_H_