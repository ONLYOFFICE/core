#pragma once
#ifndef PPTX_LOGIC_SPTGT_INCLUDE_H_
#define PPTX_LOGIC_SPTGT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "TxEl.h"
#include "GraphicEl.h"
#include "./../../Limit/TLChartSubElement.h"

namespace PPTX
{
	namespace Logic
	{
		class SpTgt : public WrapperWritingElement
		{
		public:
			SpTgt();
			virtual ~SpTgt();
			explicit SpTgt(const XML::XNode& node);
			const SpTgt& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> spid;

			nullable_property<bool> bg;
			nullable_property<std::string> subSpid;

			//oleChartEl
			nullable_property<std::string, Limit::TLChartSubElement> type;
			nullable_property<int, setter::only_positive<int> > lvl;

			nullable_property<TxEl> txEl;
			nullable_property<GraphicEl> graphicEl;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SPTGT_INCLUDE_H_