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
		ExternalImage(const OOX::CPath& uri);
		~ExternalImage();

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_EXTERNALIMAGE_INCLUDE_H_