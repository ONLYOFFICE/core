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
		Item(const boost::filesystem::wpath& filename);
		virtual ~Item();

	public:
		virtual void read(const boost::filesystem::wpath& filename);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	private:
		nullable_property<std::string> SelectedStyle;
		nullable_property<std::string> StyleName;
	};
} // namespace OOX

#endif // OOX_ITEM_INCLUDE_H_