#pragma once
#ifndef PPTX_LOGIC_BLDGRAPHIC_INCLUDE_H_
#define PPTX_LOGIC_BLDGRAPHIC_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "BldSub.h"

namespace PPTX
{
	namespace Logic
	{
		class BldGraphic : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BldGraphic)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				bldSub		= node.ReadNode(_T("p:bldSub"));
				node.ReadAttributeBase(L"spid", spid);
				grpId = node.ReadAttributeInt(L"grpId");
				node.ReadAttributeBase(L"uiExpand", uiExpand);

				Normalize();

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("spid"), spid);
				oAttr.Write(_T("grpId"), grpId);
				oAttr.Write(_T("uiExpand"), uiExpand);

				if (!bldSub.IsInit())
					return XmlUtils::CreateNode(_T("p:bldGraphic"), oAttr, _T("<p:bldAsOne/>"));
				
				return XmlUtils::CreateNode(_T("p:bldGraphic"), oAttr, bldSub->toXML());
			}

		public:
			nullable<BldSub>		bldSub;

			CString					spid;
			int						grpId;
			nullable_bool			uiExpand;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(bldSub.IsInit())
					bldSub->SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				if (grpId < 0)
					grpId = 0;
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLDGRAPHIC_INCLUDE_H