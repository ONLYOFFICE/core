#pragma once
#ifndef OOX_HYPERLINK_INCLUDE_H_
#define OOX_HYPERLINK_INCLUDE_H_

#include "External.h"


namespace PPTX
{
	class HyperLink : public External
	{
	public:
		HyperLink()
		{
		}
		HyperLink(const OOX::CPath& uri)
		{
			read(uri);
		}
		~HyperLink()
		{
		}

	public:
		virtual const FileType type() const
		{
			return FileTypes::HyperLink;
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

#endif // OOX_HYPERLINK_INCLUDE_H_