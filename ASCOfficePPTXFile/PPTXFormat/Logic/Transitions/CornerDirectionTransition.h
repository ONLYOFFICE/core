#pragma once
#ifndef PPTX_LOGIC_CORNERDIRECTION_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_CORNERDIRECTION_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/CornerDirectionVal.h"

namespace PPTX
{
	namespace Logic
	{
		class CornerDirectionTransition : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CornerDirectionTransition)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name	= XmlUtils::GetNameNoNS(node.GetName());
				node.ReadAttributeBase(L"dir", dir);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("dir"), dir);

				return XmlUtils::CreateNode(_T("p:") + name, oAttr);
			}

		public:
			CString											name;
			nullable_limit<Limit::CornerDirectionVal>		dir;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CORNERDIRECTION_TRANSITION_INCLUDE_H