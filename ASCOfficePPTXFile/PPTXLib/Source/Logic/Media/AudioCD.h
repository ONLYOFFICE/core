#pragma once
#ifndef PPTX_LOGIC_AUDIOCD_INCLUDE_H_
#define PPTX_LOGIC_AUDIOCD_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable_property.h"

namespace PPTX
{
	namespace Logic
	{
		class AudioCD : public WrapperWritingElement
		{
		public:
			AudioCD();
			virtual ~AudioCD();			
			explicit AudioCD(const XML::XNode& node);
			const AudioCD& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int, setter::between<int, 0, 255> > stTrack;
			nullable_property<int, setter::only_positive<int> > stTime;
			property<int, setter::between<int, 0, 255> > endTrack;
			nullable_property<int, setter::only_positive<int> > endTime;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_AUDIOCD_INCLUDE_H_