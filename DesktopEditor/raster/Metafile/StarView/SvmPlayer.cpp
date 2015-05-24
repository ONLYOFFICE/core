
#include "SvmPlayer.h"
#include "SvmFile.h"

// MetaFile
#include "SvmEnums.h"
#include "SvmObjects.h"

#define DEBUG_SVMPAINT 0

// Flags for Push
#define PUSH_LINECOLOR                  ((USHORT)0x0001)
#define PUSH_FILLCOLOR                  ((USHORT)0x0002)
#define PUSH_FONT                       ((USHORT)0x0004)
#define PUSH_TEXTCOLOR                  ((USHORT)0x0008)
#define PUSH_MAPMODE                    ((USHORT)0x0010)
#define PUSH_CLIPREGION                 ((USHORT)0x0020)
#define PUSH_RASTEROP                   ((USHORT)0x0040)
#define PUSH_TEXTFILLCOLOR              ((USHORT)0x0080)
#define PUSH_TEXTALIGN                  ((USHORT)0x0100)
#define PUSH_REFPOINT                   ((USHORT)0x0200)
#define PUSH_TEXTLINECOLOR              ((USHORT)0x0400)
#define PUSH_TEXTLAYOUTMODE             ((USHORT)0x0800)
#define PUSH_TEXTLANGUAGE               ((USHORT)0x1000)
#define PUSH_OVERLINECOLOR              ((USHORT)0x2000)
#define PUSH_ALLTEXT                    (PUSH_TEXTCOLOR | PUSH_TEXTFILLCOLOR | PUSH_TEXTLINECOLOR | PUSH_OVERLINECOLOR | PUSH_TEXTALIGN | PUSH_TEXTLAYOUTMODE | PUSH_TEXTLANGUAGE)
#define PUSH_ALLFONT                    (PUSH_ALLTEXT | PUSH_FONT)
#define PUSH_ALL                        ((USHORT)0xFFFF)

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

	m_pFile = pFile;
	m_pDC = pDC;
	m_vDCStack.push_back(pDC);

	m_nFlags = 0;
	m_eRasterOp = ROP_OVERPAINT;
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
		m_pFile->SetError();
		return;
	}
	m_nFlags = 0;
	m_eRasterOp = ROP_OVERPAINT;
	m_ushIndex = 0;

	m_pDC = pDC;
	m_vDCStack.push_back(pDC);
	m_vAvailableIndexes.clear();

	InitStockObjects();
}
void CSvmPlayer::SelectObject(unsigned short ushIndex)
{
	CSvmObjectMap::const_iterator oPos = m_mObjects.find(ushIndex);
	if (m_mObjects.end() != oPos)
	{
		CSvmObjectBase* pObject = oPos->second;


		switch (pObject->GetType())
		{
		case SVM_OBJECT_BRUSH:	m_pDC->SetBrush((CSvmBrush*)pObject); break;
		case SVM_OBJECT_FONT:	m_pDC->SetFont((CSvmFont*)pObject); break;
		case SVM_OBJECT_PEN:	m_pDC->SetPen((CSvmPen*)pObject); break;
		}
	}
}
CSvmObjectBase *CSvmPlayer::GetLastObject (ESvmObjectType type)
{
	CSvmObjectMap::iterator oIterator = m_mObjects.end();
	oIterator--;
	for (; oIterator != m_mObjects.begin(); oIterator--)
	{
		if (oIterator->second->GetType() == type)
		{
			return oIterator->second;
		}
	}
	return NULL;
}
CSvmDC* CSvmPlayer::SaveDC()
{
	if (!m_pDC)
	{
		m_pFile->SetError();
		return NULL;
	}

	CSvmDC* pNewDC = m_pDC->Copy();
	if (!pNewDC)
	{
		m_pFile->SetError();
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
		m_pFile->SetError();
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
	// Найдем наименьший доступный индекс
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

	SelectObject(m_ushIndex-1);
}
void CSvmPlayer::SetRasterOp(int op)
{
	m_eRasterOp = (ESvnRasterOp)op;
}
void CSvmPlayer::InitStockObjects()
{
	InitStockBrush(false, 79, 129, 189); //default OnlyOffice
	InitStockPen(false, 0x00, 0x00, 0x00);
}
void CSvmPlayer::Pop()
{
	//if ( m_nFlags & PUSH_LINECOLOR )
	//{
	//	if ( m_nFlags->mpLineColor )
	//		SetLineColor( *m_nFlags->mpLineColor );
	//	else
	//		SetLineColor();
	//}
	//if ( m_nFlags & PUSH_FILLCOLOR )
	//{
	//	if ( m_nFlags->mpFillColor )
	//		SetFillColor( *m_nFlags->mpFillColor );
	//	else
	//		SetFillColor();
	//}
	//if ( m_nFlags & PUSH_FONT )
	//	SetFont( *pData->mpFont );
	//if ( m_nFlags & PUSH_TEXTCOLOR )
	//	SetTextColor( *pData->mpTextColor );
	//if ( m_nFlags & PUSH_TEXTFILLCOLOR )
	//{
	//	if ( pData->mpTextFillColor )
	//		SetTextFillColor( *pData->mpTextFillColor );
	//	else
	//		SetTextFillColor();
	//}
	//if ( m_nFlags & PUSH_TEXTLINECOLOR )
	//{
	//	if ( pData->mpTextLineColor )
	//		SetTextLineColor( *pData->mpTextLineColor );
	//	else
	//		SetTextLineColor();
	//}
	//if ( m_nFlags & PUSH_OVERLINECOLOR )
	//{
	//	if ( pData->mpOverlineColor )
	//		SetOverlineColor( *pData->mpOverlineColor );
	//	else
	//		SetOverlineColor();
	//}
	//if ( m_nFlags & PUSH_TEXTALIGN )
	//	SetTextAlign( pData->meTextAlign );
	//if( m_nFlags & PUSH_TEXTLAYOUTMODE )
	//	SetLayoutMode( pData->mnTextLayoutMode );
	//if( m_nFlags & PUSH_TEXTLANGUAGE )
	//	SetDigitLanguage( pData->meTextLanguage );
	//if ( m_nFlags & PUSH_RASTEROP )
	//	SetRasterOp( m_eRasterOp );
	//if ( m_nFlags & PUSH_MAPMODE )
	//{
	//	if ( pData->mpMapMode )
	//		SetMapMode( *pData->mpMapMode );
	//	else
	//		SetMapMode();
	//}
	if ( m_nFlags & PUSH_CLIPREGION )
	{
		GetDC()->GetClip()->ClipOnRenderer(m_pFile->m_pOutput);
		GetDC()->GetClip()->Reset();
	}

	m_nFlags = 0;
	//if ( m_nFlags & PUSH_REFPOINT )
	//{
	//	if ( pData->mpRefPoint )
	//		SetRefPoint( *pData->mpRefPoint );
	//	else
	//		SetRefPoint();
	//}
}

void CSvmPlayer::Push(int nFlags) // объекты с множественной настройкой 
{
	m_nFlags = nFlags;
	//if ( nFlags & PUSH_LINECOLOR )
	//{
	//	if ( mbLineColor )
	//		pData->mpLineColor = new Color( maLineColor );
	//	else
	//		pData->mpLineColor = NULL;
	//}
	//if ( nFlags & PUSH_FILLCOLOR )
	//{
	//	if ( mbFillColor )
	//		pData->mpFillColor = new Color( maFillColor );
	//	else
	//		pData->mpFillColor = NULL;
	//}
	//if ( nFlags & PUSH_FONT )
	//	pData->mpFont = new Font( maFont );
	//if ( nFlags & PUSH_TEXTCOLOR )
	//	pData->mpTextColor = new Color( GetTextColor() );
	//if ( nFlags & PUSH_TEXTFILLCOLOR )
	//{
	//	if ( IsTextFillColor() )
	//		pData->mpTextFillColor = new Color( GetTextFillColor() );
	//	else
	//		pData->mpTextFillColor = NULL;
	//}
	//if ( nFlags & PUSH_TEXTLINECOLOR )
	//{
	//	if ( IsTextLineColor() )
	//		pData->mpTextLineColor = new Color( GetTextLineColor() );
	//	else
	//		pData->mpTextLineColor = NULL;
	//}
	//if ( nFlags & PUSH_OVERLINECOLOR )
	//{
	//	if ( IsOverlineColor() )
	//		pData->mpOverlineColor = new Color( GetOverlineColor() );
	//	else
	//		pData->mpOverlineColor = NULL;
	//}
	//if ( nFlags & PUSH_TEXTALIGN )
	//	pData->meTextAlign = GetTextAlign();
	//if( nFlags & PUSH_TEXTLAYOUTMODE )
	//	pData->mnTextLayoutMode = GetLayoutMode();
	//if( nFlags & PUSH_TEXTLANGUAGE )
	//	pData->meTextLanguage = GetDigitLanguage();
	//if ( nFlags & PUSH_RASTEROP )
	//	pData->meRasterOp = GetRasterOp();
	//if ( nFlags & PUSH_MAPMODE )
	//{
	//	if ( mbMap )
	//		pData->mpMapMode = new MapMode( maMapMode );
	//	else
	//		pData->mpMapMode = NULL;
	//}
	if ( nFlags & PUSH_CLIPREGION )
	{
		GetDC()->GetClip()->Reset();
		//new region
	}
	//if ( nFlags & PUSH_REFPOINT )
	//{
	//	if ( mbRefPoint )
	//		pData->mpRefPoint = new Point( maRefPoint );
	//	else
	//		pData->mpRefPoint = NULL;
	//}
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
	m_ulMapMode = MAP_POINT;
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
	pNewDC->m_oClip = m_oClip;
	pNewDC->m_unArcDirection = m_unArcDirection;

	return pNewDC;
}
void CSvmDC::SetMapMode(MapMode & mapMode)
{
	m_ulMapMode = (ESvmMapUnit)mapMode.unit;

	switch (m_ulMapMode)
	{
	case MAP_MM:	// 1 unit = 1 mm
	{
		double dPixel = 1. * 72 / 25.4;
		SetPixelWidth(dPixel);
		SetPixelHeight(dPixel);		
	}break;
	case MAP_CM:	// 1 unit = 1 cm = 10 mm
	{
		double dPixel = 10. * 72 / 25.4;
		SetPixelWidth(dPixel);
		SetPixelHeight(dPixel);		
	}break;
	case MAP_100TH_MM:
	case MAP_10TH_MM:
	case MAP_1000TH_INCH:
	case MAP_100TH_INCH:
	case MAP_10TH_INCH:
		//хз
		break;
	case MAP_INCH:	// 1 unit = 1 inch
	{
		double dPixel = 1. * 72;
		SetPixelWidth(dPixel);
		SetPixelHeight(dPixel);		
	}break;
	case MAP_POINT:
	{
		SetPixelWidth(1);
		SetPixelHeight(1);		
	}break;
	case MAP_TWIP: // 1 unit = 1/1440 inch
	{
		SetPixelWidth(0.05);
		SetPixelHeight(0.05);		
	}break;
	case MAP_PIXEL:
	{
		double dPixel = 1. * 3. /4.;
		SetPixelWidth(dPixel);
		SetPixelHeight(dPixel);		
	}break;
	case MAP_SYSFONT:
	case MAP_APPFONT:
		//хз
		break;
	case MAP_RELATIVE:
		UpdatePixelMetrics();
		break;
	case MAP_LASTENUMDUMMY:
		break;
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
CSvmClip* CSvmDC::GetClip()
{
	return &m_oClip;
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
	if 	(m_ulMapMode == MAP_RELATIVE)
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

