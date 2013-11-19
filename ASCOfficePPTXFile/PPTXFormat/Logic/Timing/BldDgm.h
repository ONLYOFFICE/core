#pragma once
#ifndef PPTX_LOGIC_BLDDGM_INCLUDE_H_
#define PPTX_LOGIC_BLDDGM_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/DgmBuildType.h"

namespace PPTX
{
	namespace Logic
	{
		class BldDgm : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BldDgm)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"spid", spid);
				grpId = node.ReadAttributeInt(L"grpId");
				node.ReadAttributeBase(L"uiExpand", uiExpand);
				node.ReadAttributeBase(L"bld", bld);

				Normalize();

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("spid"), spid);
				oAttr.Write(_T("grpId"), grpId);
				oAttr.Write(_T("uiExpand"), uiExpand);
				oAttr.WriteLimitNullable(_T("bld"), bld);

				return XmlUtils::CreateNode(_T("p:bldDgm"), oAttr);
			}

		public:
			CString								spid;
			int									grpId;
			nullable_bool						uiExpand;
			nullable_limit<Limit::DgmBuildType> bld;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				if (grpId < 0)
					grpId = 0;
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLDDGM_INCLUDE_H