#pragma once
#ifndef ODT_LOGIC_NAMED_STYLE_INCLUDE_H_
#define ODT_LOGIC_NAMED_STYLE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/StyleFamily.h"
#include "./../Limit/StyleClass.h"
#include "Properties.h"


namespace Odt
{
	namespace Logic
	{
		class NamedStyle : public WritingElement
		{
		public:
			NamedStyle();
			virtual ~NamedStyle();
			explicit NamedStyle(const XML::XNode& node);
			const NamedStyle& operator =(const XML::XNode& node);
		
		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool isSimple() const;

		public:
			property<std::string>											Name;
			nullable_property<std::string>						DisplayName;
			property<std::string, Limit::StyleFamily>	Family;
			nullable_property<std::string>						ParentName;
			nullable_property<std::string>						NextName;
			nullable_property<std::string>						MasterPageName;
			nullable_property<int>										DefaultOutlineLevel;
			nullable_property<std::string, Limit::StyleClass>	Class;
			nullable_property<std::string>						ListStyleName;
			property<Properties>											Properties;			
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_NAMED_STYLE_INCLUDE_H_