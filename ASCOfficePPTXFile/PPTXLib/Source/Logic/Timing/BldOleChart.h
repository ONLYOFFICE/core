#pragma once
#ifndef PPTX_LOGIC_BLDOLECHART_INCLUDE_H_
#define PPTX_LOGIC_BLDOLECHART_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "./../../Limit/ChartBuildType.h"

namespace PPTX
{
	namespace Logic
	{
		class BldOleChart : public WrapperWritingElement
		{
		public:
			BldOleChart();
			virtual ~BldOleChart();			
			explicit BldOleChart(const XML::XNode& node);
			const BldOleChart& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> spid;
			property<int, setter::only_positive<int> > grpId;
			nullable_property<bool> uiExpand;
			nullable_property<std::string, Limit::ChartBuildType> bld;
			nullable_property<bool> animBg;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLDOLECHART_INCLUDE_H