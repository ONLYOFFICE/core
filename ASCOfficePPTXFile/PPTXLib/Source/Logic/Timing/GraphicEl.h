#pragma once
#ifndef PPTX_LOGIC_GRAPHICEL_INCLUDE_H_
#define PPTX_LOGIC_GRAPHICEL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../../Limit/DgmBuild.h"
#include "./../../Limit/ChartBuild.h"

namespace PPTX
{
	namespace Logic
	{
		class GraphicEl : public WrapperWritingElement
		{
		public:
			GraphicEl();
			virtual ~GraphicEl();
			explicit GraphicEl(const XML::XNode& node);
			const GraphicEl& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			//Dgm
			nullable_property<std::string> dgmId;
			nullable_property<std::string, Limit::DgmBuild> dgmBuildStep;

			//Chart
			nullable_property<std::string, Limit::ChartBuild> chartBuildStep;
			nullable_property<int> seriesIdx;
			nullable_property<int> categoryIdx;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GRAPHICEL_INCLUDE_H_