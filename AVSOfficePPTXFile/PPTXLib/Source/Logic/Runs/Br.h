#pragma once
#ifndef PPTX_LOGIC_BR_INCLUDE_H_
#define PPTX_LOGIC_BR_INCLUDE_H_

#include "RunBase.h"
#include "property.h"
#include "./../RunProperties.h"

namespace PPTX
{
	namespace Logic
	{
		class Br : public RunBase
		{
		public:
			Br();
			virtual ~Br();			
			explicit Br(const XML::XNode& node);
			const Br& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual std::string GetText()const{return "\n";};
			virtual std::wstring GetWText()const{return L"\n";};
		public:
			nullable_property<RunProperties> rPr;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BR_INCLUDE_H