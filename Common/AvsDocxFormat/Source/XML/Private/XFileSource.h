#pragma once
#ifndef XML_PRIVATE_XFILE_SOURCE_INCLUDE_H_
#define XML_PRIVATE_XFILE_SOURCE_INCLUDE_H_

#include "XSingleSource.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <iterator>


namespace XML
{
	namespace Private
	{
		class XFileSource : public XSingleSource
		{
		public:
			XFileSource(const boost::filesystem::path& path);
			XFileSource(const boost::filesystem::wpath& path);
			virtual ~XFileSource();

		private:
			void setBegin();

		private:
			std::ifstream		m_stream;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XFILE_SOURCE_INCLUDE_H_