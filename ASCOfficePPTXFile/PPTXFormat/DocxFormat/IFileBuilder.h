#pragma once
#ifndef OOX_FILE_BUILDER_INCLUDE_H_
#define OOX_FILE_BUILDER_INCLUDE_H_

#include "ContentTypes/File.h"


namespace PPTX
{
	class IFileBuilder
	{
	public:
        IFileBuilder() {}
        virtual ~IFileBuilder() {}

	public:
	    virtual void Commit(const OOX::CPath& path)															= 0;
		virtual void Finalize(const OOX::CPath& path, const OOX::CPath& directory, ContentTypes::File& content)	= 0;
	};
} // namespace OOX

#endif // OOX_FILE_BUILDER_INCLUDE_H_
