#pragma once
#ifndef OOX_VIDEO_INCLUDE_H_
#define OOX_VIDEO_INCLUDE_H_

#include "Media.h"


namespace PPTX
{
	class Video : public Media
	{
	public:
		Video()
		{
		}
		Video(const OOX::CPath& filename)
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
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return m_filename.GetFilename();
		}
	};
} // namespace PPTX

#endif // OOX_VIDEO_INCLUDE_H_