#pragma once
#ifndef ODP_CONTENT_IMAGE_INCLUDE_H_
#define ODP_CONTENT_IMAGE_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "OdtFormat/Limit/Actuate.h"
#include "OdtFormat/Limit/HrefType.h"
#include "OdtFormat/Limit/Show.h"
#include "OdtFormat/Content/Text.h"

namespace Odp
{
	namespace Content
	{
		class Image : public Odt::WritingElement
		{
		public:
			Image();
			virtual ~Image();
			explicit Image(const XML::XNode& node);
			const Image& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> href;
			property<std::string, Odt::Limit::HrefType> type;
			property<std::string, Odt::Limit::Show> show;
			property<std::string, Odt::Limit::Actuate> actuate;

			property<std::vector<Odt::Content::Text> > Items;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_IMAGE_INCLUDE_H_