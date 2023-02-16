#include "CDefs.h"

namespace SVG
{
	CDefs::CDefs(CObjectBase *pParent)
	    : CContainer(pParent)
	{}

	IDefObject *CDefs::GetDef(const std::wstring &wsId) const
	{
		if (m_arObjects.empty())
			return NULL;

		std::vector<CObjectBase*>::const_iterator oFound = std::find_if(m_arObjects.begin(), m_arObjects.end(), [&wsId](CObjectBase* pObject){ if (wsId == pObject->GetId()) return pObject;});

		return dynamic_cast<IDefObject*>(*oFound);
	}
}
