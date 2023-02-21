#ifndef CPOLYLINE_H
#define CPOLYLINE_H

#include "CObjectBase.h"

namespace SVG
{
	class CPolyline : public CSvgGraphicsObject
	{
	public:
		CPolyline(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		void BeginDraw(IRenderer* pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const;
		void DrawLines(IRenderer* pRenderer) const;
		virtual void EndDraw(IRenderer* pRenderer, int& nTypePath) const;

		std::vector<double> m_arValues;
	};

	class CPolygon : public CPolyline
	{
	public:
		CPolygon(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);
		virtual ~CPolygon();

		void EndDraw(IRenderer* pRenderer, int& nTypePath) const override;
	};
}

#endif // CPOLYLINE_H
