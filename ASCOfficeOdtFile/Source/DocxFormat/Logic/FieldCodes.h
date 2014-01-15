#pragma once
#ifndef OOX_LOGIC_FIELD_CODES_INCLUDE_H_
#define OOX_LOGIC_FIELD_CODES_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"


namespace OOX
{
	namespace Logic
	{
		class FieldCodes : public WritingElement
		{
		public:
			FieldCodes();
			virtual ~FieldCodes();			
			explicit FieldCodes(const XML::XNode& node);
			const FieldCodes& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;	

		public:
			property<std::string> Text;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_FIELD_CODES_INCLUDE_H_