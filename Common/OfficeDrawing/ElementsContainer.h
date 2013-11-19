#pragma once
#include "Elements.h"
//#include "PropertyParcer.h"

// класс, с нормальной структурой, не умеющий читать из файла себя
class CElementsContainer
{
public:
	// размеры в миллиметрах
	long m_lWidth;   
	long m_lHeight; 

	// а вот эти - "настоящие"(в логической системе координат), чтобы масштабировать
	long m_lOriginalWidth;
	long m_lOriginalHeight;

	double m_dDuration;
	double m_dCurrentTime;
	bool m_bDefaultDuration;

	double m_dStartTime;
	double m_dEndTime;
	
	CSimpleArray<IElement*> m_arElements;

	CSimpleArray<SColorAtom> m_arColorScheme;
	CSimpleArray<CFont_>* m_pFonts;

	// странно сюда вставлять то, что есть только в ППТ. но, иначе надо много классов менять.
	// а тут - одно лишнее поле - и порядок
	CAtlArray<BOOL>* m_parEmptyPictures;
	
public:

	CElementsContainer() : m_arElements(), m_arColorScheme(), m_pFonts(NULL)
	{
		m_lWidth = 210;
		m_lHeight = 190;

		m_lOriginalWidth = 0;
		m_lOriginalHeight = 0;

		m_dCurrentTime = 0.0;
		m_dDuration = 30000.0;

		m_dStartTime = 0.0;
		m_dEndTime = m_dDuration;
		m_bDefaultDuration = true;

		m_parEmptyPictures = NULL;
	}

	~CElementsContainer()
	{
		Clear();
	}

	void Clear()
	{
		int nCount = m_arElements.GetSize();
		for (int nIndex = nCount - 1; nIndex >= 0; --nIndex)
		{
			IElement* pElem = m_arElements[nIndex];
			m_arElements.RemoveAt(nIndex);
			RELEASEOBJECT(pElem);
		}
	}

	void NormalizeCoordinates()
	{
		// здесь:
		// x *= (m_lWidth / m_lOriginalWidth)
		// y *= (m_lHeight / m_lOriginalHeight)

		double dScaleX = (double)m_lWidth / m_lOriginalWidth;
		double dScaleY = (double)m_lHeight / m_lOriginalHeight;

		for (int nIndex = 0; nIndex < m_arElements.GetSize(); ++nIndex)
		{
			m_arElements[nIndex]->NormalizeCoords(dScaleX, dScaleY);

			CShapeElement* pElem = dynamic_cast<CShapeElement*>(m_arElements[nIndex]);
			if (NULL != pElem)
			{
				pElem->SetUpProperties(NULL, NULL);
			}
		}
	}

	CString ToXml()
	{
		CString str = _T("");
		str.Format(_T("<ColorSource Color='65535' Duration='%lf'/>"), m_dDuration);
		return str;
	}

	DWORD GetIndexPicture(DWORD lIndex)
	{
		if (NULL == m_parEmptyPictures)
			return lIndex;
		
		LONG lResult = 0;
		size_t nCount = m_parEmptyPictures->GetCount();

		if (lIndex > nCount)
			return 0;

		for (size_t nIndex = 0; nIndex < lIndex; ++nIndex)
		{
			if ((*m_parEmptyPictures)[nIndex])
				++lResult;
		}
		return lIndex - lResult;
	}
};
