#pragma once
#ifndef OOX_IMAGE_INCLUDE_H_
#define OOX_IMAGE_INCLUDE_H_

#include "Media.h"

namespace OOX
{
	class Image : public Media
	{
	public:
		Image();
		Image(const OOX::CPath& filename);
		virtual ~Image();

	public:
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_IMAGE_INCLUDE_H_