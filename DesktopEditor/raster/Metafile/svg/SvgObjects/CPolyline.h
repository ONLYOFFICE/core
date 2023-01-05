#ifndef CPOLYLINE_H
#define CPOLYLINE_H

#include "CObjectBase.h"

namespace SVG
{
	class CPolyline : public CObjectBase
	{
	public:
		CPolyline(CObjectBase* pParent = NULL, CGeneralStyle* pBaseStyle = NULL);
		virtual ~CPolyline();

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) override;
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath) override;

		void BeginDraw(IRenderer* pRenderer, int& nTypePath);
		void DrawLines(IRenderer* pRenderer);
		virtual void EndDraw(IRenderer* pRenderer, int& nTypePath);

		std::vector<double> m_arValues;
	};

	class CPolygon : public CPolyline
	{
	public:
		CPolygon(CObjectBase* pParent = NULL, CGeneralStyle* pBaseStyle = NULL);
		virtual ~CPolygon();

		void EndDraw(IRenderer* pRenderer, int& nTypePath) override;
	};
}

#endif // CPOLYLINE_H
