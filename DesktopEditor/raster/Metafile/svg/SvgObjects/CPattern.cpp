#include "CPattern.h"
#include <iostream>

#include "../../../graphics/pro/Graphics.h"

namespace SVG
{
	CPattern::CPattern(XmlUtils::CXmlNode& oNode, NSFonts::IFontManager *pFontManager)
		: CAppliedObject(oNode), m_oContainer(oNode), m_pFontManager(pFontManager), m_pImage(NULL), m_enPatternUnits(objectBoundingBox)
	{}

	CPattern::~CPattern()
	{
		RELEASEINTERFACE(m_pImage);
	}

	void CPattern::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"patternUnits"))
		{
			const std::wstring& wsValue = mAttributes.at(L"patternUnits");

			if (L"objectBoundingBox" == wsValue)
				m_enPatternUnits = objectBoundingBox;
			else if (L"userSpaceOnUse" == wsValue)
				m_enPatternUnits = userSpaceOnUse;
		}
	}

	void CPattern::Update(const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		if (NULL != m_pImage)
			RELEASEINTERFACE(m_pImage);

		NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();

		pGrRenderer->SetFontManager(m_pFontManager);

		double dMMtoPx = 96. / 25.4;

		double dKoefWidth  = m_oContainer.m_oWindow.m_oWidth  .ToDouble(NSCSS::Pixel);
		double dKoefHeight = m_oContainer.m_oWindow.m_oHeight .ToDouble(NSCSS::Pixel);

		if (objectBoundingBox == m_enPatternUnits)
		{
			dKoefWidth  *= oObjectBounds.m_dRight  - oObjectBounds.m_dLeft;
			dKoefHeight *= oObjectBounds.m_dBottom - oObjectBounds.m_dTop;
		}

		int nWidth  = dKoefWidth  * dMMtoPx + 1;
		int nHeight = dKoefHeight * dMMtoPx + 1;

		if (0 == nWidth || 0 == nHeight)
			return;

		BYTE* pBgraData = new(std::nothrow) BYTE[nWidth * nHeight * 4];

		if (!pBgraData)
			return;

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

		pGrRenderer->SetTransform(dMMtoPx, 0., 0., dMMtoPx, 0., 0.);

		//Отрисовка
		m_oContainer.Draw(pGrRenderer, pFile);

		pGrRenderer->EndCommand(c_nImageType);
		RELEASEINTERFACE(pGrRenderer);

		oFrame.put_Data(NULL);

		m_pImage = new Aggplus::CImage;
		m_pImage->Create(pBgraData, oFrame.get_Width(), oFrame.get_Height(), oFrame.get_Stride());
	}

	bool CPattern::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		Update(pFile, oObjectBounds);

		if (NULL == pRenderer || NULL == m_pImage)
			return false;

		pRenderer->put_BrushType(c_BrushTypeTexture);
		pRenderer->put_BrushTextureMode(c_BrushTextureModeTile);
		pRenderer->put_BrushTextureImage(m_pImage);

		RELEASEINTERFACE(m_pImage);

		return true;
	}

	CGraphicsContainer &CPattern::GetContainer()
	{
		return m_oContainer;
	}
}
