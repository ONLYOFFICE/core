#pragma once
#ifndef OOX_EXTERNALIMAGE_INCLUDE_H_
#define OOX_EXTERNALIMAGE_INCLUDE_H_

#include "External.h"


namespace OOX
{
	class ExternalImage : public External
	{
	public:
		ExternalImage()
		{
		}
		ExternalImage(const CPath& uri)
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

#endif // OOX_EXTERNALIMAGE_INCLUDE_H_