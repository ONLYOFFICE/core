#pragma once
#ifndef PPTX_LOGIC_TNLST_INCLUDE_H_
#define PPTX_LOGIC_TNLST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "TimeNodeBase.h"

namespace PPTX
{
	namespace Logic
	{
		class TnLst : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TnLst)

			TnLst& operator=(const TnLst& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				list = oSrc.list;
				name = oSrc.name;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name = XmlUtils::GetNameNoNS(node.GetName());
				
				node.LoadArray(_T("*"), list);
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(list);

				return XmlUtils::CreateNode(_T("p:") + name, oValue);
			}
		public:
			std::vector<TimeNodeBase>		list;
			CString						name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				size_t count = 0;
				for (size_t i = 0; i < count; ++i)
					list[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TNLST_INCLUDE_H_