#pragma once
#ifndef OOX_EXTERNALVIDEO_INCLUDE_H_
#define OOX_EXTERNALVIDEO_INCLUDE_H_

#include "External.h"


namespace OOX
{
	class ExternalVideo : public External
	{
	public:
		ExternalVideo()
		{
		}
		ExternalVideo(const CPath& uri)
		{
			read(uri);
		}
		~ExternalVideo()
		{
		}

	public:
		virtual const FileType type() const
		{
			return FileTypes::ExternalVideo;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
	};
} // namespace OOX

#endif // OOX_EXTERNALVIDEO_INCLUDE_H_