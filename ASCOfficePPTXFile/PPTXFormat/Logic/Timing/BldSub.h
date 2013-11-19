#pragma once
#ifndef PPTX_LOGIC_BLDSUB_INCLUDE_H_
#define PPTX_LOGIC_BLDSUB_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/ChartBuildType.h"
#include "./../../Limit/AnimationDgmBuild.h"

namespace PPTX
{
	namespace Logic
	{
		class BldSub : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BldSub)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oNode;
				if (node.GetNode(_T("a:bldChart"), oNode))
				{
					chart		= true;
					oNode.ReadAttributeBase(L"bld", bldChart);
					oNode.ReadAttributeBase(L"animBg", animBg);
				}
				else if (node.GetNode(_T("a:bldDgm"), oNode))
				{
					chart		= false;
					oNode.ReadAttributeBase(L"bld", bldDgm);
					oNode.ReadAttributeBase(L"rev", rev);
				}
				else
					chart.reset();

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				if (chart.IsInit())
				{
					if (*chart)
					{
						XmlUtils::CAttribute oAttr;
						oAttr.WriteLimitNullable(_T("bld"), bldChart);
						oAttr.Write(_T("animBg"), animBg);

						return XmlUtils::CreateNode(_T("p:bldSub"), XmlUtils::CreateNode(_T("a:bldChart"), oAttr));
					}

					XmlUtils::CAttribute oAttr;
					oAttr.WriteLimitNullable(_T("bld"), bldDgm);
					oAttr.Write(_T("rev"), rev);

					return XmlUtils::CreateNode(_T("p:bldSub"), XmlUtils::CreateNode(_T("a:bldDgm"), oAttr));
				}
				return _T("<p:bldSub/>");
			}

		public:
			nullable_bool								chart;
			//bldChart
			nullable_bool								animBg;
			nullable_limit<Limit::ChartBuildType>		bldChart;
			//bldDgm
			nullable_limit<Limit::AnimationDgmBuild>	bldDgm;
			nullable_bool								rev;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLDSUB_INCLUDE_H_