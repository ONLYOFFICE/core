#pragma once
#ifndef PPTX_LOGIC_Tav_INCLUDE_H_
#define PPTX_LOGIC_Tav_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "AnimVariant.h"

namespace PPTX
{
	namespace Logic
	{
		class Tav : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Tav)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"tm", tm);
				node.ReadAttributeBase(L"fmla", fmla);
			
				val		= node.ReadNodeNoNS(_T("val"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("tm"), tm);
				oAttr.Write(_T("fmla"), fmla);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(val);

				return XmlUtils::CreateNode(_T("p:tav"), oAttr, oValue);
			}
		public:
			nullable<AnimVariant>	val;
			nullable_string			tm;
			nullable_string			fmla;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(val.IsInit())
					val->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_Tav_INCLUDE_H_