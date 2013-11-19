#pragma once
#ifndef PPTX_LOGIC_CBHVR_INCLUDE_H_
#define PPTX_LOGIC_CBHVR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CTn.h"
#include "AttrNameLst.h"
#include "TgtEl.h"
#include "./../../Limit/TLAccumulate.h"
#include "./../../Limit/TLAdditive.h"
#include "./../../Limit/TLOverride.h"
#include "./../../Limit/TLTransform.h"

namespace PPTX
{
	namespace Logic
	{
		class CBhvr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CBhvr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"accumulate", accumulate);
				node.ReadAttributeBase(L"additive", additive);
				node.ReadAttributeBase(L"by", by);
				node.ReadAttributeBase(L"from", from);
				node.ReadAttributeBase(L"override", override_);
				node.ReadAttributeBase(L"rctx", rctx);
				node.ReadAttributeBase(L"to", to);
				node.ReadAttributeBase(L"xfrmType", xfrmType);

				cTn			= node.ReadNode(_T("p:cTn"));
				tgtEl		= node.ReadNode(_T("p:tgtEl"));
				attrNameLst = node.ReadNode(_T("p:attrNameLst"));

				FillParentPointersForChilds();
			}

			CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("accumulate"), accumulate);
				oAttr.WriteLimitNullable(_T("additive"), additive);
				oAttr.Write(_T("by"), by);
				oAttr.Write(_T("from"), from);
				oAttr.WriteLimitNullable(_T("override"), override_);
				oAttr.Write(_T("rctx"), rctx);
				oAttr.Write(_T("to"), to);
				oAttr.WriteLimitNullable(_T("xfrmType"), xfrmType);

				XmlUtils::CNodeValue oValue;
				oValue.Write(cTn);
				oValue.Write(tgtEl);
				oValue.WriteNullable(attrNameLst);

				return XmlUtils::CreateNode(_T("p:cBhvr"), oAttr, oValue);
			}

		public:
			CTn						cTn;
			TgtEl					tgtEl;
			nullable<AttrNameLst>	attrNameLst;

			nullable_limit<Limit::TLAccumulate>		accumulate; //ST_TLBehaviorAccumulateType 
			nullable_limit<Limit::TLAdditive>		additive;	//ST_TLBehaviorAdditiveType 
			nullable_string							by;			//W3C XML Schema string 
			nullable_string							from;		//W3C XML Schema string 
			nullable_limit<Limit::TLOverride>		override_;	//ST_TLBehaviorOverrideType 
			nullable_string							rctx;		//"PPT"/"IE" W3C XML Schema string 
			nullable_string							to;			//W3C XML Schema string 
			nullable_limit<Limit::TLTransform>		xfrmType;	//ST_TLBehaviorTransformType 
		protected:
			virtual void FillParentPointersForChilds()
			{
				cTn.SetParentPointer(this);
				tgtEl.SetParentPointer(this);
				if(attrNameLst.IsInit())
					attrNameLst->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CBHVR_INCLUDE_H