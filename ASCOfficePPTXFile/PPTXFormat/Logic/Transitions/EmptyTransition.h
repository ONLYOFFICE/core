#pragma once
#ifndef PPTX_LOGIC_EMPTY_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_EMPTY_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class EmptyTransition : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(EmptyTransition)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name = XmlUtils::GetNameNoNS(node.GetName());
			}

			virtual CString toXML() const
			{
				return _T("<p:") + name + _T("/>");
			}

		public:
			CString name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EMPTY_TRANSITION_INCLUDE_H