#pragma once
#ifndef XML_PRIVATE_XFILE_SOURCE_INCLUDE_H_
#define XML_PRIVATE_XFILE_SOURCE_INCLUDE_H_

#include "XSingleSource.h"
#include <fstream>
#include <iterator>

#include "../../../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"

namespace XML
{
	namespace Private
	{
		class XFileSource : public XSingleSource
		{
		public:
			XFileSource(const OOX::CPath& path);
			virtual ~XFileSource();

		private:
			void setBegin();

		private:
			std::ifstream		m_stream;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XFILE_SOURCE_INCLUDE_H_