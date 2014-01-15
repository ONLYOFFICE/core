#pragma once
#ifndef OOX_LOGIC_FOOTNOTE_REFERENCE_INCLUDE_H_
#define OOX_LOGIC_FOOTNOTE_REFERENCE_INCLUDE_H_

#include "RunItemBase.h"
#include "property.h"


namespace OOX
{
	namespace Logic
	{
		class FootnoteReference : public RunItemBase
		{
		public:
			FootnoteReference();
			virtual ~FootnoteReference();
			explicit FootnoteReference(const size_t index);
			explicit FootnoteReference(const XML::XNode& node);
			const FootnoteReference& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;
			
		public:
			property<size_t>	Id;
			nullable_property<int>	CustomMarkFollows;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_FOOTNOTE_REFERENCE_INCLUDE_H_