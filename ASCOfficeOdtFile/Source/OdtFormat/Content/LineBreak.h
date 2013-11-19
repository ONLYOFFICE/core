#pragma once
#ifndef ODT_CONTENT_LINE_BREAK_INCLUDE_H_
#define ODT_CONTENT_LINE_BREAK_INCLUDE_H_

#include "ParagraphItemBase.h"


namespace Odt
{
	namespace Content
	{
		class LineBreak : public ParagraphItemBase
		{
		public:
			LineBreak();
			virtual ~LineBreak();
			explicit LineBreak(const XML::XNode& node);
			explicit LineBreak(const std::string& text);
			const LineBreak& operator =(const XML::XNode& node);
			const LineBreak& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_LINE_BREAK_INCLUDE_H_