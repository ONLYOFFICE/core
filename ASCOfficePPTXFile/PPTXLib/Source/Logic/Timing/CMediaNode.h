#pragma once
#ifndef PPTX_LOGIC_CMEDIANODE_INCLUDE_H_
#define PPTX_LOGIC_CMEDIANODE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CTn.h"
#include "TgtEl.h"

namespace PPTX
{
	namespace Logic
	{
		class CMediaNode : public WrapperWritingElement
		{
		public:
			CMediaNode();
			virtual ~CMediaNode();			
			explicit CMediaNode(const XML::XNode& node);
			const CMediaNode& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CTn> cTn;
			property<TgtEl> tgtEl;

			nullable_property<bool> mute;
			nullable_property<int, setter::only_positive<int> > numSld;
			nullable_property<bool> showWhenStopped;
			nullable_property<int> vol;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CMEDIANODE_INCLUDE_H