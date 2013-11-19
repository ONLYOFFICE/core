#pragma once
#ifndef PPTX_LOGIC_WHEEL_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_WHEEL_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class WheelTransition : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(WheelTransition)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"spokes", spokes);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("spokes"), spokes);

				return XmlUtils::CreateNode(_T("p:wheel"), oAttr);
			}

		public:
			nullable_int	spokes;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				spokes.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_WHEEL_TRANSITION_INCLUDE_H