#pragma once
#ifndef ODT_CONTENT_SECTION_INCLUDE_H_
#define ODT_CONTENT_SECTION_INCLUDE_H_

#include "TextItem.h"
#include "Text.h"
#include <vector>
#include "property.h"
#include "nullable.h"


namespace Odt
{
	namespace Content
	{
		class Section : public TextItem
		{
		public:
			Section();
			virtual ~Section();
			explicit Section(const XML::XNode& node);
			const Section& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			virtual const bool hasList() const;
			virtual const bool hasNote() const;

		public:
			property<std::string>					Style;
			property<std::vector<Text> >			Items;
			nullable_property<std::string>			Name;
			
		private:
			bool									m_hasNote;
			bool									m_hasList;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_SECTION_INCLUDE_H_