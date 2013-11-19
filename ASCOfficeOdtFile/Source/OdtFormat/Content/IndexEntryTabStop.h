#pragma once
#ifndef ODT_CONTENT_INDEX_ENTRY_TAB_STOP_INCLUDE_H_
#define ODT_CONTENT_INDEX_ENTRY_TAB_STOP_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <string>
#include "./../Limit/TabStopType.h"
#include "./../Limit/LeaderText.h"


namespace Odt
{
	namespace Content
	{
		class IndexEntryTabStop : public WritingElement
		{
		public:
			IndexEntryTabStop();
			virtual ~IndexEntryTabStop();
			explicit IndexEntryTabStop(const XML::XNode& node);
			const IndexEntryTabStop& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string, Limit::TabStopType>		Type;
			property<std::string, Limit::LeaderText>		LeaderChar;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_INDEX_ENTRY_TAB_STOP_INCLUDE_H_