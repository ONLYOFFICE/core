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
		HyperLink(const boost::filesystem::wpath& uri);
		~HyperLink();

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_HYPERLINK_INCLUDE_H_