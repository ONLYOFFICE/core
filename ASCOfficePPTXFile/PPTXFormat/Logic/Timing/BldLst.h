#pragma once
#ifndef PPTX_LOGIC_BLDLST_INCLUDE_H_
#define PPTX_LOGIC_BLDLST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "BuildNodeBase.h"

namespace PPTX
{
	namespace Logic
	{
		class BldLst : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BldLst)

			BldLst& operator=(const BldLst& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				list = oSrc.list;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.LoadArray(_T("*"), list);
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(list);
				return XmlUtils::CreateNode(_T("p:bldLst"), oValue);
			}
		public:
			std::vector<BuildNodeBase> list;
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

#endif // PPTX_LOGIC_BLDLST_INCLUDE_H_