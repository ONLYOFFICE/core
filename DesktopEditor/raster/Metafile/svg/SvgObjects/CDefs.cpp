#include "CDefs.h"

namespace SVG
{
	CDefs::CDefs()
	{}

	CContainer &CDefs::GetContainer()
	{
		return m_oContainer;
	}

	IDefObject *CDefs::GetDef(const std::wstring &wsId) const
	{
		if (m_oContainer.m_arObjects.empty())
			return NULL;

		std::vector<CObjectBase*>::const_iterator oFound = std::find_if(m_oContainer.m_arObjects.begin(), m_oContainer.m_arObjects.end(), [&wsId](CObjectBase* pObject){ if (wsId == pObject->GetId()) return pObject;});

		if (m_oContainer.m_arObjects.end() != oFound)
			return dynamic_cast<IDefObject*>(*oFound);

		return NULL;
	}
}
