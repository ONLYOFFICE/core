#pragma once
#ifndef ODP_CONTENT_GROUP_INCLUDE_H_
#define ODP_CONTENT_GROUP_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "DrawingElement.h"

namespace Odp
{
	namespace Content
	{
		class Group : public Odt::WritingElement
		{
		public:
			Group();
			virtual ~Group();
			explicit Group(const XML::XNode& node);
			const Group& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::vector<DrawingElement> > Elements;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_GROUP_INCLUDE_H_