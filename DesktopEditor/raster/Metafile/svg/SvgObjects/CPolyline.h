#ifndef CPOLYLINE_H
#define CPOLYLINE_H

#include "CObjectBase.h"

namespace SVG
{
	class CPolyline : public CObjectBase
	{
	public:
		CPolyline(CObjectBase* pParent = NULL);
		virtual ~CPolyline();

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer, const CGeneralStyle* pBaseStyle) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath, const CGeneralStyle* pBaseStyle) const override;

		void BeginDraw(IRenderer* pRenderer, int& nTypePath, const CGeneralStyle* pBaseStyle) const;
		void DrawLines(IRenderer* pRenderer) const;
		virtual void EndDraw(IRenderer* pRenderer, int& nTypePath) const;

		std::vector<double> m_arValues;
	};

	class CPolygon : public CPolyline
	{
	public:
		CPolygon(CObjectBase* pParent = NULL);
		virtual ~CPolygon();

		void EndDraw(IRenderer* pRenderer, int& nTypePath) const override;
	};
}

#endif // CPOLYLINE_H
