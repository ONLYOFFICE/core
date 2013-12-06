#pragma once
#ifndef OOX_ITEM_INCLUDE_H_
#define OOX_ITEM_INCLUDE_H_

#include "File.h"
#include "nullable_property.h"


namespace OOX
{
	class Item : public OOX::File
	{
	public:
		Item();
		Item(const OOX::CPath& filename);
		virtual ~Item();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

	private:
		nullable_property<std::string> SelectedStyle;
		nullable_property<std::string> StyleName;
	};
} // namespace OOX

#endif // OOX_ITEM_INCLUDE_H_