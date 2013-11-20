#pragma once
#ifndef OOX_LOGIC_SYMBOL_INCLUDE_H_
#define OOX_LOGIC_SYMBOL_INCLUDE_H_

#include "RunItemBase.h"


namespace OOX
{
	namespace Logic
	{
		class Symbol : public RunItemBase
		{
		public:
			Symbol();
			virtual ~Symbol();
			explicit Symbol(const std::string& text);
			explicit Symbol(const XML::XNode& node);
			const Symbol& operator =(const XML::XNode& node);
			const Symbol& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
		    nullable_property<std::string> Font;
			nullable_property<std::string> Char;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SYMBOL_INCLUDE_H_