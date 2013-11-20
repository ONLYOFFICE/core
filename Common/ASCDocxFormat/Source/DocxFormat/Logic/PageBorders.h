#pragma once
#ifndef OOX_LOGIC_PAGE_BORDERS_INCLUDE_H_
#define OOX_LOGIC_PAGE_BORDERS_INCLUDE_H_

#include "./../WritingElement.h"
#include "Border.h"


namespace OOX
{
	namespace Logic
	{
		class PageBorders : public WritingElement
		{
		public:
			PageBorders();
			virtual ~PageBorders();
			explicit PageBorders(const XML::XNode& node);
			const PageBorders& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
	
		public:
			nullable_property<Border>		Top;
			nullable_property<Border>		Bottom;
			nullable_property<Border>		Left;
			nullable_property<Border>		Right;
			nullable_property<std::string>	OffsetFrom;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PAGE_BORDERS_INCLUDE_H_