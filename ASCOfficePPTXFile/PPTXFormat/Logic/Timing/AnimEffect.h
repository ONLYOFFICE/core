#pragma once
#ifndef PPTX_LOGIC_ANIMEFFECT_INCLUDE_H_
#define PPTX_LOGIC_ANIMEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CBhvr.h"
#include "AnimVariant.h"
#include "./../../Limit/TLTransition.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimEffect : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AnimEffect)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"filter", filter);
				node.ReadAttributeBase(L"prLst", prLst);
				node.ReadAttributeBase(L"transition", transition);

				cBhvr		= node.ReadNode(_T("p:cBhvr"));
				progress	= node.ReadNodeNoNS(_T("progress"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("filter"), filter);
				oAttr.Write(_T("prLst"), prLst);
				oAttr.WriteLimitNullable(_T("transition"), transition);

				XmlUtils::CNodeValue oValue;
				oValue.Write(cBhvr);
				oValue.WriteNullable(progress);

				return XmlUtils::CreateNode(_T("p:animEffect"), oAttr, oValue);
			}

		public:
			CBhvr cBhvr;
			
			nullable<AnimVariant>				progress;
			nullable_string						filter;
			nullable_string						prLst;
			nullable_limit<Limit::TLTransition> transition;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cBhvr.SetParentPointer(this);
				if(progress.IsInit())
					progress->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMEFFECT_INCLUDE_H