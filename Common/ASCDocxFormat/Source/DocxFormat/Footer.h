#pragma once
#ifndef OOX_FOOTER_INCLUDE_H_
#define OOX_FOOTER_INCLUDE_H_

#include "File.h"
#include "IFileContainer.h"
#include "Logic/TextItem.h"
#include <vector>
#include "property.h"


namespace OOX
{
	class Footer : public OOX::File, public IFileContainer
	{
	public:
		Footer();
		Footer(const boost::filesystem::wpath& filename);
		virtual ~Footer();

	public:
		virtual void read(const boost::filesystem::wpath& filename);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		property<std::vector<Logic::TextItem> >	Items;
	};
} // namespace OOX

#endif // OOX_FOOTER_INCLUDE_H_