#pragma once
#ifndef OOX_LOGIC_FOOTNOTE_REF_INCLUDE_H_
#define OOX_LOGIC_FOOTNOTE_REF_INCLUDE_H_

#include "RunItemBase.h"


namespace OOX
{
	namespace Logic
	{
		class FootnoteRef : public RunItemBase
		{
		public:
			FootnoteRef();
			virtual ~FootnoteRef();			
			explicit FootnoteRef(const XML::XNode& node);			
			const FootnoteRef& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_FOOTNOTE_REF_INCLUDE_H_