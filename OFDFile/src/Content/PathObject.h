#ifndef PATHOBJECT_H
#define PATHOBJECT_H

#include "IPageBlock.h"
#include "GraphicUnit.h"

#include "../Types/Color.h"

namespace OFD
{
class IPathElement
{
public:
	IPathElement(){};
	virtual ~IPathElement(){};

	static IPathElement* ReadFromArray(std::vector<std::string>& arValues) { return nullptr; };
};

class CStartElement : public IPathElement
{
	double m_dX;
	double m_dY;
public:
	CStartElement();
	static IPathElement* ReadFromArray(std::vector<std::string>& arValues);
};

class CMoveElement : public IPathElement
{
	double m_dX;
	double m_dY;
public:
	CMoveElement();
	static IPathElement* ReadFromArray(std::vector<std::string>& arValues);
};

class CLineElement :  public IPathElement
{
	double m_dX;
	double m_dY;
public:
	CLineElement();
	static IPathElement* ReadFromArray(std::vector<std::string>& arValues);
};

class CBezierCurve2Element : public IPathElement
{
	double m_dX1;
	double m_dY1;
	double m_dX2;
	double m_dY2;
public:
	CBezierCurve2Element();
	static IPathElement* ReadFromArray(std::vector<std::string>& arValues);
};

class CBezierCurve3Element : public IPathElement
{
	double m_dX1;
	double m_dY1;
	double m_dX2;
	double m_dY2;
	double m_dX3;
	double m_dY3;
public:
	CBezierCurve3Element();
	static IPathElement* ReadFromArray(std::vector<std::string>& arValues);
};

class CArcElement : public IPathElement
{
	double m_dRadiusX;
	double m_dRadiusY;
	double m_dAngle;
	bool   m_bLarge;
	bool   m_bSweep;
	double m_dX;
	double m_dY;
public:
	CArcElement();
	static IPathElement* ReadFromArray(std::vector<std::string>& arValues);
};

class CCloseElement : public IPathElement
{
public:
	CCloseElement();
	static IPathElement* ReadFromArray(std::vector<std::string>& arValues);
};

class CPathObject : public IPageBlock, public CGraphicUnit
{
	bool m_bStroke;
	bool m_bFill;

	enum class ERule
	{
		NonZero,
		Even_Odd
	} m_eRule;

	CColor* m_pFillColor;
	CColor* m_pStrokeColor;

	std::vector<const IPathElement*> m_arElements;

	void AddElement(const IPathElement* pElement);
public:
	CPathObject(CXmlReader& oLiteReader);
	~CPathObject();

	bool Read(CXmlReader& oLiteReader) override;
	void Draw(IRenderer* pRenderer) const override;
};
}

#endif // PATHOBJECT_H
