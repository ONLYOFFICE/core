#pragma once
#ifndef OOX_FILE_BUILDER_INCLUDE_H_
#define OOX_FILE_BUILDER_INCLUDE_H_

#include <boost/filesystem.hpp>
#include "ContentTypes/File.h"


namespace OOX
{
	class IFileBuilder
	{
	public:
		IFileBuilder();
		virtual ~IFileBuilder();

	public:
	    virtual void Commit(const boost::filesystem::wpath& path) = 0;
		virtual void Finalize(const boost::filesystem::wpath& path, const boost::filesystem::wpath& directory, ContentTypes::File& content) = 0;
	};
} // namespace OOX

#endif // OOX_FILE_BUILDER_INCLUDE_H_