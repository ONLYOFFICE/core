#pragma once
#ifndef PPTX_LOGIC_ORIENTATION_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_ORIENTATION_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/Orient.h"

namespace PPTX
{
	namespace Logic
	{
		class OrientationTransition : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(OrientationTransition)

		public:
			void fromXML(XmlUtils::CXmlNode& node)
			{
				name	= XmlUtils::GetNameNoNS(node.GetName());
				node.ReadAttributeBase(L"dir", dir);
			}

			CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("dir"), dir);

				return XmlUtils::CreateNode(_T("p:") + name, oAttr);
			}

		public:
			CString									name;
			nullable_limit<Limit::Orient>			dir;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ORIENTATION_TRANSITION_INCLUDE_H