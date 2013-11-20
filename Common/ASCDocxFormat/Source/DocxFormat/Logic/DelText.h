#pragma once
#ifndef OOX_LOGIC_DEL_TEXT_INCLUDE_H_
#define OOX_LOGIC_DEL_TEXT_INCLUDE_H_

#include "RunItemBase.h"
#include "nullable_property.h"


namespace OOX
{
	namespace Logic
	{
		class DelText : public RunItemBase
		{
		public:
			DelText();
			virtual ~DelText();
			explicit DelText(const XML::XNode& node);
			const DelText& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			virtual const std::string toTxt() const;

		public:
			nullable_property<std::string>		Text;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DEL_TEXT_INCLUDE_H_