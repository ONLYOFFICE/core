#pragma once
#ifndef XML_PRIVATE_XWFILE_SOURCE_INCLUDE_H_
#define XML_PRIVATE_XWFILE_SOURCE_INCLUDE_H_

#include "XWideSource.h"
#include <fstream>
#include <iterator>

#include "../../../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"

namespace XML
{
	namespace Private
	{
		class XWFileSource : public XWideSource
		{
		public:
			XWFileSource(const OOX::CPath& path, XWideSource::eSourceType type);
			virtual ~XWFileSource();

		private:
			void setBegin();

		private:
			std::wifstream										m_stream;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XWFILE_SOURCE_INCLUDE_H_