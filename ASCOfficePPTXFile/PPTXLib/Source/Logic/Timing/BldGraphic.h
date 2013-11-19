#pragma once
#ifndef PPTX_LOGIC_BLDGRAPHIC_INCLUDE_H_
#define PPTX_LOGIC_BLDGRAPHIC_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "BldSub.h"

namespace PPTX
{
	namespace Logic
	{
		class BldGraphic : public WrapperWritingElement
		{
		public:
			BldGraphic();
			virtual ~BldGraphic();			
			explicit BldGraphic(const XML::XNode& node);
			const BldGraphic& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<BldSub> bldSub;

			property<std::string> spid;
			property<int, setter::only_positive<int> > grpId;
			nullable_property<bool> uiExpand;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLDGRAPHIC_INCLUDE_H