#pragma once
#ifndef ODT_CONTENT_SOFT_PAGE_BREAK_INCLUDE_H_
#define ODT_CONTENT_SOFT_PAGE_BREAK_INCLUDE_H_

#include "ParagraphItemBase.h"


namespace Odt
{
	namespace Content
	{
		class SoftPageBreak : public ParagraphItemBase
		{
		public:
			SoftPageBreak();
			virtual ~SoftPageBreak();
			explicit SoftPageBreak(const XML::XNode& node);
			explicit SoftPageBreak(const std::string& text);
			const SoftPageBreak& operator =(const XML::XNode& node);
			const SoftPageBreak& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_SOFT_PAGE_BREAK_INCLUDE_H_