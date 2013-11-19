#pragma once
#ifndef ODT_META_USERDEFINED_INCLUDE_H_
#define ODT_META_USERDEFINED_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "property.h"


namespace Odp
{
	namespace Meta
	{
		class UserDefined : public Odt::WritingElement
		{
		public:
			UserDefined();
			virtual ~UserDefined();
			explicit UserDefined(const XML::XNode& node);
			const UserDefined& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> name;
		};
	} // namespace Meta
} // namespace Odp

#endif // ODT_META_USERDEFINED_INCLUDE_H_