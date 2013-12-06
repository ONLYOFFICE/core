#pragma once
#ifndef OOX_WEBSETTINGS_INCLUDE_H_
#define OOX_WEBSETTINGS_INCLUDE_H_

#include "File.h"

namespace OOX
{
	class WebSettings : public OOX::File
	{
	public:
		WebSettings();
		WebSettings(const OOX::CPath& filename);
		virtual ~WebSettings();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_WEBSETTING_INCLDUE_H_