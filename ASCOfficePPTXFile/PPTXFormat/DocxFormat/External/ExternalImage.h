#pragma once
#ifndef OOX_EXTERNALIMAGE_INCLUDE_H_
#define OOX_EXTERNALIMAGE_INCLUDE_H_

#include "External.h"


namespace PPTX
{
	class ExternalImage : public External
	{
	public:
		ExternalImage()
		{
		}
		ExternalImage(const OOX::CPath& uri)
		{
			read(uri);
		}
		~ExternalImage()
		{
		}

	public:
		virtual const FileType type() const
		{
			return FileTypes::ExternalImage;
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

#endif // OOX_EXTERNALIMAGE_INCLUDE_H_