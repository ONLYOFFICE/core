#pragma once
#ifndef OOX_VIDEO_INCLUDE_H_
#define OOX_VIDEO_INCLUDE_H_

#include "Media.h"


namespace OOX
{
	class Video : public Media
	{
	public:
		Video()
		{
		}
		Video(const CPath& filename)
		{
			read(filename);
		}
		virtual ~Video()
		{
		}

	public:
		virtual const FileType type() const
		{
			return FileTypes::Video;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return m_filename.GetFilename();
		}
	};
} // namespace OOX

#endif // OOX_VIDEO_INCLUDE_H_