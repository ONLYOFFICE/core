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
		WebSettings(const boost::filesystem::wpath& filename);
		virtual ~WebSettings();

	public:
		virtual void read(const boost::filesystem::wpath& filename);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_WEBSETTING_INCLDUE_H_