#pragma once
#ifndef OOX_LOGIC_ENDNOTE_REF_INCLUDE_H_
#define OOX_LOGIC_ENDNOTE_REF_INCLUDE_H_

#include "RunItemBase.h"


namespace OOX
{
	namespace Logic
	{
		class EndnoteRef : public RunItemBase
		{
		public:
			EndnoteRef();
			virtual ~EndnoteRef();			
			explicit EndnoteRef(const XML::XNode& node);			
			const EndnoteRef& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_ENDNOTE_REF_INCLUDE_H_