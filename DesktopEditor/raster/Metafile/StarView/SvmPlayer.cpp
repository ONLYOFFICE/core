
#include "SvmPlayer.h"
#include "SvmFile.h"



// MetaFile
#include "SvmEnums.h"
#include "SvmObjects.h"

#define DEBUG_SVMPAINT 0


/**
   Namespace for StarView Metafile (SVM) classes
*/
namespace MetaFile
{

	CSvmPlayer::CSvmPlayer(CSvmFile* pFile)
	{
		CSvmDC* pDC = new CSvmDC();
		if (!pDC)
		{
			pFile->SetError();
			return;
		}

		m_pDC = pDC;
		m_vDCStack.push_back(pDC);
		m_ushIndex = 0;

		InitStockObjects();
	}

CSvmPlayer::~CSvmPlayer()
{
	for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
	{
		CSvmDC* pDC = m_vDCStack.at(nIndex);
		delete pDC;
	}
	m_vDCStack.clear();

	for (CSvmObjectMap::iterator oIterator = m_mObjects.begin(); oIterator != m_mObjects.end(); oIterator++)
	{
		CSvmObjectBase* pOldObject = oIterator->second;
		delete pOldObject;
	}
	m_mObjects.clear();
}

void    CSvmPlayer::Clear()
{
	for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
	{
		CSvmDC* pDC = m_vDCStack.at(nIndex);
		delete pDC;
	}
	m_vDCStack.clear();

	for (CSvmObjectMap::iterator oIterator = m_mObjects.begin(); oIterator != m_mObjects.end(); oIterator++)
	{
		CSvmObjectBase* pOldObject = oIterator->second;
		delete pOldObject;
	}
	m_mObjects.clear();

	CSvmDC* pDC = new CSvmDC();
	if (!pDC)
	{
		m_pSvmFile->SetError();
		return;
	}

	m_pDC = pDC;
	m_vDCStack.push_back(pDC);
	m_ushIndex = 0;
	m_vAvailableIndexes.clear();

	InitStockObjects();

	SelectObject(0x00000000); // BLACK_PEN
	SelectObject(0x00000001); // WHITE_BRUSH
}
void CSvmPlayer::SelectObject(unsigned short ushIndex)
{
	CSvmObjectMap::const_iterator oPos = m_mObjects.find(ushIndex);
	if (m_mObjects.end() != oPos)
	{
		CSvmObjectBase* pObject = oPos->second;


		switch (pObject->GetType())
		{
		case SVM_OBJECT_BRUSH: m_pDC->SetBrush((CSvmBrush*)pObject); break;
		case SVM_OBJECT_FONT: m_pDC->SetFont((CSvmFont*)pObject); break;
		case SVM_OBJECT_PEN: m_pDC->SetPen((CSvmPen*)pObject); break;
		}
	}
}
CSvmDC* CSvmPlayer::SaveDC()
{
	if (!m_pDC)
	{
		m_pSvmFile->SetError();
		return NULL;
	}

	CSvmDC* pNewDC = m_pDC->Copy();
	if (!pNewDC)
	{
		m_pSvmFile->SetError();
		return NULL;
	}

	m_vDCStack.push_back(pNewDC);
	m_pDC = pNewDC;
	return pNewDC;
}
CSvmDC* CSvmPlayer::RestoreDC()
{
	if (m_vDCStack.size() <= 1)
	{
		m_pSvmFile->SetError();
		return m_pDC;
	}

	CSvmDC* pDC = m_vDCStack.at(m_vDCStack.size() - 1);
	m_vDCStack.pop_back();
	delete pDC;

	pDC = m_vDCStack.at(m_vDCStack.size() - 1);
	m_pDC = pDC;
	return m_pDC;
}
CSvmDC* CSvmPlayer::GetDC()
{
	return m_pDC;
}

void CSvmPlayer::RegisterObject(CSvmObjectBase* pObject)
{
	// Ќайдем наименьший доступный индекс
	unsigned short ushIndex = m_ushIndex;
	int nAvailableIndex = -1;
	if (m_vAvailableIndexes.size() > 0)
	{
		for (std::size_t nIterator = 0; nIterator != m_vAvailableIndexes.size(); ++nIterator)
		{
			if (ushIndex > m_vAvailableIndexes[nIterator])
			{
				ushIndex = m_vAvailableIndexes[nIterator];
				nAvailableIndex = (int)nIterator;
			}
		}
	}

	CSvmObjectMap::const_iterator oPos = m_mObjects.find(ushIndex);
	if (m_mObjects.end() != oPos)
	{
		CSvmObjectBase* pOldObject = oPos->second;
		delete pOldObject;
		m_mObjects.erase(ushIndex);
	}

	m_mObjects.insert(std::pair<unsigned int, CSvmObjectBase*>(ushIndex, pObject));

	if (-1 != nAvailableIndex)
		m_vAvailableIndexes.erase(m_vAvailableIndexes.begin() + nAvailableIndex);

	if (ushIndex == m_ushIndex)
		m_ushIndex++;
}
void CSvmPlayer::InitStockObjects()
{
	InitStockBrush(false, 0x40, 0x40, 0x40);
	InitStockPen(false, 0x00, 0x00, 0x00);
}
void  CSvmPlayer::InitStockBrush(bool bNull, unsigned char r, unsigned char g, unsigned char b)
{
	CSvmBrush* pBrush = new CSvmBrush();
	if (!pBrush)
		return;

	if (bNull)
		pBrush->BrushStyle = BS_NULL;
	else
	{
		pBrush->BrushStyle = BS_SOLID;
		pBrush->Color.Set(r, g, b);
	}

	RegisterObject((CSvmObjectBase*)pBrush);
}
void    CSvmPlayer::InitStockPen(bool bNull, unsigned char r, unsigned char g, unsigned char b)
{
	CSvmPen* pPen = new CSvmPen();
	if (!pPen)
		return;

	if (bNull)
		pPen->PenStyle = PS_NULL;
	else
	{
		pPen->PenStyle = PS_COSMETIC | PS_SOLID;
		pPen->Color.Set(r, g, b);
	}

	RegisterObject((CSvmObjectBase*)pPen);
}

//----------------------------------------------------------------------------------------
CSvmDC::CSvmDC()
{
	m_ulMapMode = MM_TEXT;
	m_pBrush    = NULL;
	m_pPen      = NULL;
	m_pFont     = NULL;
	m_oTransform.Init();
	m_oInverseTransform.Init();
	m_oTextColor.Init();
	m_oBgColor.Init();
	m_ulTextAlign   = TA_TOP | TA_LEFT | TA_NOUPDATECP;
	m_ulBgMode      = TRANSPARENT;
	m_ulMiterLimit  = 0;
	m_ulFillMode    = WINDING;
	m_ulStretchMode = 0;
	m_oWindow.Init();
	m_oViewport.Init();
	m_dPixelHeight = 1;
	m_dPixelWidth  = 1;
	m_oCurPos.x = 0;
	m_oCurPos.y = 0;
	m_unArcDirection = AD_COUNTERCLOCKWISE;
}
CSvmDC::~CSvmDC()
{
}
CSvmDC*         CSvmDC::Copy()
{
	CSvmDC* pNewDC = new CSvmDC();
	if (!pNewDC)
		return NULL;

	pNewDC->m_ulMapMode = m_ulMapMode;
	pNewDC->m_pBrush = m_pBrush;
	pNewDC->m_pPen = m_pPen;
	pNewDC->m_pFont = m_pFont;
	//pNewDC->m_pPalette = m_pPalette;
	pNewDC->m_oTransform.Copy(&m_oTransform);
	pNewDC->m_oInverseTransform.Copy(&m_oInverseTransform);
	pNewDC->m_oTextColor.Copy(m_oTextColor);
	pNewDC->m_oBgColor.Copy(m_oBgColor);
	pNewDC->m_ulTextAlign = m_ulTextAlign;
	pNewDC->m_ulBgMode = m_ulBgMode;
	pNewDC->m_ulMiterLimit = m_ulMiterLimit;
	pNewDC->m_ulFillMode = m_ulFillMode;
	pNewDC->m_ulStretchMode = m_ulStretchMode;
	pNewDC->m_ulRop2Mode = m_ulRop2Mode;
	pNewDC->m_dPixelHeight = m_dPixelHeight;
	pNewDC->m_dPixelWidth = m_dPixelWidth;
	pNewDC->m_oWindow.Copy(&m_oWindow);
	pNewDC->m_oViewport.Copy(&m_oViewport);
	pNewDC->m_oCurPos = m_oCurPos;
	//pNewDC->m_oClip = m_oClip;
	pNewDC->m_unArcDirection = m_unArcDirection;

	return pNewDC;
}
void CSvmDC::SetMapMode(MapMode & mapMode)
{
	m_ulMapMode = mapMode.isSimple ? MM_TWIPS : MM_TWIPS;

	switch (m_ulMapMode)
	{
		case MM_TEXT: // 1 unit = 1pt
		{
			SetPixelWidth(1);
			SetPixelHeight(1);
			break;
		}
		case MM_LOMETRIC: // 1 unit = 0.1mm
		{
			double dPixel = 0.1 * 72 / 25.4;
			SetPixelWidth(dPixel);
			SetPixelHeight(dPixel);
			break;
		}
		case MM_HIMETRIC: // 1 unit = 0.01mm
		{
			double dPixel = 0.01 * 72 / 25.4;
			SetPixelWidth(dPixel);
			SetPixelHeight(dPixel);
			break;
		}
		case MM_LOENGLISH: // 1 unit = 0.01 inch
		{
			double dPixel = 0.01 * 72;
			SetPixelWidth(dPixel);
			SetPixelHeight(dPixel);
			break;
		}
		case MM_HIENGLISH: // 1 unit = 0.001 inch
		{
			double dPixel = 0.001 * 72;
			SetPixelWidth(dPixel);
			SetPixelHeight(dPixel);
			break;
		}
		case MM_TWIPS: // 1 unit = 1/1440 inch
		{
			SetPixelWidth(0.05);
			SetPixelHeight(0.05);
			break;
		}
		case MM_ISOTROPIC:
		case MM_ANISOTROPIC:
		{
			UpdatePixelMetrics();
			break;
		}
	}
}

TXForm* CSvmDC::GetTransform()
{
	return &m_oTransform;
}

TXForm* CSvmDC::GetInverseTransform()
{
	return &m_oTransform;
}

void CSvmDC::SetTextColor(TSvmColor& oColor)
{
	m_oTextColor.Copy(oColor);
}
TSvmColor&  CSvmDC::GetTextColor()
{
	return m_oTextColor;
}
void CSvmDC::SetBrush(CSvmBrush* pBrush)
{
	m_pBrush = pBrush;
}
void CSvmDC::RemoveBrush(CSvmBrush* pBrush)
{
	if (pBrush == m_pBrush)
		m_pBrush = NULL;
}
CSvmBrush* CSvmDC::GetBrush()
{
	return m_pBrush;
}
void CSvmDC::SetFont(CSvmFont* pFont)
{
	m_pFont = pFont;
}
void CSvmDC::RemoveFont(CSvmFont* pFont)
{
	if (pFont == m_pFont)
		m_pFont = NULL;
}
CSvmFont* CSvmDC::GetFont()
{
	return m_pFont;
}
void CSvmDC::SetTextAlign(unsigned int ulAlign)
{
	m_ulTextAlign = ulAlign;
}
unsigned int CSvmDC::GetTextAlign()
{
	return m_ulTextAlign;
}
void CSvmDC::SetBgMode(unsigned int ulBgMode)
{
	m_ulBgMode = ulBgMode;
}
unsigned int CSvmDC::GetBgMode()
{
	return m_ulBgMode;
}
void CSvmDC::SetBgColor(TSvmColor& oColor)
{
	m_oBgColor.Copy(oColor);
}
void CSvmDC::SetBgColor(TSvmColor* oColor)
{
	m_oBgColor.Copy(*oColor);
}
TSvmColor& CSvmDC::GetBgColor()
{
	return m_oBgColor;
}
void CSvmDC::SetMiterLimit(unsigned int ulMiter)
{
	m_ulMiterLimit = ulMiter;
}
unsigned int CSvmDC::GetMiterLimit()
{
	return m_ulMiterLimit;
}
void CSvmDC::SetFillMode(unsigned int ulFillMode)
{
	m_ulFillMode = ulFillMode;
}
unsigned int    CSvmDC::GetFillMode()
{
	return m_ulFillMode;
}
void CSvmDC::SetPen(CSvmPen* pPen)
{
	m_pPen = pPen;
}
void CSvmDC::RemovePen(CSvmPen* pPen)
{
	if (pPen == m_pPen)
		m_pPen = NULL;
}
CSvmPen* CSvmDC::GetPen()
{
	return m_pPen;
}
void CSvmDC::SetStretchMode(unsigned int& oMode)
{
	m_ulStretchMode = oMode;
}
unsigned int CSvmDC::GetStretchMode()
{
	return m_ulStretchMode;
}
double          CSvmDC::GetPixelWidth()
{
	return m_dPixelWidth;
}
double          CSvmDC::GetPixelHeight()
{
	return m_dPixelHeight;
}
void            CSvmDC::SetPixelWidth(double dPixelW)
{
	m_dPixelWidth = dPixelW;
}
void            CSvmDC::SetPixelHeight(double dPixelH)
{
	m_dPixelHeight = dPixelH;
}
void CSvmDC::SetWindowOrigin(TSvmPoint& oPoint)
{
	m_oWindow.lX = oPoint.x;
	m_oWindow.lY = oPoint.y;
	UpdatePixelMetrics();
}
void  CSvmDC::SetWindowExtents(TSvmSize& oPoint)
{
	m_oWindow.ulW = oPoint.cx;
	m_oWindow.ulH = oPoint.cy;
	UpdatePixelMetrics();
}
TSvmWindow* CSvmDC::GetWindow()
{
	return &m_oWindow;
}
void CSvmDC::SetViewportOrigin(TSvmPoint& oPoint)
{
	m_oViewport.lX = oPoint.x;
	m_oViewport.lY = oPoint.y;
	UpdatePixelMetrics();
}
void CSvmDC::SetViewportExtents(TSvmSize& oPoint)
{
	m_oViewport.ulW = oPoint.cx;
	m_oViewport.ulH = oPoint.cy;
	UpdatePixelMetrics();
}
TSvmWindow* CSvmDC::GetViewport()
{
	return &m_oViewport;
}
bool CSvmDC::UpdatePixelMetrics()
{
	unsigned int ulMapMode = m_ulMapMode;
	if (MM_ISOTROPIC == ulMapMode)
	{
		if (0 == m_oWindow.ulW || 0 == m_oViewport.ulW)
			return false;

		double dPixel = (double)m_oViewport.ulW / (double)m_oWindow.ulW;
		SetPixelHeight(dPixel);
		SetPixelWidth(dPixel);
	}
	else if (MM_ANISOTROPIC == ulMapMode)
	{
		double dPixelX = (double)m_oViewport.ulW / (double)m_oWindow.ulW;
		double dPixelY = (double)m_oViewport.ulH / (double)m_oWindow.ulH;

		SetPixelWidth(dPixelX);
		SetPixelHeight(dPixelY);
	}

	return true;
}
void            CSvmDC::SetRop2Mode(unsigned int& nMode)
{
	m_ulRop2Mode = nMode;
}
unsigned int    CSvmDC::GetRop2Mode()
{
	return m_ulRop2Mode;
}
//void            CSvmDC::SetPalette(CSvmPalette* pPalette)
//{
//	m_pPalette = pPalette;
//}
//void            CSvmDC::RemovePalette(CSvmPalette* pPalette)
//{
//	if (m_pPalette == pPalette)
//		m_pPalette = NULL;
//}
//CSvmPalette* CSvmDC::GetPalette()
//{
//	return m_pPalette;
//}
void CSvmDC::SetCurPos(TSvmPoint& oPoint)
{
	SetCurPos(oPoint.x, oPoint.y);
}
void CSvmDC::SetCurPos(int lX, int lY)
{
	m_oCurPos.x = lX;
	m_oCurPos.y = lY;
}
TSvmPoint & CSvmDC::GetCurPos()
{
	return m_oCurPos;
}
//CSvmClip* CSvmDC::GetClip()
//{
//	return &m_oClip;
//}
//void CSvmDC::ClipToPath(CSvmPath* pPath, unsigned int unMode)
//{
//	m_oClip.SetPath(pPath, unMode);
//}
void CSvmDC::SetArcDirection(unsigned int unDirection)
{
	m_unArcDirection = unDirection;
}
unsigned int CSvmDC::GetArcDirection()
{
	return m_unArcDirection;
}


}

