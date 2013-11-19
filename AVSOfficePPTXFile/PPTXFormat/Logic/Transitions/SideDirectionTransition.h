#pragma once
#ifndef PPTX_LOGIC_SIDEDIRECTION_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_SIDEDIRECTION_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/SideDirectionVal.h"

namespace PPTX
{
	namespace Logic
	{
		class SideDirectionTransition : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SideDirectionTransition)

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
			CString										name;
			nullable_limit<Limit::SideDirectionVal>		dir;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SIDEDIRECTION_TRANSITION_INCLUDE_H