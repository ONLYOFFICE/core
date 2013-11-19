#include "EffectDag.h"

namespace PPTX
{
	namespace Logic
	{
		EffectDag::EffectDag()
		{
		}

		EffectDag::~EffectDag()
		{
		}

		EffectDag::EffectDag(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const EffectDag& EffectDag::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}

		void EffectDag::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name	= node.GetName();
			node.ReadAttributeBase(L"name", name);
			node.ReadAttributeBase(L"type", type);
			Effects.RemoveAll();
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
			size_t count = Effects.GetCount();
			for(size_t i = 0; i < count; ++i)
				Effects[i].SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX