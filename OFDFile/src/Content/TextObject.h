#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include "IPageBlock.h"
#include "GraphicUnit.h"

#include "../Types/Color.h"

namespace OFD
{
class CTextCode
{
	double m_dX;
	double m_dY;

	std::vector<double> m_arDeltaX;
	std::vector<double> m_arDeltaY;

	std::wstring m_wsText;
public:
	CTextCode(CXmlReader& oLiteReader);
};

class CTextObject : public IPageBlock, public CGraphicUnit
{
	unsigned int m_unFont;
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

	std::vector<CTextCode*> m_arTextCodes;
public:
	CTextObject(CXmlReader& oLiteReader);
	~CTextObject();

	virtual bool Read(CXmlReader& oLiteReader) override;
};
}

#endif // TEXTOBJECT_H
