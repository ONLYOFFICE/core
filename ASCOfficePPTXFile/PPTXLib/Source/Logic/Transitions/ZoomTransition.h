#pragma once
#ifndef PPTX_LOGIC_ZOOM_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_ZOOM_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/InOutDirectionVal.h"

namespace PPTX
{
	namespace Logic
	{
		class ZoomTransition : public WrapperWritingElement
		{
		public:
			ZoomTransition();
			virtual ~ZoomTransition();			
			explicit ZoomTransition(const XML::XNode& node);
			const ZoomTransition& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string, Limit::InOutDirectionVal> dir;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_Zoom_TRANSITION_INCLUDE_H