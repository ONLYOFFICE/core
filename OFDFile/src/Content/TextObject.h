#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include "IPageBlock.h"
#include "GraphicUnit.h"

#include "../Types/Color.h"

namespace OFD
{
struct TCGTransform
{
	unsigned int m_unCodePosition;
	unsigned int m_unCodeCount;
	unsigned int m_unGlyphCount;

	std::vector<unsigned int> m_arGlyphs;

	static TCGTransform Read(CXmlReader& oLiteReader);

	bool Draw(IRenderer* pRenderer, unsigned int& unIndex, double dX, double dY) const;
};

class CTextCode
{
	double m_dX;
	double m_dY;

	std::vector<double> m_arDeltaX;
	std::vector<double> m_arDeltaY;

	std::wstring m_wsText;
public:
	CTextCode(CXmlReader& oLiteReader);

	void Draw(IRenderer* pRenderer, unsigned int& unIndex, const std::vector<TCGTransform>& arCGTransforms) const;
};

class CTextObject : public IPageBlock, public CGraphicUnit
{
	double m_dSize;
	bool m_bStroke;
	bool m_bFill;
	double m_dHScale;
	unsigned int m_unReadDirection;
	unsigned int m_unCharDirection;
	unsigned int m_unWeight;
	bool m_bItalic;

	CColor* m_pFillColor;
	CColor* m_pStrokeColor;

	const CFont* m_pFont;

	std::vector<const CTextCode*> m_arTextCodes;
	std::vector<TCGTransform>     m_arCGTransforms;

	NSFonts::IFontManager* m_pFontManager;
public:
	CTextObject(CXmlReader& oLiteReader, const CRes* pPublicRes, NSFonts::IFontManager* pFontManager);
	~CTextObject();

	void Draw(IRenderer* pRenderer) const override;
};
}

#endif // TEXTOBJECT_H
