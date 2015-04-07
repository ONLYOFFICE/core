#pragma once
#ifndef OOX_EXTERNALAUDIO_INCLUDE_H_
#define OOX_EXTERNALAUDIO_INCLUDE_H_

#include "External.h"


namespace PPTX
{
	class ExternalAudio : public External
	{
	public:
		ExternalAudio()
		{
		}
		ExternalAudio(const OOX::CPath& uri)
		{
			read(uri);
		}
		~ExternalAudio()
		{
		}

	public:
		virtual const FileType type() const
		{
			return FileTypes::ExternalAudio;
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

#endif // OOX_EXTERNALAUDIO_INCLUDE_H_