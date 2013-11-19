#pragma once
#ifndef PPTX_LOGIC_BLDP_INCLUDE_H_
#define PPTX_LOGIC_BLDP_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/ParaBuildType.h"
#include "TmplLst.h"

namespace PPTX
{
	namespace Logic
	{
		class BldP : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BldP)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				tmplLst				= node.ReadNode(_T("p:tmplLst"));

				spid				= node.GetAttribute(_T("spid"));
				grpId				= node.ReadAttributeInt(L"grpId");
				node.ReadAttributeBase(L"uiExpand", uiExpand);
				node.ReadAttributeBase(L"build", build);
				node.ReadAttributeBase(L"bldLvl", bldLvl);
				node.ReadAttributeBase(L"animBg", animBg);
				node.ReadAttributeBase(L"autoUpdateAnimBg", autoUpdateAnimBg);
				node.ReadAttributeBase(L"rev", rev);
				node.ReadAttributeBase(L"advAuto", advAuto);

				Normalize();

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("spid"), spid);
				oAttr.Write(_T("grpId"), grpId);
				oAttr.Write(_T("uiExpand"), uiExpand);
				oAttr.WriteLimitNullable(_T("build"), build);
				oAttr.Write(_T("bldLvl"), bldLvl);
				oAttr.Write(_T("animBg"), animBg);
				oAttr.Write(_T("autoUpdateAnimBg"), autoUpdateAnimBg);
				oAttr.Write(_T("rev"), rev);
				oAttr.Write(_T("advAuto"), advAuto);
				oAttr.Write(_T("spid"), spid);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(tmplLst);

				return XmlUtils::CreateNode(_T("p:bldP"), oAttr, oValue);
			}

		public:
			nullable<TmplLst>						tmplLst;

			CString									spid;
			int										grpId;
			nullable_bool							uiExpand;
			nullable_limit<Limit::ParaBuildType>	build;
			nullable_int							bldLvl;
			nullable_bool							animBg;
			nullable_bool							autoUpdateAnimBg;
			nullable_bool							rev;
			nullable_string							advAuto;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(tmplLst.IsInit())
					tmplLst->SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				if (grpId < 0)
					grpId = 0;

				if (bldLvl.IsInit())
					if (*bldLvl < 0)
						*bldLvl = 0;
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLDP_INCLUDE_H