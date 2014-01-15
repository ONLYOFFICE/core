#pragma once
#ifndef OOX_FILE_INCLUDE_H_
#define OOX_FILE_INCLUDE_H_

#include "NamespaceOwn.h"
#include <string>
#include <boost/filesystem.hpp>
#include "ContentTypes/File.h"
#include "FileType.h"


namespace OOX
{
	class File : public NamespaceOwn
	{
	public:
		File();
		virtual ~File();

	public:
		virtual void read(const boost::filesystem::wpath& filename) = 0;
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const = 0;

	public:
		virtual const OOX::FileType type() const = 0;
		virtual const boost::filesystem::wpath DefaultDirectory() const = 0;
		virtual const boost::filesystem::wpath DefaultFileName() const = 0;
	};
} // namespace OOX

#endif // OOX_FILE_INCLUDE_H_