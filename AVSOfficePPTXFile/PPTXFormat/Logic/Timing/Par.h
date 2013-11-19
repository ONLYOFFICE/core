#pragma once
#ifndef PPTX_LOGIC_PAR_INCLUDE_H_
#define PPTX_LOGIC_PAR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CTn.h"

namespace PPTX
{
	namespace Logic
	{
		class Par : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Par)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cTn = node.ReadNode(_T("p:cTn"));
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				return XmlUtils::CreateNode(_T("p:par"), cTn.toXML());
			}

		public:
			CTn		cTn;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cTn.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PAR_INCLUDE_H