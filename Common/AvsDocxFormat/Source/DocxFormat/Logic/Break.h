#pragma once
#ifndef OOX_LOGIC_BREAK_INCLUDE_H_
#define OOX_LOGIC_BREAK_INCLUDE_H_

#include "RunItemBase.h"
#include "./../Limit/BreakType.h"
#include "./../Limit/BreakClear.h"


namespace OOX
{
	namespace Logic
	{
		class Break : public RunItemBase
		{
		public:
			Break();
			virtual ~Break();
			explicit Break(const std::string& text);
			explicit Break(const XML::XNode& node);
			const Break& operator =(const XML::XNode& node);
			const Break& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			nullable_property<std::string, Limit::BreakType>	Type;
			nullable_property<std::string, Limit::BreakClear>	Clear;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_BREAK_INCLUDE_H_