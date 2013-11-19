#pragma once
#ifndef ODT_CONTENT_OBJECT_OLE_INCLUDE_H_
#define ODT_CONTENT_OBJECT_OLE_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/Show.h"
#include "./../Limit/Actuate.h"
#include "./../Limit/ImageType.h"


namespace Odt
{
	namespace Content
	{
		class ObjectOle : public WritingElement
		{
		public:
			ObjectOle();
			virtual ~ObjectOle();
			explicit ObjectOle(const XML::XNode& node);
			const ObjectOle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<boost::filesystem::wpath> Href;
			property<std::string, Limit::ImageType> Type;
			property<std::string, Limit::Show>		Show;
			property<std::string, Limit::Actuate>	Actuate;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_OBJECT_OLE_INCLUDE_H_
