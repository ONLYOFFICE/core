#pragma once
#ifndef PPTX_LOGIC_VIDEO_INCLUDE_H_
#define PPTX_LOGIC_VIDEO_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CMediaNode.h"

namespace PPTX
{
	namespace Logic
	{
		class Video : public WrapperWritingElement
		{
		public:
			Video();
			virtual ~Video();			
			explicit Video(const XML::XNode& node);
			const Video& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CMediaNode> cMediaNode;
			nullable_property<bool> fullScrn;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_VIDEO_INCLUDE_H