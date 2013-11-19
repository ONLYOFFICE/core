#pragma once
#ifndef ODT_CONTENT_PAGE_NUMBER_INCLUDE_H_
#define ODT_CONTENT_PAGE_NUMBER_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"
#include "./../Limit/SelectPage.h"
#include "./../Logic/NumFormat.h"


namespace Odt
{
	namespace Content
	{
		class PageNumber : public ParagraphItemBase
		{
		public:
			PageNumber();
			virtual ~PageNumber();
			explicit PageNumber(const XML::XNode& node);
			const PageNumber& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			nullable_property<std::string, Limit::SelectPage> SelectPage;
			nullable_property<Logic::NumFormat>			 NumFormat;
			property<std::string>										 Value;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_PAGE_NUMBER_INCLUDE_H_