#pragma once
#ifndef PPTX_LOGIC_SET_INCLUDE_H_
#define PPTX_LOGIC_SET_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CBhvr.h"
#include "AnimVariant.h"

namespace PPTX
{
	namespace Logic
	{
		class Set : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Set)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cBhvr	= node.ReadNode(_T("p:cBhvr"));
				to		= node.ReadNode(_T("p:to"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(cBhvr);
				oValue.WriteNullable(to);

				return XmlUtils::CreateNode(_T("p:set"), oValue);
			}

		public:
			CBhvr					cBhvr;
			nullable<AnimVariant>	to;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cBhvr.SetParentPointer(this);
				if(to.IsInit())
					to->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SET_INCLUDE_H