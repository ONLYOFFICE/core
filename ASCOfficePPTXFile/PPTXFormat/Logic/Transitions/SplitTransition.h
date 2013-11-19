#pragma once
#ifndef PPTX_LOGIC_SPLIT_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_SPLIT_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/InOutDirectionVal.h"
#include "./../../Limit/Orient.h"

namespace PPTX
{
	namespace Logic
	{
		class SplitTransition : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SplitTransition)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"dir", dir);
				node.ReadAttributeBase(L"orient", orient);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("dir"), dir);
				oAttr.WriteLimitNullable(_T("orient"), orient);

				return XmlUtils::CreateNode(_T("p:split"), oAttr);
			}

		public:
			nullable_limit<Limit::InOutDirectionVal>	dir;
			nullable_limit<Limit::Orient>				orient;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SPLIT_TRANSITION_INCLUDE_H_