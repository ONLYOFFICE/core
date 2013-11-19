#pragma once
#ifndef ODT_CONTENT_SPACE_INCLUDE_H_
#define ODT_CONTENT_SPACE_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"
#include "setter.h"


namespace Odt
{
	namespace Content
	{
		class Space : public ParagraphItemBase
		{
		public:
			Space();
			virtual ~Space();
			explicit Space(const size_t count);
			explicit Space(const XML::XNode& node);
			explicit Space(const std::string& text);
			const Space& operator =(const XML::XNode& node);
			const Space& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			property<int, setter::between<int, 0, 100> >		Count;
		};
	} // namespace Content
} // namespace Odt;

#endif // ODT_CONTENT_SPACE_INCLUDE_H_