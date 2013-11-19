#pragma once
#ifndef PPTX_LOGIC_ANIM_INCLUDE_H_
#define PPTX_LOGIC_ANIM_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CBhvr.h"
#include "TavLst.h"
#include "./../../Limit/TLCalcMode.h"
#include "./../../Limit/TLValueType.h"

namespace PPTX
{
	namespace Logic
	{
		class Anim : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Anim)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"by", by);
				node.ReadAttributeBase(L"from", from);
				node.ReadAttributeBase(L"to", to);
				node.ReadAttributeBase(L"calcmode", calcmode);
				node.ReadAttributeBase(L"valueType", valueType);

				cBhvr		= node.ReadNode(_T("p:cBhvr"));
				tavLst		= node.ReadNode(_T("p:tavLst"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("by"), by);
				oAttr.Write(_T("from"), from);
				oAttr.Write(_T("to"), to);
				oAttr.WriteLimitNullable(_T("calcmode"), calcmode);
				oAttr.WriteLimitNullable(_T("valueType"), valueType);

				XmlUtils::CNodeValue oValue;
				oValue.Write(cBhvr);
				oValue.WriteNullable(tavLst);

				return XmlUtils::CreateNode(_T("p:anim"), oAttr, oValue);
			}

		public:
			CBhvr cBhvr;
			nullable<TavLst> tavLst;

			nullable_string						by; //W3C XML Schema string 
			nullable_limit<Limit::TLCalcMode>	calcmode; //ST_TLAnimateBehaviorCalcMode
			nullable_string						from; //W3C XML Schema string 
			nullable_string						to; //W3C XML Schema string 
			nullable_limit<Limit::TLValueType>	valueType; //ST_TLAnimateBehaviorValueType
		protected:
			virtual void FillParentPointersForChilds()
			{
				cBhvr.SetParentPointer(this);
				if(tavLst.IsInit())
					tavLst->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIM_INCLUDE_H