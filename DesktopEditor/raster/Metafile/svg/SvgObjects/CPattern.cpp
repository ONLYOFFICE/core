#include "CPattern.h"
#include <iostream>

#include "../../../graphics/pro/Graphics.h"

namespace SVG
{
	CPattern::CPattern(CObjectBase *pParent, NSFonts::IFontManager *pFontManager)
	    : CContainer(pParent), m_pFontManager(pFontManager)
	{}

	bool CPattern::DrawDef(IRenderer *pRenderer, const TRect &oParentRect, CDefs *pDefs) const
	{
		NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();

		pGrRenderer->SetFontManager(m_pFontManager);

		int nWidth  = oParentRect.m_oWidth.ToDouble()  * m_oWindow.m_oWidth.ToDouble();
		int nHeight = oParentRect.m_oHeight.ToDouble() * m_oWindow.m_oHeight.ToDouble();

		if (0 == nWidth || 0 == nHeight)
			return false;

		BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];

		if (!pBgraData)
			return false;

		unsigned int alfa = 0xffffff;
		//дефолтный тон должен быть прозрачным, а не белым
		//memset(pBgraData, 0xff, nWidth * nHeight * 4);
		for (int i = 0; i < nWidth * nHeight; i++)
			((unsigned int*)pBgraData)[i] = alfa;

		CBgraFrame oFrame;
		oFrame.put_Data(pBgraData);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nHeight);
		oFrame.put_Stride(-4 * nWidth);

		pGrRenderer->CreateFromBgraFrame(&oFrame);
		pGrRenderer->put_Width(nWidth);
		pGrRenderer->put_Height(nHeight);

		pGrRenderer->SetSwapRGB(false);
		pGrRenderer->BeginCommand(c_nImageType);

		//Отрисовка
		CContainer::Draw(pGrRenderer, pDefs);

		pGrRenderer->EndCommand(c_nImageType);

		oFrame.put_Data(NULL);

		Aggplus::CImage oImage;
		oImage.Create(pBgraData, nWidth, nHeight, -4 * nWidth);

		for (double dX = oParentRect.m_oX.ToDouble(); dX < oParentRect.m_oWidth.ToDouble(); dX += nWidth)
			for (double dY = oParentRect.m_oY.ToDouble(); dY < oParentRect.m_oHeight.ToDouble(); dY += nHeight)
				pRenderer->DrawImage(&oImage, dX, dY, nWidth, nHeight);

		RELEASEINTERFACE(pGrRenderer);

		return true;
	}
}
