#pragma once
#ifndef OOX_LOGIC_LANG_INCLUDE_H_
#define OOX_LOGIC_LANG_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "nullable_property.h"


namespace OOX
{
	namespace Logic
	{
		class Lang : public WritingElement
		{
		public:
			Lang();
			virtual ~Lang();
			explicit Lang(const XML::XNode& node);
			const Lang& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string>	Value;
			nullable_property<std::string>	EastAsia;
			nullable_property<std::string>	Bidi;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_LANG_INCLUDE_H_