#include "CDefs.h"

namespace SVG
{
	CDefObject::CDefObject(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent)
	    : CSvgObject(oNode, pParent)
	{}

	CDefObject::~CDefObject()
	{}

	bool CDefObject::Apply(IRenderer *pRenderer, const CDefs *pDefs, const TBounds &oObjectBounds)
	{
		return true;
	}

	CDefs::CDefs(){}

	CDefObject *CDefs::GetDef(const std::wstring &wsId) const
	{
		if (m_arObjects.empty())
			return NULL;

		std::vector<CDefObject*>::const_iterator oFound = std::find_if(m_arObjects.begin(), m_arObjects.end(), [&wsId](CDefObject* pObject){ if (wsId == pObject->GetId()) return true; else return false;});

		if (m_arObjects.end() != oFound)
			return dynamic_cast<CDefObject*>(*oFound);

		return NULL;
	}
}
