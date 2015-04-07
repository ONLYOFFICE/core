#pragma once
#ifndef OOX_FILE_BUILDER_INCLUDE_H_
#define OOX_FILE_BUILDER_INCLUDE_H_

#include "ContentTypes.h"

namespace OOX
{
	class IFileBuilder
	{
	public:
		IFileBuilder() {}
		virtual ~IFileBuilder() {}

	public:
	    virtual void Commit  (const CPath& oPath)														 = 0;
		virtual void Finalize(const CPath& oPath, const CPath& oDirectory, OOX::CContentTypes& oContent) = 0;
	};
} // namespace OOX

#endif // OOX_FILE_BUILDER_INCLUDE_H_