#pragma once
#ifndef ODT_LOGIC_DEFAULT_STYLE_INCLUDE_H_
#define ODT_LOGIC_DEFAULT_STYLE_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "./../Limit/StyleFamily.h"
#include "Properties.h"


namespace Odt
{
	namespace Logic
	{
		class DefaultStyle : public WritingElement
		{
		public:
			DefaultStyle();
			virtual ~DefaultStyle();
			explicit DefaultStyle(const XML::XNode& node);
			const DefaultStyle& operator =(const XML::XNode& node);
		
		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string, Limit::StyleFamily>	Family;
			property<Properties>											Properties;			
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_DEFAULT_STYLE_INCUDE_H_