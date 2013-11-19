#pragma once
#ifndef OOX_LOGIC_NULL_RUN_INCLUDE_H_
#define OOX_LOGIC_NULL_RUN_INCLUDE_H_

#include "RunItemBase.h"


namespace OOX
{
	namespace Logic
	{
		class NullRun : public RunItemBase
		{
		public:
			NullRun();
			virtual ~NullRun();
			explicit NullRun(const XML::XNode& node);
			const NullRun& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_NULL_RUN_INCLUDE_H_