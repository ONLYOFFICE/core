#pragma once
#ifndef PPTX_LOGIC_GRAPHICEL_INCLUDE_H_
#define PPTX_LOGIC_GRAPHICEL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/DgmBuild.h"
#include "./../../Limit/ChartBuild.h"

namespace PPTX
{
	namespace Logic
	{
		class GraphicEl : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(GraphicEl)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oNode;
				
				if (node.GetNode(_T("p:chart"), oNode))
				{
					oNode.ReadAttributeBase(L"bldStep", chartBuildStep);
					oNode.ReadAttributeBase(L"seriesIdx", seriesIdx);
					oNode.ReadAttributeBase(L"categoryIdx", categoryIdx);
				}
				else if (node.GetNode(_T("p:dgm"), oNode))
				{
					oNode.ReadAttributeBase(L"bldStep", dgmBuildStep);
					oNode.ReadAttributeBase(L"id", dgmId);
				}
			}

			virtual CString toXML() const
			{
				if (chartBuildStep.IsInit())
				{
					XmlUtils::CAttribute oAttr;
					oAttr.WriteLimitNullable(_T("bldStep"), chartBuildStep);
					oAttr.Write(_T("seriesIdx"), seriesIdx);
					oAttr.Write(_T("categoryIdx"), categoryIdx);

					return XmlUtils::CreateNode(_T("p:graphicEl"), XmlUtils::CreateNode(_T("p:chart"), oAttr));
				}

				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("id"), dgmId);
				oAttr.WriteLimitNullable(_T("bldStep"), dgmBuildStep);

				return XmlUtils::CreateNode(_T("p:graphicEl"), XmlUtils::CreateNode(_T("p:dgm"), oAttr));
			}
		public:
			//Dgm
			nullable_string						dgmId;
			nullable_limit<Limit::DgmBuild>		dgmBuildStep;

			//Chart
			nullable_limit<Limit::ChartBuild>	chartBuildStep;
			nullable_int						seriesIdx;
			nullable_int						categoryIdx;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GRAPHICEL_INCLUDE_H_