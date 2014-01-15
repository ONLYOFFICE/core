#pragma once
#ifndef XML_PRIVATE_XWFILE_SOURCE_INCLUDE_H_
#define XML_PRIVATE_XWFILE_SOURCE_INCLUDE_H_

#include "XWideSource.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <iterator>


namespace XML
{
	namespace Private
	{
		class XWFileSource : public XWideSource
		{
		public:
			XWFileSource(const boost::filesystem::path& path, XWideSource::eSourceType type);
			XWFileSource(const boost::filesystem::wpath& path, XWideSource::eSourceType type);
			virtual ~XWFileSource();

		private:
			void setBegin();

		private:
			std::wifstream										m_stream;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XWFILE_SOURCE_INCLUDE_H_