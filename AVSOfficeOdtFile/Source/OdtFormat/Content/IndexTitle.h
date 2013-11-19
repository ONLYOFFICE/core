#pragma once
#ifndef ODT_CONTENT_INDEX_TITLE_INCLUDE_H_
#define ODT_CONTENT_INDEX_TITLE_INCLUDE_H_

#include "TextItem.h"
#include "property.h"
#include "Paragraph.h"
#include <string>


namespace Odt
{
	namespace Content
	{
		class IndexTitle : public TextItem
		{
		public:
			IndexTitle();
			virtual ~IndexTitle();
			explicit IndexTitle(const XML::XNode& node);
			const IndexTitle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			virtual const bool hasList() const;
			virtual const bool hasNote() const;

		public:
			property<Paragraph>			Item;
			property<std::string>		Style;
			property<std::string>		Name;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_INDEX_TITLE_INCLUDE_H_