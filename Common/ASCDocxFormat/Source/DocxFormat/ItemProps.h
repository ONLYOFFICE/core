#pragma once
#ifndef OOX_ITEM_PROPS_INCLUDE_H_
#define OOX_ITEM_PROPS_INCLUDE_H_

#include "File.h"
#include "property.h"
#include <string>
#include <vector>

#include "WritingElement.h"

namespace OOX
{
	class ItemProps : public OOX::File, public WritingElement
	{
	public:
		class SchemaRef : public WritingElement
		{
		public:
			SchemaRef();
			virtual ~SchemaRef();			
			explicit SchemaRef(const XML::XNode& node);
			const SchemaRef& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> Uri;		
		};
		class SchemaRefs : public WritingElement
		{
		public:
			SchemaRefs();
			virtual ~SchemaRefs();			
			explicit SchemaRefs(const XML::XNode& node);
			const SchemaRefs& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			std::vector<SchemaRef> SchemaRef;		
		};

	public:
		ItemProps();
		ItemProps(const OOX::CPath& filename);
		virtual ~ItemProps();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

	private:
		property<SchemaRefs>	Refs;
		property<std::string>	ItemID;
	};
} // namespace OOX

#endif // OOX_ITEM_PROPS_INCLUDE_H_