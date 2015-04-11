#include "EmfPlayer.h"
#include "EmfFile.h"

namespace Metafile
{
	CEmfPlayer::CEmfPlayer(CEmfFile* pFile)
	{
		CEmfDC* pDC = new CEmfDC();
		if (!pDC)
		{
			pFile->SetError();
			return;
		}

		m_pDC = pDC;
		m_vDCStack.push_back(pDC);
	};
	CEmfPlayer::~CEmfPlayer()
	{
		for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
		{
			CEmfDC* pDC = m_vDCStack.at(nIndex);
			delete pDC;
		}

		for (CEmfObjectMap::iterator oIterator = m_mObjects.begin(); oIterator != m_mObjects.end(); oIterator++)
		{
			CEmfObjectBase* pOldObject = oIterator->second;
			delete pOldObject;
		}
		m_mObjects.clear();
	}
	void CEmfPlayer::Clear()
	{
		for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
		{
			CEmfDC* pDC = m_vDCStack.at(nIndex);
			delete pDC;
		}

		for (CEmfObjectMap::iterator oIterator = m_mObjects.begin(); oIterator != m_mObjects.end(); oIterator++)
		{
			CEmfObjectBase* pOldObject = oIterator->second;
			delete pOldObject;
		}
		m_mObjects.clear();

		CEmfDC* pDC = new CEmfDC();
		if (!pDC)
		{
			m_pEmfFile->SetError();
			return;
		}

		m_pDC = pDC;
		m_vDCStack.push_back(pDC);
	}
	CEmfDC* CEmfPlayer::SaveDC()
	{
		if (!m_pDC)
		{
			m_pEmfFile->SetError();
			return NULL;
		}

		CEmfDC* pNewDC = m_pDC->Copy();
		if (!pNewDC)
		{
			m_pEmfFile->SetError();
			return NULL;
		}

		m_vDCStack.push_back(pNewDC);
		m_pDC = pNewDC;
		return pNewDC;
	}
	CEmfDC* CEmfPlayer::RestoreDC()
	{
		if (m_vDCStack.size() <= 1)
		{
			m_pEmfFile->SetError();
			return m_pDC;
		}

		CEmfDC* pDC = m_vDCStack.at(m_vDCStack.size() - 1);
		m_vDCStack.pop_back();
		delete pDC;

		pDC = m_vDCStack.at(m_vDCStack.size() - 1);
		m_pDC = pDC;
		return m_pDC;
	}
	CEmfDC* CEmfPlayer::GetDC()
	{
		return m_pDC;
	}
	void CEmfPlayer::RegisterObject(unsigned long ulIndex, CEmfObjectBase* pObject)
	{
		CEmfObjectMap::const_iterator oPos = m_mObjects.find(ulIndex);
		if (m_mObjects.end() != oPos)
		{ 
			CEmfObjectBase* pOldObject = oPos->second;
			delete pOldObject;
			m_mObjects.erase(ulIndex);
		}

		m_mObjects.insert(std::pair<unsigned long, CEmfObjectBase*>(ulIndex, pObject));
	}
	void CEmfPlayer::SelectObject(unsigned long ulIndex)
	{
		CEmfObjectMap::const_iterator oPos = m_mObjects.find(ulIndex);
		if (m_mObjects.end() != oPos)
		{
			CEmfObjectBase* pObject = oPos->second;

			switch (pObject->GetType())
			{
				case EMF_OBJECT_BRUSH: m_pDC->SetBrush((CEmfLogBrushEx*)pObject); break;
				case EMF_OBJECT_FONT: m_pDC->SetFont((CEmfLogFont*)pObject); break;
			}
		}
	}

	CEmfDC::CEmfDC()
	{
		m_oTransform.Init();
		m_oTextColor.Init();
		m_pBrush = NULL;
	}
	CEmfDC::~CEmfDC()
	{
	}
	CEmfDC* CEmfDC::Copy()
	{
		CEmfDC* pNewDC = new CEmfDC();
		if (!pNewDC)
			return NULL;

		pNewDC->m_oTransform.Copy(&m_oTransform);
		pNewDC->m_oTextColor.Copy(&m_oTextColor);
		pNewDC->m_pBrush = m_pBrush;

		return pNewDC;
	}
	TEmfXForm* CEmfDC::GetTransform()
	{
		return &m_oTransform;
	}
	void CEmfDC::SetTextColor(TEmfColor& oColor)
	{
		m_oTextColor.Copy(&oColor);
	}
	TEmfColor& CEmfDC::GetTextColor()
	{
		return m_oTextColor;
	}
	void CEmfDC::SetBrush(CEmfLogBrushEx* pBrush)
	{
		m_pBrush = pBrush;
	}
	CEmfLogBrushEx* CEmfDC::GetBrush()
	{
		return m_pBrush;
	}
	void CEmfDC::SetFont(CEmfLogFont* pFont)
	{
		m_pFont = pFont;
	}
	CEmfLogFont* CEmfDC::GetFont()
	{
		return m_pFont;
	}
	void CEmfDC::SetTextAlign(unsigned long ulAlign)
	{
		m_ulTextAlign = ulAlign;
	}
	unsigned long CEmfDC::GetTextAlign()
	{
		return m_ulTextAlign;
	}
	void CEmfDC::SetBgMode(unsigned long ulBgMode)
	{
		m_ulBgMode = ulBgMode;
	}
	unsigned long CEmfDC::GetBgMode()
	{
		return m_ulBgMode;
	}
	void CEmfDC::SetBgColor(TEmfColor& oColor)
	{
		m_oBgColor.Copy(&oColor);
	}
	TEmfColor& CEmfDC::GetBgColor()
	{
		return m_oBgColor;
	}
}