#pragma once
#ifndef OOX_LOGIC_CONTINUATION_SEPARATOR_INCLUDE_H_
#define OOX_LOGIC_CONTINUATION_SEPARATOR_INCLUDE_H_

#include "RunItemBase.h"


namespace OOX
{
	namespace Logic
	{
		class ContinuationSeparator : public RunItemBase
		{
		public:
			ContinuationSeparator();
			virtual ~ContinuationSeparator();			
			explicit ContinuationSeparator(const XML::XNode& node);
			const ContinuationSeparator& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_CONTINUATION_SEPARATOR_INCLUDE_H_