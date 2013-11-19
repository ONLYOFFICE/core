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
		Image(const boost::filesystem::wpath& filename);
		virtual ~Image();

	public:
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_IMAGE_INCLUDE_H_