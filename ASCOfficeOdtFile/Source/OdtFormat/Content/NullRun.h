#pragma once
#ifndef ODT_CONTENT_NULL_RUN_INCLUDE_H_
#define ODT_CONTENT_NULL_RUN_INCLUDE_H_

#include "ParagraphItemBase.h"


namespace Odt
{
	namespace Content
	{
		class NullRun : public ParagraphItemBase
		{
		public:
			NullRun();
			virtual ~NullRun();
			explicit NullRun(const XML::XNode& node);
			explicit NullRun(const std::string& text);
			const NullRun& operator =(const XML::XNode& node);
			const NullRun& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_NULL_RUN_INCLUDE_H_