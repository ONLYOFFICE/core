#pragma once
#ifndef PPTX_LOGIC_OPTIONALBLACK_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_OPTIONALBLACK_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class OptionalBlackTransition : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(OptionalBlackTransition)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name	= XmlUtils::GetNameNoNS(node.GetName());
				node.ReadAttributeBase(L"thruBlk", thruBlk);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("thruBlk"), thruBlk);

				return XmlUtils::CreateNode(_T("p:") + name, oAttr);
			}

		public:
			CString					name;
			nullable_bool			thruBlk;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_OPTIONALBLACK_TRANSITION_INCLUDE_H