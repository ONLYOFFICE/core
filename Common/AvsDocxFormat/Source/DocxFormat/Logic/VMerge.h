#pragma once
#ifndef OOX_LOGIC_V_MERGE_INCLUDE_H_
#define OOX_LOGIC_V_MERGE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "nullable_property.h"
#include "./../Limit/VMergeVal.h"


namespace OOX
{
	namespace Logic
	{
		class VMerge : public WritingElement
		{
		public:
			VMerge();
			virtual ~VMerge();
			explicit VMerge(const XML::XNode& node);
			const VMerge& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string, Limit::VMergeVal>		Value;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_V_MERGE_INCLUDE_H_