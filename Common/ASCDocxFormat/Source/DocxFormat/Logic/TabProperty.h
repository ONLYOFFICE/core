#pragma once
#ifndef OOX_LOGIC_TAB_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_TAB_PROPERTY_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "./../Limit/TabVal.h"
#include "./../Limit/Leader.h"
#include "./../Unit.h"


namespace OOX
{
	namespace Logic
	{
		class TabProperty : public WritingElement
		{
		public:
			TabProperty();
			virtual ~TabProperty();
			explicit TabProperty(const XML::XNode& node);
			const TabProperty& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string, Limit::TabVal>					 Val;
			nullable_property<std::string, Limit::Leader>  Leader;
			property<UniversalUnit>									 Pos;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TAB_PROPERTY_INCLUDE_H_