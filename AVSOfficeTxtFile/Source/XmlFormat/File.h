#pragma once
#ifndef XML_FORMAT_FILE_INCLUDE_H_
#define XML_FORMAT_FILE_INCLUDE_H_

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include "XML.h"


namespace XmlFormat
{
	class File
	{
	public:
		File();

	public:
		void read(const boost::filesystem::wpath& filename, bool space);
		void write(const boost::filesystem::wpath& filename) const;

	public:
		const bool isValid(const boost::filesystem::wpath& filename) const;

	public:
		boost::shared_ptr<XML::XDocument> Document;
	};
} // namespace XmlFormat

#endif // XML_FORMAT_FILE_INCLUDE_H_