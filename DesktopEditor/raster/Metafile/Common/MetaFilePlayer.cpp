#include "MetaFilePlayer.h"

namespace MetaFile
{
	template<typename CMetaFileDC> CMetaFilePlayer<CMetaFileDC>::CMetaFilePlayer(CMetaFileBase* pFile) : m_pFile(pFile)
	{
		CMetaFileDC* pDC = new CMetaFileDC();
		if (!pDC)
		{
			pFile->SetError();
			return;
		}

		m_pDC = pDC;
		m_vDCStack.push_back(pDC);

		InitStockObjects();
	};
	template<typename CMetaFileDC> CMetaFilePlayer<CMetaFileDC>::~CMetaFilePlayer()
	{
		for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
		{
			CMetaFileDC* pDC = m_vDCStack.at(nIndex);
			delete pDC;
		}
		m_vDCStack.clear();

		for (CMetaFileObjectMap::iterator oIterator = m_mObjects.begin(); oIterator != m_mObjects.end(); oIterator++)
		{
			CMetaFileObjectBase* pOldObject = oIterator->second;
			delete pOldObject;
		}
		m_mObjects.clear();
	}
	template<typename CMetaFileDC> void CMetaFilePlayer<CMetaFileDC>::Clear()
	{
		for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
		{
			CMetaFileDC* pDC = m_vDCStack.at(nIndex);
			delete pDC;
		}
		m_vDCStack.clear();

		for (CMetaFileObjectMap::iterator oIterator = m_mObjects.begin(); oIterator != m_mObjects.end(); oIterator++)
		{
			CMetaFileObjectBase* pOldObject = oIterator->second;
			delete pOldObject;
		}
		m_mObjects.clear();

		CMetaFileDC* pDC = new CMetaFileDC();
		if (!pDC)
		{
			m_pFile->SetError();
			return;
		}

		m_pDC = pDC;
		m_vDCStack.push_back(pDC);
		InitStockObjects();
	}
	template<typename CMetaFileDC> CMetaFileDC* CMetaFilePlayer<CMetaFileDC>::SaveDC()
	{
		if (!m_pDC)
		{
			m_pFile->SetError();
			return NULL;
		}

		CMetaFileDC* pNewDC = m_pDC->Copy();
		if (!pNewDC)
		{
			m_pFile->SetError();
			return NULL;
		}

		m_vDCStack.push_back(pNewDC);
		m_pDC = pNewDC;
		return pNewDC;
	}
	template<typename CMetaFileDC> CMetaFileDC* CMetaFilePlayer<CMetaFileDC>::RestoreDC()
	{
		if (m_vDCStack.size() <= 1)
		{
			m_pFile->SetError();
			return m_pDC;
		}

		CMetaFileDC* pDC = m_vDCStack.at(m_vDCStack.size() - 1);
		m_vDCStack.pop_back();
		delete pDC;

		pDC = m_vDCStack.at(m_vDCStack.size() - 1);
		m_pDC = pDC;
		return m_pDC;
	}
	template<typename CMetaFileDC> CMetaFileDC* CMetaFilePlayer<CMetaFileDC>::GetDC()
	{
		return m_pDC;
	}
	template<typename CMetaFileDC> void CMetaFilePlayer<CMetaFileDC>::RegisterObject(unsigned int ulIndex, CMetaFileObjectBase* pObject)
	{
		CMetaFileObjectMap::const_iterator oPos = m_mObjects.find(ulIndex);
		if (m_mObjects.end() != oPos)
		{
			CMetaFileObjectBase* pOldObject = oPos->second;
			delete pOldObject;
			m_mObjects.erase(ulIndex);
		}

		m_mObjects.insert(std::pair<unsigned int, CMetaFileObjectBase*>(ulIndex, pObject));
	}
	template<typename CMetaFileDC> void CMetaFilePlayer<CMetaFileDC>::SelectObject(unsigned int ulIndex)
	{
		CMetaFileObjectMap::const_iterator oPos = m_mObjects.find(ulIndex);
		if (m_mObjects.end() != oPos)
		{
			CMetaFileObjectBase* pObject = oPos->second;

			for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
			{
				CMetaFileDC* pDC = m_vDCStack.at(nIndex);

				switch (pObject->GetType())
				{
				case METAFILE_OBJECT_BRUSH: m_pDC->SetBrush(pObject); break;
				case METAFILE_OBJECT_FONT : m_pDC->SetFont(pObject); break;
				case METAFILE_OBJECT_PEN  : m_pDC->SetPen(pObject); break;
				}
			}
		}
	}
	template<typename CMetaFileDC> void CMetaFilePlayer<CMetaFileDC>::SelectPalette(unsigned int ulIndex)
	{
		CEmfObjectMap::const_iterator oPos = m_mObjects.find(ulIndex);
		if (m_mObjects.end() != oPos)
		{
			CEmfObjectBase* pObject = oPos->second;
			if (METAFILE_OBJECT_PALETTE == pObject->GetType())
				m_pDC->SetPalette(pObject);
		}
	}
	template<typename CMetaFileDC> void CMetaFilePlayer<CMetaFileDC>::DeleteObject(unsigned int ulIndex)
	{
		// TODO: —делать поиск по DC_BRUSH и DC_PEN

		CEmfObjectMap::const_iterator oPos = m_mObjects.find(ulIndex);
		if (m_mObjects.end() != oPos)
		{
			CEmfObjectBase* pObject = oPos->second;

			switch (pObject->GetType())
			{
			case METAFILE_OBJECT_BRUSH: m_pDC->RemoveBrush(pObject); break;
			case METAFILE_OBJECT_FONT: m_pDC->RemoveFont(pObject); break;
			case METAFILE_OBJECT_PEN: m_pDC->RemovePen(pObject); break;
			}

			delete pObject;
			m_mObjects.erase(ulIndex);
		}
	}
	template<typename CMetaFileDC> void CMetaFilePlayer<CMetaFileDC>::InitStockObjects()
	{
	}
}