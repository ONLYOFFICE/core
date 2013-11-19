#pragma once
#ifndef ODT_CONTENT_INDEX_BODY_INCLUDE_H_
#define ODT_CONTENT_INDEX_BODY_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "Text.h"
#include <vector>


namespace Odt
{
	namespace Content
	{
		class IndexBody : public WritingElement
		{
		public:
			IndexBody();
			virtual ~IndexBody();
			explicit IndexBody(const XML::XNode& node);
			const IndexBody& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::vector<Text> > Items;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_INDEX_BODY_INCLUDE_H_