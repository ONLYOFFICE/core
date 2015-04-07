#include "EffectDag.h"

namespace PPTX
{
	namespace Logic
	{

		void EffectDag::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name	= node.GetName();
			node.ReadAttributeBase(L"name", name);
			node.ReadAttributeBase(L"type", type);
            Effects.clear();
			node.LoadArray(_T("*"), Effects);

			FillParentPointersForChilds();
		}

		CString EffectDag::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("name"), name);
			oAttr.WriteLimitNullable(_T("type"), type);

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(Effects);

			return XmlUtils::CreateNode(m_name, oAttr, oValue);
		}

		void EffectDag::FillParentPointersForChilds()
		{
            size_t count = Effects.size();
			for(size_t i = 0; i < count; ++i)
				Effects[i].SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
