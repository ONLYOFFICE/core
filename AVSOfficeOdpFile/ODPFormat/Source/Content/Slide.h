#pragma once
#ifndef ODP_CONTENT_SLIDE_INCLUDE_H_
#define ODP_CONTENT_SLIDE_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "OdtFormat/Content/Forms.h"
#include "DrawingElement.h"
#include "Animation/Par.h"

namespace Odp
{
	namespace Content
	{
		class Slide : public Odt::WritingElement
		{
		public:
			Slide();
			virtual ~Slide();
			explicit Slide(const XML::XNode& node);
			const Slide& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> Name;
			property<std::string> StyleName;
			property<std::string> MasterPageName;
			nullable_property<std::string> Id;

			nullable_property<Odt::Content::Forms> Forms;
			property<std::vector<DrawingElement> > Elements;
			nullable_property<Par> Animation;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_SLIDE_INCLUDE_H_