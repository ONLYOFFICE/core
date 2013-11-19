#pragma once
#ifndef PPTX_LOGIC_AUDIO_INCLUDE_H_
#define PPTX_LOGIC_AUDIO_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CMediaNode.h"

namespace PPTX
{
	namespace Logic
	{
		class Audio : public WrapperWritingElement
		{
		public:
			Audio();
			virtual ~Audio();			
			explicit Audio(const XML::XNode& node);
			const Audio& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CMediaNode> cMediaNode;
			nullable_property<bool> isNarration;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_AUDIO_INCLUDE_H