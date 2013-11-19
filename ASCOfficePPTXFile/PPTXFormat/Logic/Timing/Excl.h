#pragma once
#ifndef PPTX_LOGIC_EXCL_INCLUDE_H_
#define PPTX_LOGIC_EXCL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CTn.h"

namespace PPTX
{
	namespace Logic
	{
		class Excl : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Excl)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cTn = node.ReadNode(_T("p:cTn"));
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(cTn);

				return XmlUtils::CreateNode(_T("p:excl"), oValue);
			}

		public:
			CTn			cTn;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cTn.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EXCL_INCLUDE_H