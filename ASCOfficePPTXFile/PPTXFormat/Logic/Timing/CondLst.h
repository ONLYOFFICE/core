#pragma once
#ifndef PPTX_LOGIC_CONDLST_INCLUDE_H_
#define PPTX_LOGIC_CONDLST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "Cond.h"

namespace PPTX
{
	namespace Logic
	{
		class CondLst : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CondLst)

			CondLst& operator=(const CondLst& oSrc)
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
				node.LoadArray(_T("p:cond"), list);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(list);

				return XmlUtils::CreateNode(_T("p:") + name, oValue);
			}
		public:
			std::vector<Cond>			list;
			CString					name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				size_t count = list.size();
				for (size_t i = 0; i < count; ++i)
					list[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CONDLST_INCLUDE_H_