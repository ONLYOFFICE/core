#pragma once
#ifndef PPTX_LOGIC_ITERATE_INCLUDE_H_
#define PPTX_LOGIC_ITERATE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/IterateType.h"

namespace PPTX
{
	namespace Logic
	{
		class Iterate : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Iterate)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"type", type);
				node.ReadAttributeBase(L"backwards", backwards);

				XmlUtils::CXmlNode oNode;			
				if (node.GetNode(_T("p:tmAbs"), oNode))
					oNode.ReadAttributeBase(L"val", tmAbs);
				else if (node.GetNode(_T("p:tmPct"), oNode))
					oNode.ReadAttributeBase(L"val", tmPct);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("type"), type);
				oAttr.Write(_T("backwards"), backwards);
				
				if (tmAbs.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("val"), tmAbs);
					return XmlUtils::CreateNode(_T("p:iterate"), oAttr, XmlUtils::CreateNode(_T("p:tmAbs"), oAttr1));
				}
				else if (tmPct.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("val"), tmPct);
					return XmlUtils::CreateNode(_T("p:iterate"), oAttr, XmlUtils::CreateNode(_T("p:tmPct"), oAttr1));
				}

				return XmlUtils::CreateNode(_T("p:iterate"), oAttr);
			}

		public:
			nullable_limit<Limit::IterateType>		type;
			nullable_bool							backwards;

			nullable_string							tmAbs;
			nullable_int							tmPct;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ITERATE_INCLUDE_H