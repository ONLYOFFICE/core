#pragma once
#ifndef OOX_APP_INCLUDE_H_
#define OOX_APP_INCLUDE_H_

#include "File.h"

namespace OOX
{
	class App : public OOX::File
	{
	public:
		App();
		App(const OOX::CPath& filename);
		virtual ~App();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_APP_INCLUDE_H_