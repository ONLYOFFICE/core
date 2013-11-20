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
		ExternalAudio(const boost::filesystem::wpath& uri);
		~ExternalAudio();

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_EXTERNALAUDIO_INCLUDE_H_