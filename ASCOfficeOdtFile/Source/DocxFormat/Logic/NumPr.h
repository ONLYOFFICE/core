#pragma once
#ifndef OOX_LOGIC_NUMPR_INCLUDE_H_
#define OOX_LOGIC_NUMPR_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"


namespace OOX
{
	namespace Logic
	{
		class NumPr : public WritingElement
		{
		public:
			NumPr();
			virtual ~NumPr();
			explicit NumPr(const XML::XNode& node);
			const NumPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<int>	Ilvl;
			nullable_property<int>	NumId;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_NUMPR_INCLUDE_H_