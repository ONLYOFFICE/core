#pragma once
#ifndef OOX_LOGIC_TABLE_BORDER_INCLUDE_H_
#define OOX_LOGIC_TABLE_BORDER_INCLUDE_H_

#include "property.h"
#include "./../WritingElement.h"

#include "BorderItem.h"
#include "./../Limit/BorderName.h"

namespace OOX
{
	namespace Logic
	{
		class Border : public WritingElement
		{
		public:
			Border();
			virtual ~Border();
			explicit Border(const XML::XNode& node);
			Border(const std::string& name, const BorderItem& border);
			const Border& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string, Limit::BorderName>	Name;
			property<BorderItem>						Bdr;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_BORDER_INCLUDE_H_