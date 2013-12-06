#pragma once
#ifndef OOX_EXTERNALAUDIO_INCLUDE_H_
#define OOX_EXTERNALAUDIO_INCLUDE_H_

#include "External.h"

namespace OOX
{
	class ExternalAudio : public External
	{
	public:
		ExternalAudio();
		ExternalAudio(const OOX::CPath& uri);
		~ExternalAudio();

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_EXTERNALAUDIO_INCLUDE_H_