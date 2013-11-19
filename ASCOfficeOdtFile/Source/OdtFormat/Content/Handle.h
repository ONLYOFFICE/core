#pragma once
#ifndef ODT_CONTENT_HANDLE_INCLUDE_H_
#define ODT_CONTENT_HANDLE_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <string>


namespace Odt
{
	namespace Content
	{
		class Handle : public WritingElement
		{
		public:
			Handle();
			virtual ~Handle();
			explicit Handle(const XML::XNode& node);
			const Handle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>						Position;
			nullable_property<std::string>	XMin;
			nullable_property<std::string>	YMin;
			nullable_property<std::string>	XMax;
			nullable_property<std::string>	YMax;

			nullable_property<std::string>	Polar;
			nullable_property<std::string>	RMin;
			nullable_property<std::string>	RMax;			
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_HANDLE_INCLUDE_H_