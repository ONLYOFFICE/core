#pragma once
#ifndef ODT_FILE_INCLUDE_H_
#define ODT_FILE_INCLUDE_H_

#include "NamespaceOwn.h"
#include <string>
#include <boost_filesystem_version.h>


namespace Odt
{
	class File : public NamespaceOwn
	{
	public:
		File();
		virtual ~File();

	public:
		virtual void read(const boost::filesystem::wpath& path) = 0;
		virtual void write(const boost::filesystem::wpath& path) const = 0;
		virtual const bool isValid(const boost::filesystem::wpath& path) const = 0;
	};
} // namespace Odt

#endif // ODT_FILE_INCLUDE_H_