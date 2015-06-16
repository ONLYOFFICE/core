#include "Destination.h"
#include "Pages.h"

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// CDestination
	//----------------------------------------------------------------------------------------
	CDestination::CDestination(CPage* pPage, CXref* pXref)
	{
		pXref->Add(this);

		// Первый элемент массива должен быть страницей, которой принадлежит объект
		Add((CObjectBase*)pPage);
		Add("Fit"); // Значение по умолчанию Fit
	}
	bool CDestination::IsValid() const
	{
		if (m_arrList.size() < 2)
			return false;

		CObjectBase* pObject = Get(0, false);
		if (object_type_DICT != pObject->GetType() || dict_type_PAGE != ((CDictObject*)pObject)->GetDictType())
			return false;

		return true;
	}
	void CDestination::PrepareArray()
	{
		CPage* pPage = (CPage*)Get(0);

		if (m_arrList.size() > 1)
		{
			Clear();
			Add((CObjectBase*)pPage);
		}
	}
	void CDestination::SetXYZ(float fLeft, float fTop, float fZoom)
	{
		if (!IsValid())
			return;

		// Если параметр приближения задан некорректно, тогда оставляем его нетронутым(что соответствует значению 0)
		if (fZoom < 0.08 || fZoom > 32)
			fZoom = 0;

		fLeft = max(fLeft, 0);
		fTop  = max(fTop, 0);

		PrepareArray();
		Add("XYZ");
		Add(fLeft);
		Add(fTop);
		Add(fZoom);
	}
	void CDestination::SetFit()
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("Fit");
	}
	void CDestination::SetFitH(float fTop)
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitH");
		Add(fTop);
	}
	void CDestination::SetFitV(float fLeft)
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitV");
		Add(fLeft);
	}
	void CDestination::SetFitR(float fLeft, float fBottom, float fRight, float fTop)
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitR");
		Add(fLeft);
		Add(fBottom);
		Add(fRight);
		Add(fTop);
	}
	void CDestination::SetFitB()
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitB");
	}
	void CDestination::SetFitBH(float fTop)
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitBH");
		Add(fTop);
	}
	void CDestination::SetFitBV(float fLeft)
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitBV");
		Add(fLeft);
	}
}

