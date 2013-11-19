#pragma once
#ifndef ODT_CONTENT_LIST_INCLUDE_H_
#define ODT_CONTENT_LIST_INCLUDE_H_

#include "TextItem.h"
#include "property.h"
#include "nullable_property.h"
#include <string>
#include <vector>
#include "ListItem.h"


namespace Odt
{
	namespace Content
	{
		class List : public TextItem
		{
		public:
			List();
			virtual ~List();
			explicit List(const XML::XNode& node);
			const List& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			virtual const bool hasList() const;
			virtual const bool hasNote() const;

		public:
			nullable_property<std::string>		Style;
			property<std::vector<ListItem> >	Items;
			nullable_property<bool>				ContinueNumbering;
			nullable_property<std::string> id;
			nullable_property<std::string> ContinueList;

		private:
			bool m_hasNote;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_LIST_INCLUDE_H_