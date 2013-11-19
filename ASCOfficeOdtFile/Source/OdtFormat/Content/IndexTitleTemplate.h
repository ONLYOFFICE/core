#pragma once
#ifndef ODT_CONTENT_INDEX_TITLE_TEMPLATE_INCLUDE_H_
#define ODT_CONTENT_INDEX_TITLE_TEMPLATE_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <string>


namespace Odt
{
	namespace Content
	{
		class IndexTitleTemplate : public WritingElement
		{
		public:
			IndexTitleTemplate();
			virtual ~IndexTitleTemplate();
			explicit IndexTitleTemplate(const XML::XNode& node);
			const IndexTitleTemplate& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> Text;
			property<std::string> Style;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_INDEX_TITLE_TEMPLATE_INCLUDE_H_