#pragma once
#ifndef OOX_LOGIC_TAB_INCLUDE_H_
#define OOX_LOGIC_TAB_INCLUDE_H_

#include "RunItemBase.h"


namespace OOX
{
	namespace Logic
	{
		class Tab : public RunItemBase
		{
		public:
			Tab();
			virtual ~Tab();
			explicit Tab(const std::string& text);
			explicit Tab(const XML::XNode& node);
			const Tab& operator =(const XML::XNode& node);
			const Tab& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TAB_INCLUDE_H_