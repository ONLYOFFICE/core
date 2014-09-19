#pragma once
#ifndef PPTX_LOGIC_TMPLLST_INCLUDE_H_
#define PPTX_LOGIC_TMPLLST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "Tmpl.h"

namespace PPTX
{
	namespace Logic
	{
		class TmplLst : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TmplLst)

			TmplLst& operator=(const TmplLst& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				list = oSrc.list;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.LoadArray(_T("p:tmpl"), list);
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(list);

				return XmlUtils::CreateNode(_T("p:tmplLst"), oValue);			
			}
		public:
			std::vector<Tmpl> list;
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

#endif // PPTX_LOGIC_TMPLLST_INCLUDE_H_