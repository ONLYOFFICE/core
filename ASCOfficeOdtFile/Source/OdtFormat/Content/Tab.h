#pragma once
#ifndef ODT_CONTENT_TAB_INCLUDE_H_
#define ODT_CONTENT_TAB_INCLUDE_H_

#include "ParagraphItemBase.h"


namespace Odt
{
	namespace Content
	{
		class Tab : public ParagraphItemBase
		{
		public:
			Tab();
			virtual ~Tab();
			explicit Tab(const XML::XNode& node);
			explicit Tab(const std::string& text);
			const Tab& operator =(const XML::XNode& node);
			const Tab& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TAB_INCLUDE_H_