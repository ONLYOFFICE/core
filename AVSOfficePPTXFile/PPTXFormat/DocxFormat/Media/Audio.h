#pragma once
#ifndef OOX_AUDIO_INCLUDE_H_
#define OOX_AUDIO_INCLUDE_H_

#include "Media.h"

namespace OOX
{
	class Audio : public Media
	{
	public:
		Audio()
		{
		}
		Audio(const CPath& filename)
		{
			read(filename);
		}
		virtual ~Audio()
		{
		}

	public:
		virtual const FileType type() const
		{
			return FileTypes::Audio;
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

#endif // OOX_AUDIO_INCLUDE_H_