#pragma once
#ifndef OOX_HYPERLINK_INCLUDE_H_
#define OOX_HYPERLINK_INCLUDE_H_

#include "External.h"

namespace OOX
{
	class HyperLink : public External
	{
	public:
		HyperLink();
		HyperLink(const OOX::CPath& uri);
		~HyperLink();

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_HYPERLINK_INCLUDE_H_