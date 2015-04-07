#pragma once
#ifndef PPTX_LOGIC_ANIMVARIANT_INCLUDE_H_
#define PPTX_LOGIC_ANIMVARIANT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimVariant : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AnimVariant)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name	= XmlUtils::GetNameNoNS(node.GetName());

				XmlUtils::CXmlNode oNode;

				if (node.GetNode(_T("p:boolVal"), oNode))
					oNode.ReadAttributeBase(L"val", boolVal);
				else if (node.GetNode(_T("p:intVal"), oNode))
					oNode.ReadAttributeBase(L"val", intVal);
				else if (node.GetNode(_T("p:fltVal"), oNode))
					oNode.ReadAttributeBase(L"val", fltVal);
				else if (node.GetNode(_T("p:clrVal"), oNode))
					clrVal.GetColorFrom(oNode);
				else if (node.GetNode(_T("p:strVal"), oNode))
					oNode.ReadAttributeBase(L"val", strVal);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;

				if (strVal.IsInit())
				{
					XmlUtils::CAttribute oAttr;
					oAttr.Write(_T("val"), strVal);
					oValue.m_strValue += XmlUtils::CreateNode(_T("p:strVal"), oAttr);
				}
				if (boolVal.IsInit())
				{
					XmlUtils::CAttribute oAttr;
					oAttr.Write(_T("val"), boolVal);
					oValue.m_strValue += XmlUtils::CreateNode(_T("p:boolVal"), oAttr);
				}
				if (intVal.IsInit())
				{
					XmlUtils::CAttribute oAttr;
					oAttr.Write(_T("val"), intVal);
					oValue.m_strValue += XmlUtils::CreateNode(_T("p:intVal"), oAttr);
				}
				if (fltVal.IsInit())
				{
					XmlUtils::CAttribute oAttr;
					oAttr.Write(_T("val"), fltVal);
					oValue.m_strValue += XmlUtils::CreateNode(_T("p:fltVal"), oAttr);
				}
				if (clrVal.is_init())
				{
					oValue.m_strValue += (_T("<p:clrVal>") + clrVal.toXML() + _T("</p:clrVal>"));
				}
				
				return XmlUtils::CreateNode(_T("p:") + name, oValue);
			}
		public:
			CString				name;

			nullable_bool		boolVal;
			nullable_string		strVal;
			nullable_int		intVal;
			nullable_double		fltVal;

			UniColor			clrVal;
		protected:
			virtual void FillParentPointersForChilds()
			{
				clrVal.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMVARIANT_INCLUDE_H_