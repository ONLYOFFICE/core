#pragma once
#ifndef OOX_VIDEO_INCLUDE_H_
#define OOX_VIDEO_INCLUDE_H_

#include "Media.h"


namespace OOX
{
	class Video : public Media
	{
	public:
		Video();
		Video(const boost::filesystem::wpath& filename);
		virtual ~Video();

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_VIDEO_INCLUDE_H_