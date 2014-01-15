#pragma once
#ifndef OOX_EXTERNALVIDEO_INCLUDE_H_
#define OOX_EXTERNALVIDEO_INCLUDE_H_

#include "External.h"


namespace OOX
{
	class ExternalVideo : public External
	{
	public:
		ExternalVideo();
		ExternalVideo(const boost::filesystem::wpath& uri);
		~ExternalVideo();

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_EXTERNALVIDEO_INCLUDE_H_