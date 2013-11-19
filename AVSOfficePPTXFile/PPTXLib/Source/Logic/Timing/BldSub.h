#pragma once
#ifndef PPTX_LOGIC_BLDSUB_INCLUDE_H_
#define PPTX_LOGIC_BLDSUB_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "./../../Limit/ChartBuildType.h"
#include "./../../Limit/AnimationDgmBuild.h"

namespace PPTX
{
	namespace Logic
	{
		class BldSub : public WrapperWritingElement
		{
		public:
			BldSub();
			virtual ~BldSub();			
			explicit BldSub(const XML::XNode& node);
			const BldSub& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<bool> chart;
			//bldChart
			nullable_property<bool> animBg;
			nullable_property<std::string, Limit::ChartBuildType> bldChart;
			//bldDgm
			nullable_property<std::string, Limit::AnimationDgmBuild> bldDgm;
			nullable_property<bool> rev;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLDSUB_INCLUDE_H_