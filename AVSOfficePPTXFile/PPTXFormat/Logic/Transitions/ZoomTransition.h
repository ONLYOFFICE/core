#pragma once
#ifndef PPTX_LOGIC_ZOOM_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_ZOOM_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/InOutDirectionVal.h"

namespace PPTX
{
	namespace Logic
	{
		class ZoomTransition : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(ZoomTransition)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"dir", dir);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("dir"), dir);

				return XmlUtils::CreateNode(_T("p:zoom"), oAttr);
			}

		public:
			nullable_limit<Limit::InOutDirectionVal>		dir;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_Zoom_TRANSITION_INCLUDE_H