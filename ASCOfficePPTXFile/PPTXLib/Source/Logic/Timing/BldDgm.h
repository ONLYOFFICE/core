#pragma once
#ifndef PPTX_LOGIC_BLDDGM_INCLUDE_H_
#define PPTX_LOGIC_BLDDGM_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "./../../Limit/DgmBuildType.h"

namespace PPTX
{
	namespace Logic
	{
		class BldDgm : public WrapperWritingElement
		{
		public:
			BldDgm();
			virtual ~BldDgm();			
			explicit BldDgm(const XML::XNode& node);
			const BldDgm& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> spid;
			property<int, setter::only_positive<int> > grpId;
			nullable_property<bool> uiExpand;
			nullable_property<std::string, Limit::DgmBuildType> bld;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLDDGM_INCLUDE_H