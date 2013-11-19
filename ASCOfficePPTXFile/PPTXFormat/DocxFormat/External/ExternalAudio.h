#pragma once
#ifndef OOX_EXTERNALAUDIO_INCLUDE_H_
#define OOX_EXTERNALAUDIO_INCLUDE_H_

#include "External.h"


namespace OOX
{
	class ExternalAudio : public External
	{
	public:
		ExternalAudio()
		{
		}
		ExternalAudio(const CPath& uri)
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

#endif // OOX_EXTERNALAUDIO_INCLUDE_H_