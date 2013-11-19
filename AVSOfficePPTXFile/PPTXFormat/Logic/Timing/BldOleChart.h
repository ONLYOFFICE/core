#pragma once
#ifndef PPTX_LOGIC_BLDOLECHART_INCLUDE_H_
#define PPTX_LOGIC_BLDOLECHART_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/ChartBuildType.h"

namespace PPTX
{
	namespace Logic
	{
		class BldOleChart : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BldOleChart)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				spid		= node.GetAttribute(_T("spid"));
				grpId		= node.ReadAttributeInt(L"grpId");
				node.ReadAttributeBase(L"uiExpand", uiExpand);
				node.ReadAttributeBase(L"bld", bld);
				node.ReadAttributeBase(L"animBg", animBg);

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
				oAttr.Write(_T("animBg"), animBg);

				return XmlUtils::CreateNode(_T("p:bldOleChart"), oAttr);
			}

		public:
			CString									spid;
			int										grpId;
			nullable_bool							uiExpand;
			nullable_limit<Limit::ChartBuildType>	bld;
			nullable_bool							animBg;
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

#endif // PPTX_LOGIC_BLDOLECHART_INCLUDE_H