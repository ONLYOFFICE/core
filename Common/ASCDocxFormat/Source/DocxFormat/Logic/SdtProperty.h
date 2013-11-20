#pragma once
#ifndef OOX_LOGIC_SDT_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_SDT_PROPERTY_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "DocPartObj.h"


namespace OOX
{
	namespace Logic
	{
		class SdtProperty : public WritingElement
		{
		public:
			SdtProperty();
			virtual ~SdtProperty();
			explicit SdtProperty(const XML::XNode& node);
			const SdtProperty& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int>				 Id;
			property<DocPartObj> DocPartObj;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SDT_PROPERTY_INCLUDE_H_