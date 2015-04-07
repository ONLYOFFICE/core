#pragma once
#ifndef PPTX_LOGIC_EIGHTDIRECTION_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_EIGHTDIRECTION_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/EightDirectionVal.h"

namespace PPTX
{
	namespace Logic
	{
		class EightDirectionTransition : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(EightDirectionTransition)

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
			nullable_limit<Limit::EightDirectionVal>	dir;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EIGHTDIRECTION_TRANSITION_INCLUDE_H