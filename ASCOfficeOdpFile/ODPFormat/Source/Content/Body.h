#pragma once
#ifndef ODP_CONTENT_BODY_INCLUDE_H_
#define ODP_CONTENT_BODY_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "Slide.h"

namespace Odp
{
	namespace Content
	{
		class Body : public Odt::WritingElement
		{
		public:
			Body();
			virtual ~Body();
			explicit Body(const XML::XNode& node);
			const Body& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::vector<Slide> > Slides;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_BODY_INCLUDE_H_