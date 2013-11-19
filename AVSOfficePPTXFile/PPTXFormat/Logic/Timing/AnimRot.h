#pragma once
#ifndef PPTX_LOGIC_ANIMROT_INCLUDE_H_
#define PPTX_LOGIC_ANIMROT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CBhvr.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimRot : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AnimRot)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cBhvr	= node.ReadNode(_T("p:cBhvr"));

				node.ReadAttributeBase(L"by", by);
				node.ReadAttributeBase(L"from", from);
				node.ReadAttributeBase(L"to", to);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("by"), by);
				oAttr.Write(_T("from"), from);
				oAttr.Write(_T("to"), to);

				XmlUtils::CNodeValue oValue;
				oValue.Write(cBhvr);

				return XmlUtils::CreateNode(_T("p:animRot"), oAttr, oValue);
			}

		public:
			CBhvr			cBhvr;

			nullable_int	by;
			nullable_int	from;
			nullable_int	to;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cBhvr.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMROT_INCLUDE_H