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
		Video(const OOX::CPath& filename);
		virtual ~Video();

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_VIDEO_INCLUDE_H_