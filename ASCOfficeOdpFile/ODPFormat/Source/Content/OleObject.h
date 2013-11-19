#pragma once
#ifndef ODP_CONTENT_OLEOBJECT_INCLUDE_H_
#define ODP_CONTENT_OLEOBJECT_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "OdtFormat/Limit/Actuate.h"
#include "OdtFormat/Limit/HrefType.h"
#include "OdtFormat/Limit/Show.h"

namespace Odp
{
	namespace Content
	{
		class OleObject : public Odt::WritingElement
		{
		public:
			OleObject();
			virtual ~OleObject();
			explicit OleObject(const XML::XNode& node);
			const OleObject& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> href;
			property<std::string, Odt::Limit::HrefType> type;
			property<std::string, Odt::Limit::Show> show;
			property<std::string, Odt::Limit::Actuate> actuate;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_OLEOBJECT_INCLUDE_H_