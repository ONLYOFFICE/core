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
		ExternalVideo(const OOX::CPath& uri);
		~ExternalVideo();

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_EXTERNALVIDEO_INCLUDE_H_