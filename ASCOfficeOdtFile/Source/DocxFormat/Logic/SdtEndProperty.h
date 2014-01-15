#pragma once
#ifndef OOX_LOGIC_SDT_END_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_SDT_END_PROPERTY_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "RunProperty.h"


namespace OOX
{
	namespace Logic
	{
		class SdtEndProperty : public WritingElement
		{
		public:
			SdtEndProperty();
			virtual ~SdtEndProperty();
			explicit SdtEndProperty(const XML::XNode& node);
			const SdtEndProperty& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<RunProperty>						RunProperty;
			nullable_property<std::string>	Id;
			property<bool>									Temporary;
			property<bool>									ShowingPlcHdr;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SDT_END_PROPERTY_INCLUDE_H_