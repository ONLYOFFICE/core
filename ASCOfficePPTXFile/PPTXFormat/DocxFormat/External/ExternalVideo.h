#pragma once
#ifndef OOX_EXTERNALVIDEO_INCLUDE_H_
#define OOX_EXTERNALVIDEO_INCLUDE_H_

#include "External.h"


namespace PPTX
{
	class ExternalVideo : public External
	{
	public:
		ExternalVideo()
		{
		}
		ExternalVideo(const OOX::CPath& uri)
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
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
	};
} // namespace PPTX

#endif // OOX_EXTERNALVIDEO_INCLUDE_H_