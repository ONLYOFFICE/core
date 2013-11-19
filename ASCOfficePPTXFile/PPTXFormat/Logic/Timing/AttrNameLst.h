#pragma once
#ifndef PPTX_LOGIC_ATTRNAMELST_INCLUDE_H_
#define PPTX_LOGIC_ATTRNAMELST_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "AttrName.h"

namespace PPTX
{
	namespace Logic
	{
		class AttrNameLst : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AttrNameLst)

			AttrNameLst& operator=(const AttrNameLst& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				list.Copy(oSrc.list);
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.LoadArray(_T("p:attrName"), list);
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(list);

				return XmlUtils::CreateNode(_T("p:attrNameLst"), oValue);
			}
		public:
			CAtlArray<AttrName> list;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ATTRNAMELST_INCLUDE_H_