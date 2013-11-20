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
		Audio(const boost::filesystem::wpath& filename);
		virtual ~Audio();

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_AUDIO_INCLUDE_H_