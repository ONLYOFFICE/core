#pragma once
#ifndef OOX_AUDIO_INCLUDE_H_
#define OOX_AUDIO_INCLUDE_H_

#include "Media.h"

namespace PPTX
{
	class Audio : public Media
	{
	public:
		Audio()
		{
		}
		Audio(const OOX::CPath& filename)
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

#endif // OOX_AUDIO_INCLUDE_H_