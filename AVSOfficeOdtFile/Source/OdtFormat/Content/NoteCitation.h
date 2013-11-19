#pragma once
#ifndef ODT_CONTENT_NOTE_CITATION_INCLUDE_H_
#define ODT_CONTENT_NOTE_CITATION_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"
#include "nullable_property.h"


namespace Odt
{
	namespace Content
	{
		class NoteCitation : public WritingElement
		{
		public:
			NoteCitation();
			virtual ~NoteCitation();
			explicit NoteCitation(const XML::XNode& node);
			const NoteCitation& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string>	Label;
			nullable_property<std::string>	Text;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_NOTE_CITATION_INCLUDE_H_
