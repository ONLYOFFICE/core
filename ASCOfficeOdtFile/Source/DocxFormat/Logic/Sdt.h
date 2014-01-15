#pragma once
#ifndef OOX_LOGIC_SDT_INCLUDE_H_
#define OOX_LOGIC_SDT_INCLUDE_H_

#include "TextItemBase.h"
#include "property.h"
#include "nullable_property.h"
#include "SdtProperty.h"
#include "SdtEndProperty.h"
#include "SdtContent.h"


namespace OOX
{
	namespace Logic
	{
		class Sdt : public TextItemBase
		{
		public:
			Sdt();
			virtual ~Sdt();
			explicit Sdt(const XML::XNode& node);
			const Sdt& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<SdtProperty>		Property;
			nullable_property<SdtEndProperty> EndProperty;
			property<SdtContent>							Content;		
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SDT_INCLUDE_H_