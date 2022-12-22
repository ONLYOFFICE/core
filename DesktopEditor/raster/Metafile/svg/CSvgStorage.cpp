#include "CSvgStorage.h"

namespace SVG
{
	CSvgStorage::CSvgStorage()
	{

	}

	CSvgStorage::~CSvgStorage()
	{
		Clear();
	}

	void CSvgStorage::Clear()
	{
		for (CObjectBase* pObject : m_arObjects)
			delete pObject;	
	}

	bool CSvgStorage::Empty() const
	{
		return m_arObjects.empty();
	}

	void CSvgStorage::AddObject(CObjectBase *pObject)
	{
		if (NULL != pObject)
			m_arObjects.push_back(pObject);
	}

	CObjectBase *CSvgStorage::GetFirstObject()
	{
		if (Empty())
			return NULL;

		return m_arObjects.front();
	}

	void CSvgStorage::AddStyle(const std::wstring& wsStyle)
	{
		m_oStyle.AddStyle(wsStyle);
	}

	CGeneralStyle *CSvgStorage::GetStyle()
	{
		return &m_oStyle;
	}

	bool CSvgStorage::Draw(IRenderer *pRenderer) const
	{
		for (CObjectBase* pObject : m_arObjects)
		{
			pObject->Draw(pRenderer);
		}

		return true;
	}
}
