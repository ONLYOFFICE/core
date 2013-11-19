#pragma once
#ifndef OOX_HYPERLINK_INCLUDE_H_
#define OOX_HYPERLINK_INCLUDE_H_

#include "External.h"


namespace OOX
{
	class HyperLink : public External
	{
	public:
		HyperLink()
		{
		}
		HyperLink(const CPath& uri)
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

#endif // OOX_HYPERLINK_INCLUDE_H_