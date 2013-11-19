#pragma once
#ifndef ODT_CONTENT_LIST_ITEM_INCLUDE_H_
#define ODT_CONTENT_LIST_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <vector>
#include "Text.h"


namespace Odt
{
	namespace Content
	{
		class ListItem : public WritingElement
		{
		public:
			ListItem();
			virtual ~ListItem();
			explicit ListItem(const XML::XNode& node);
			const ListItem& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool hasNote() const;

		public:
			property<std::vector<Text> >	Texts;

		private:
			bool m_hasNote;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_LIST_ITEM_INCLUDE_H_