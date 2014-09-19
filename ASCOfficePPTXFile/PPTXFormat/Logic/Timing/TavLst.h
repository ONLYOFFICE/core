#pragma once
#ifndef PPTX_LOGIC_TAVLST_INCLUDE_H_
#define PPTX_LOGIC_TAVLST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "Tav.h"

namespace PPTX
{
	namespace Logic
	{
		class TavLst : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TavLst)

			TavLst& operator=(const TavLst& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				list = oSrc.list;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.LoadArray(_T("p:tav"), list);
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(list);

				return XmlUtils::CreateNode(_T("p:tavLst"), oValue);
			}
		public:
			std::vector<Tav> list;
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

#endif // PPTX_LOGIC_TAVLST_INCLUDE_H_