#pragma once
#ifndef OOX_EXTERNALIMAGE_INCLUDE_H_
#define OOX_EXTERNALIMAGE_INCLUDE_H_

#include "External.h"


namespace OOX
{
	class ExternalImage : public External
	{
	public:
		ExternalImage();
		ExternalImage(const boost::filesystem::wpath& uri);
		~ExternalImage();

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_EXTERNALIMAGE_INCLUDE_H_