#pragma once
#ifndef OOX_AUDIO_INCLUDE_H_
#define OOX_AUDIO_INCLUDE_H_

#include "Media.h"


namespace OOX
{
	class Audio : public Media
	{
	public:
		Audio();
		Audio(const OOX::CPath& filename);
		virtual ~Audio();

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_AUDIO_INCLUDE_H_