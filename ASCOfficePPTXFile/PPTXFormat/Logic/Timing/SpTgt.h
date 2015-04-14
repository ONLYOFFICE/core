#pragma once
#ifndef PPTX_LOGIC_SPTGT_INCLUDE_H_
#define PPTX_LOGIC_SPTGT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "TxEl.h"
#include "GraphicEl.h"
#include "./../../Limit/TLChartSubElement.h"

namespace PPTX
{
	namespace Logic
	{
		class SpTgt : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SpTgt)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				spid	= node.GetAttribute(_T("spid"));

				XmlUtils::CXmlNode oNode;
                bg		= (true == node.GetNode(_T("p:bg"), oNode)) ? true : false;

				XmlUtils::CXmlNode oNodeMem;
				if (node.GetNode(_T("p:subSp"), oNodeMem))
				{
					oNodeMem.ReadAttributeBase(L"spid", subSpid);
				}
				else if (node.GetNode(_T("p:oleChartEl"), oNodeMem))
				{
					oNodeMem.ReadAttributeBase(L"type", type);
					oNodeMem.ReadAttributeBase(L"lvl", lvl);
				}
				else
				{
					txEl		= node.ReadNode(_T("p:txEl"));
					graphicEl	= node.ReadNode(_T("p:graphicEl"));
				}

				Normalize();

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("spid"), spid);
				
				if (bg)
				{
					return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, _T("<p:bg/>"));
				}
				if (subSpid.IsInit())
				{
					XmlUtils::CAttribute oAttr2;
					oAttr2.Write(_T("spid"), subSpid);

					return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, XmlUtils::CreateNode(_T("p:subSp"), oAttr2));
				}
				if (type.IsInit())
				{
					XmlUtils::CAttribute oAttr2;
					oAttr2.WriteLimitNullable(_T("type"), type);
					oAttr2.Write(_T("lvl"), lvl);

					return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, XmlUtils::CreateNode(_T("p:oleChartEl"), oAttr2));
				}
				if (txEl.IsInit())
				{
					return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, txEl->toXML());
				}
				if (graphicEl.IsInit())
				{
					return XmlUtils::CreateNode(_T("p:spTgt"), oAttr, graphicEl->toXML());
				}
				return XmlUtils::CreateNode(_T("p:spTgt"), oAttr);
			}
		public:
			CString										spid;

			bool										bg;
			nullable_string								subSpid;

			//oleChartEl
			nullable_limit<Limit::TLChartSubElement>	type;
			nullable_int								lvl;

			nullable<TxEl>								txEl;
			nullable<GraphicEl>							graphicEl;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if (txEl.IsInit())
					txEl->SetParentPointer(this);
				if (graphicEl.IsInit())
					graphicEl->SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				lvl.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SPTGT_INCLUDE_H_
