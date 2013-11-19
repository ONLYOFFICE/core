#pragma once
#ifndef OOX_LOGIC_LINE_STYLE_INCLUDE_H_
#define OOX_LOGIC_LINE_STYLE_INCLUDE_H_

#include <string>

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"

namespace OOX
{
	namespace Logic
	{
		class LineStyle : public WritingElement
		{
		public:
			LineStyle();
			virtual ~LineStyle();			
			explicit LineStyle(const XML::XNode& node);
			const LineStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string>	strokeColor;
			nullable_property<std::string>	stroked;
			nullable_property<std::string>	strokeWeight;	
			nullable_property<std::string>	lineStyle;
			nullable_property<std::string>	dashStyle;
			nullable_property<std::string>	opacity;
			nullable_property<std::string>	endcap;
			nullable_property<std::string>	startarrow;
			nullable_property<std::string>	startarrowwidth;
			nullable_property<std::string>	startarrowlength;
			nullable_property<std::string>	endarrow;
			nullable_property<std::string>	endarrowwidth;
			nullable_property<std::string>	endarrowlength;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_LINE_STYLE_INCLUDE_H_