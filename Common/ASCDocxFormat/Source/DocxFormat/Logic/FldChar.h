#pragma once
#ifndef OOX_LOGIC_FLD_CHAR_INCLUDE_H_
#define OOX_LOGIC_FLD_CHAR_INCLUDE_H_

#include "RunItemBase.h"
#include "property.h"
#include "./../Limit/FldCharType.h"


namespace OOX
{
	namespace Logic
	{
		class FldChar : public RunItemBase
		{
		public:
			FldChar();
			virtual ~FldChar();
			explicit FldChar(const XML::XNode& node);
			const FldChar& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			virtual const std::string toTxt() const;

		public:
			property<std::string, Limit::FldCharType>		FldCharType;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_FLD_CHAR_INCLUDE_H_